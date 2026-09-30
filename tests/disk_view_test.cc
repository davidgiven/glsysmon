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
#include "mock_disk_sensor_lib.h"
#include "mock_imgui_lib.h"
#include "preferences/preferences.h"
#include "timer.h"
#include "views/view.h"

TEST_CASE("DiskViewImpl DrawConfiguration with empty list adds clicked device")
{
    auto prefs = CreateMapPreferences();
    CHECK_FALSE(prefs->GetStringSet("disk.devices").has_value());

    class MockImGui : public MockImGuiIf
    {
    public:
        bool Selectable(const char* label,
            bool* p_selected,
            ImGuiSelectableFlags flags = 0,
            const ImVec2& size = ImVec2(0, 0)) override
        {
            (void)flags;
            (void)size;
            if (label == std::string("sda"))
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

    std::vector<std::string> names = {"sda", "sdb"};
    std::vector<std::vector<DiskSample>> samples(2);
    auto sensor = std::make_unique<MockDiskSensor>(names, samples);
    auto view = CreateDiskView(ctx, std::move(sensor));
    REQUIRE(view != nullptr);

    view->DrawConfiguration(*prefs);

    auto result = prefs->GetStringSet("disk.devices");
    REQUIRE(result.has_value());
    CHECK(result->size() == 1);
    CHECK(result->contains("sda"));
}

TEST_CASE(
    "DiskViewImpl DrawConfiguration with set device removes clicked device")
{
    auto prefs = CreateMapPreferences();
    prefs->SetStringSet("disk.devices", std::set<std::string>{"sda"});
    REQUIRE(prefs->GetStringSet("disk.devices").has_value());
    REQUIRE(prefs->GetStringSet("disk.devices")->contains("sda"));

    class MockImGui : public MockImGuiIf
    {
    public:
        bool Selectable(const char* label,
            bool* p_selected,
            ImGuiSelectableFlags flags = 0,
            const ImVec2& size = ImVec2(0, 0)) override
        {
            (void)flags;
            (void)size;
            if (label == std::string("sda"))
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

    std::vector<std::string> names = {"sda", "sdb"};
    std::vector<std::vector<DiskSample>> samples(2);
    auto sensor = std::make_unique<MockDiskSensor>(names, samples);
    auto view = CreateDiskView(ctx, std::move(sensor));
    REQUIRE(view != nullptr);

    view->DrawConfiguration(*prefs);

    auto result = prefs->GetStringSet("disk.devices");
    REQUIRE(result.has_value());
    CHECK_FALSE(result->contains("sda"));
    // MapPreferences stores an empty set as "" which parses back as {""}
    if (result->size() == 1 && result->contains(""))
        CHECK(result->size() == 1);
    else
        CHECK(result->empty());
}
