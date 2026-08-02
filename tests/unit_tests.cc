#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <fruit/fruit.h>

#include <string>
#include <vector>

#include "components.h"
#include "views/catalogue.h"

namespace
{

    // GetUiComponent() and GetCliPreferencesComponent() require CliArgs but
    // take no parameters; wrap them so tests can bind their own args
    // (bindInstance stores a reference, so args must outlive the Injector).
    fruit::Component<fruit::Annotated<CliPreference, Preferences>>
    GetCliPreferencesWithArgs(CliArgs* args)
    {
        return fruit::createComponent()
            .install(GetCliPreferencesComponent)
            .bindInstance(*args);
    }

    fruit::Component<Ui> GetUiWithArgs(CliArgs* args)
    {
        return fruit::createComponent()
            .install(GetUiComponent)
            .bindInstance(*args);
    }

} // namespace

TEST_CASE("Fruit resolves the UI component")
{
    CliArgs args;
    fruit::Injector<Ui> injector(GetUiWithArgs, &args);
    CHECK(injector.get<Ui*>() != nullptr);
}

TEST_CASE("Fruit resolves the view component")
{
    fruit::Injector<View> injector(GetViewComponent);
    CHECK(injector.get<View*>() != nullptr);
}

TEST_CASE("View catalogue exposes HostnameView and resolves it")
{
    const auto& catalogue = GetViewCatalogue();
    REQUIRE(catalogue.find("HostnameView") != catalogue.end());

    fruit::Injector<View> injector(catalogue.at("HostnameView"));
    CHECK(injector.get<View*>() != nullptr);
}

TEST_CASE("Hostname sensor returns the current hostname")
{
    fruit::Injector<HostnameSensor> injector(GetHostnameSensorComponent);
    HostnameSensor* sensor = injector.get<HostnameSensor*>();

    const std::string hostname = sensor->GetHostname();
    CHECK_FALSE(hostname.empty());
    CHECK(hostname.size() < 256);
}

TEST_CASE("Fruit resolves the ImGui frame renderer component")
{
    fruit::Injector<ImGuiFrameRenderer> injector(
        GetImGuiFrameRendererComponent);
    CHECK(injector.get<ImGuiFrameRenderer*>() != nullptr);
}

TEST_CASE("CLI preferences default to left / 240 / monitor 0")
{
    CliArgs args;
    fruit::Injector<fruit::Annotated<CliPreference, Preferences>> injector(
        GetCliPreferencesWithArgs, &args);
    Preferences* prefs =
        injector.get<fruit::Annotated<CliPreference, Preferences*>>();

    CHECK(GlobalPreferencesFetcher::GetSide(*prefs) == "left");
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 240);
    CHECK(GlobalPreferencesFetcher::GetMonitor(*prefs) == 0);
    CHECK(GlobalPreferencesFetcher::GetViews(*prefs) ==
          std::vector<std::string>{"HostnameView"});
}

TEST_CASE("CLI --views= parses a comma-separated list")
{
    CliArgs args;
    args.values = {"--views=HostnameView,CpuView"};
    fruit::Injector<fruit::Annotated<CliPreference, Preferences>> injector(
        GetCliPreferencesWithArgs, &args);
    Preferences* prefs =
        injector.get<fruit::Annotated<CliPreference, Preferences*>>();

    CHECK(GlobalPreferencesFetcher::GetViews(*prefs) ==
          std::vector<std::string>{"HostnameView", "CpuView"});
}

TEST_CASE("CLI preferences parse --side= / --size= / --monitor=")
{
    CliArgs args;
    args.values = {"--side=right", "--size=120", "--monitor=2"};
    fruit::Injector<fruit::Annotated<CliPreference, Preferences>> injector(
        GetCliPreferencesWithArgs, &args);
    Preferences* prefs =
        injector.get<fruit::Annotated<CliPreference, Preferences*>>();

    CHECK(GlobalPreferencesFetcher::GetSide(*prefs) == "right");
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 120);
    CHECK(GlobalPreferencesFetcher::GetMonitor(*prefs) == 2);
}

TEST_CASE("CLI preference values that are not integers fall back to defaults")
{
    CliArgs args;
    args.values = {"--size=abc"};
    fruit::Injector<fruit::Annotated<CliPreference, Preferences>> injector(
        GetCliPreferencesWithArgs, &args);
    Preferences* prefs =
        injector.get<fruit::Annotated<CliPreference, Preferences*>>();

    CHECK(prefs->GetInteger("size") == std::nullopt);
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 240);
}

TEST_CASE("CliArgs is equality-comparable and hashable")
{
    CliArgs a;
    a.values = {"--side=left", "--size=240"};
    CliArgs b;
    b.values = {"--side=left", "--size=240"};

    CHECK(a == b);
    CHECK(std::hash<CliArgs>()(a) == std::hash<CliArgs>()(b));
}
