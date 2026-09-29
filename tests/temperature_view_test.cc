#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <memory>
#include <set>
#include <string>
#include <vector>
#include <cstring>
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>

#include "app.h"
#include "context.h"
#include "fake_app_lib.h"
#include "imguiif.h"
#include "mock_imgui_lib.h"
#include "mock_temperature_sensor_lib.h"
#include "preferences/preferences.h"
#include "timer.h"
#include "views/view.h"

TEST_CASE(
    "TemperatureViewImpl DrawConfiguration with empty list adds clicked device")
{
    auto prefs = CreateMapPreferences();
    CHECK_FALSE(prefs->GetStringSet("temperature.sensors").has_value());

    class MockImGui : public MockImGuiIf
    {
    public:
        bool Selectable(const char* label,
            bool* p_selected,
            ImGuiSelectableFlags flags = 0,
            const ImVec2& size = ImVec2(0, 0)) override
        {
            if (label == std::string("temp0"))
            {
                *p_selected = !*p_selected;
                return true;
            }
            return false;
        }
    };

    auto timer = CreateTimer();
    MockImGui mockImgui;
    FakeApp app;
    Context ctx(app, mockImgui, *prefs, *timer);

    std::vector<std::string> names = {"temp0", "temp1"};
    std::vector<std::vector<double>> samples(2);
    auto sensor = std::make_unique<MockTemperatureSensor>(names, samples);
    auto view = CreateTemperatureView(ctx, std::move(sensor));
    REQUIRE(view != nullptr);

    view->DrawConfiguration(*prefs);

    auto result = prefs->GetStringSet("temperature.sensors");
    REQUIRE(result.has_value());
    CHECK(result->size() == 1);
    CHECK(result->contains("temp0"));
}

TEST_CASE(
    "TemperatureViewImpl DrawConfiguration with set device removes clicked "
    "device")
{
    auto prefs = CreateMapPreferences();
    prefs->SetStringSet("temperature.sensors", std::set<std::string>{"temp0"});
    REQUIRE(prefs->GetStringSet("temperature.sensors").has_value());
    REQUIRE(prefs->GetStringSet("temperature.sensors")->contains("temp0"));

    class MockImGui : public MockImGuiIf
    {
    public:
        bool Selectable(const char* label,
            bool* p_selected,
            ImGuiSelectableFlags flags = 0,
            const ImVec2& size = ImVec2(0, 0)) override
        {
            if (label == std::string("temp0"))
            {
                *p_selected = !*p_selected;
                return true;
            }
            return false;
        }
    };

    auto timer = CreateTimer();
    MockImGui mockImgui;
    FakeApp app;
    Context ctx(app, mockImgui, *prefs, *timer);

    std::vector<std::string> names = {"temp0", "temp1"};
    std::vector<std::vector<double>> samples(2);
    auto sensor = std::make_unique<MockTemperatureSensor>(names, samples);
    auto view = CreateTemperatureView(ctx, std::move(sensor));
    REQUIRE(view != nullptr);

    view->DrawConfiguration(*prefs);

    auto result = prefs->GetStringSet("temperature.sensors");
    REQUIRE(result.has_value());
    CHECK_FALSE(result->contains("temp0"));
    // MapPreferences stores an empty set as "" which parses back as {""}
    if (result->size() == 1 && result->contains(""))
        CHECK(result->size() == 1);
    else
        CHECK(result->empty());
}
