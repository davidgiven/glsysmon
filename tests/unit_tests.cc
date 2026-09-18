#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <memory>
#include <string>
#include <vector>

#include "components.h"
#include "views/catalogue.h"

namespace
{

    class FakeHostnameSensor : public HostnameSensor
    {
    public:
        std::string GetHostname() override
        {
            return "fake-hostname";
        }
    };

} // namespace

TEST_CASE("Fruit resolves the UI component")
{
    CliArgs args;
    auto prefs = CreatePreferences(args);
    auto ui = CreateUi(*prefs);
    CHECK(ui != nullptr);
}

TEST_CASE("A sensor bound after GetUiComponent overrides the real one")
{
    auto fake = std::make_unique<FakeHostnameSensor>();
    CHECK(fake->GetHostname() == "fake-hostname");

    auto view = CreateHostnameView(std::move(fake));
    CHECK(view != nullptr);
}

TEST_CASE("Fruit resolves the view component")
{
    auto view = CreateHostnameView();
    CHECK(view != nullptr);
}

TEST_CASE("View catalogue exposes HostnameView and resolves it")
{
    const auto& catalogue = GetViewCatalogue();
    REQUIRE(catalogue.find("HostnameView") != catalogue.end());

    auto view = catalogue.at("HostnameView")();
    CHECK(view != nullptr);
}

TEST_CASE("Hostname sensor returns the current hostname")
{
    auto sensor = CreateHostnameSensor();

    const std::string hostname = sensor->GetHostname();
    CHECK_FALSE(hostname.empty());
    CHECK(hostname.size() < 256);
}

TEST_CASE("Fruit resolves the ImGui frame renderer component")
{
    auto renderer = CreateImGuiFrameRenderer();
    CHECK(renderer != nullptr);
}

TEST_CASE("CLI preferences default to left / 240 / monitor 0")
{
    CliArgs args;
    auto prefs = CreateCliPreferences(args);

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
    auto prefs = CreateCliPreferences(args);

    CHECK(GlobalPreferencesFetcher::GetViews(*prefs) ==
          std::vector<std::string>{"HostnameView", "CpuView"});
}

TEST_CASE("CLI preferences parse --side= / --size= / --monitor=")
{
    CliArgs args;
    args.values = {"--side=right", "--size=120", "--monitor=2"};
    auto prefs = CreateCliPreferences(args);

    CHECK(GlobalPreferencesFetcher::GetSide(*prefs) == "right");
    CHECK(GlobalPreferencesFetcher::GetSize(*prefs) == 120);
    CHECK(GlobalPreferencesFetcher::GetMonitor(*prefs) == 2);
}

TEST_CASE("CLI preference values that are not integers fall back to defaults")
{
    CliArgs args;
    args.values = {"--size=abc"};
    auto prefs = CreateCliPreferences(args);

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
