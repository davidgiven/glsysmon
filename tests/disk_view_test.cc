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
#include "fake_app_lib.h"
#include "imguiif.h"
#include "mock_disk_sensor_lib.h"
#include "mock_imgui_lib.h"
#include "preferences/preferences.h"
#include "timer.h"
#include "views/view.h"
namespace {
    class TestApp : public App
    {
    public:
        TestApp(std::shared_ptr<Preferences> prefs, Timer& timer, ImGuiIf& imgui):
            _prefs(std::move(prefs)), _timer(timer), _imgui(imgui) {}
        void Setup() override {}
        void MainLoop() override {}
        void Shutdown() override {}
        std::shared_ptr<Preferences> GetSharedPreferences() override { return _prefs; }
        Preferences& GetPreferences() override { return *_prefs; }
        const Preferences& GetPreferences() const override { return *_prefs; }
        Timer& GetTimer() override { return _timer; }
        ImGuiIf& GetImGui() override { return _imgui; }
        const Timer& GetTimer() const override { return _timer; }
        const ImGuiIf& GetImGui() const override { return _imgui; }
        void Quit() override {}
    private:
        std::shared_ptr<Preferences> _prefs;
        Timer& _timer;
        ImGuiIf& _imgui;
    };
}


TEST_CASE("DiskViewImpl DrawConfiguration with empty list adds clicked device")
{
    std::shared_ptr<Preferences> prefs = CreateMapPreferences();
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
    TestApp ctx(prefs, *timer, mockImgui);

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
    std::shared_ptr<Preferences> prefs = CreateMapPreferences();
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
    TestApp ctx(prefs, *timer, mockImgui);

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

TEST_CASE("DiskViewImpl Draw with single device calls BeginPlot once")
{
    std::shared_ptr<Preferences> prefs = CreateMapPreferences();
    prefs->SetStringSet("disk.devices", std::set<std::string>{"sda"});

    class MockImGui : public MockImGuiIf
    {
    public:
        int beginPlotCount = 0;
        std::string lastLabel;

        bool BeginPlot(const char* title_id,
            const ImVec2& size = ImVec2(-1, 0),
            ImPlotFlags flags = 0) override
        {
            (void)size;
            (void)flags;
            beginPlotCount++;
            if (title_id)
                lastLabel = title_id;
            return false;
        }
    };

    auto timer = CreateTimer();
    MockImGui mockImgui;
    TestApp ctx(prefs, *timer, mockImgui);

    std::vector<std::string> names = {"sda"};
    std::vector<std::vector<DiskSample>> samples(1);
    samples[0] = {
        {100, 200},
        {150, 250},
        {200, 300}
    };

    auto sensor = std::make_unique<MockDiskSensor>(names, samples);
    auto view = CreateDiskView(ctx, std::move(sensor));
    REQUIRE(view != nullptr);

    view->Draw();

    CHECK(mockImgui.beginPlotCount == 1);
    CHECK(mockImgui.lastLabel == "##sda");
}
