#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <memory>
#include <string>
#include <vector>
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <cstring>

#include "app.h"
#include "context.h"
#include "imguiif.h"
#include "mock_disk_sensor_lib.h"
#include "mock_imgui_lib.h"
#include "preferences/preferences.h"
#include "timer.h"
#include "views/view.h"

namespace
{

    class MockImGui : public MockImGuiIf
    {
    public:
        int dummyCalls = 0;
        int beginPlotCalls = 0;
        int endPlotCalls = 0;
        int colormapCalls = 0;
        int contentAvailCalls = 0;
        int calcTextSizeCalls = 0;

        void Dummy(const ImVec2& size) override
        {
            (void)size;
            ++dummyCalls;
        }
        bool BeginPlot(const char* title_id,
            const ImVec2& size = ImVec2(-1, 0),
            ImPlotFlags flags = 0) override
        {
            (void)title_id;
            (void)size;
            (void)flags;
            ++beginPlotCalls;
            return false;
        }
        void EndPlot() override
        {
            ++endPlotCalls;
        }
        ImVec4 GetColormapColor(
            int idx, ImPlotColormap cmap = IMPLOT_AUTO) override
        {
            (void)idx;
            (void)cmap;
            ++colormapCalls;
            return ImVec4(1, 0, 0, 1);
        }
        ImVec2 GetContentRegionAvail() override
        {
            ++contentAvailCalls;
            return ImVec2(200, 0);
        }
        ImVec2 CalcTextSize(const char* text,
            const char* text_end = NULL,
            bool hide_text_after_double_hash = false,
            float wrap_width = -1.0f) override
        {
            (void)text_end;
            (void)hide_text_after_double_hash;
            (void)wrap_width;
            ++calcTextSizeCalls;
            if (!text)
                return ImVec2(0, 0);
            return ImVec2(static_cast<float>(strlen(text)) * 6.0f, 13.0f);
        }
    };

    struct DummyApp : public App
    {
        void Setup() override {}
        void MainLoop() override {}
        void Shutdown() override {}
        std::shared_ptr<Preferences> GetPreferences() override
        {
            return nullptr;
        }
        Context& GetContext() override
        {
            static auto dp = CreateMapPreferences();
            static auto dt = CreateTimer();
            static auto di = std::make_unique<MockImGuiIf>();
            static Context dc(*this, *di, *dp, *dt);
            return dc;
        }
        void Quit() override {}
    };

} // namespace

TEST_CASE(
    "DiskViewImpl with mocked sensor and mocked ImGuiIf draws filtered devices")
{
    auto prefs = CreateMapPreferences();
    prefs->SetStringSet("disk.devices", std::set<std::string>{"sda", "sdb"});
    auto timer = CreateTimer();
    auto mockImgui = std::make_unique<MockImGui>();
    MockImGui* imguiPtr = mockImgui.get();
    DummyApp app;
    Context ctx(app, *mockImgui, *prefs, *timer);

    // prepare mocked sensor with 2 channels, 10 samples each
    std::vector<std::string> names = {"sda", "sdb"};
    std::vector<std::vector<DiskSample>> samples(2);
    for (int i = 0; i < 10; ++i)
    {
        samples[0].push_back(
            DiskSample{100000.0 * (i + 1), 200000.0 * (i + 1)});
        samples[1].push_back(DiskSample{50000.0 * (i + 1), 80000.0 * (i + 1)});
    }
    auto sensor = std::make_unique<MockDiskSensor>(names, samples);
    auto view = CreateDiskView(ctx, std::move(sensor));
    REQUIRE(view != nullptr);
    CHECK(view->GetHumanName() == "Disk");
    CHECK(view->GetPrefName() == "disk");
    CHECK(view->GetSensors().size() == 1);
    CHECK(&view->GetImGui() == imguiPtr);
    const auto& cview = static_cast<const View&>(*view);
    CHECK(cview.GetSensors().size() == 1);
    CHECK(&cview.GetImGui() == imguiPtr);

    // should draw both devices (BeginPlot is mocked to return false, so body
    // including GetColormapColor is not executed; we verify filtering via
    // BeginPlot count)
    view->Draw();
    CHECK(imguiPtr->beginPlotCalls == 2);
    CHECK(imguiPtr->dummyCalls >= 1);

    // filter to single device
    prefs->SetStringSet("disk.devices", std::set<std::string>{"sda"});
    imguiPtr->beginPlotCalls = 0;
    view->Draw();
    CHECK(imguiPtr->beginPlotCalls == 1);

    // filter to none - no plots
    prefs->SetStringSet("disk.devices", std::set<std::string>{});
    imguiPtr->beginPlotCalls = 0;
    view->Draw();
    CHECK(imguiPtr->beginPlotCalls == 0);
}

TEST_CASE("DiskViewImpl with empty sensor does not draw")
{
    auto prefs = CreateMapPreferences();
    prefs->SetStringSet("disk.devices", std::set<std::string>{"sda"});
    auto timer = CreateTimer();
    auto mockImgui = std::make_unique<MockImGui>();
    MockImGui* imguiPtr = mockImgui.get();
    DummyApp app;
    Context ctx(app, *mockImgui, *prefs, *timer);
    auto sensor = std::make_unique<MockDiskSensor>(); // empty
    auto view = CreateDiskView(ctx, std::move(sensor));
    view->Draw();
    CHECK(imguiPtr->beginPlotCalls == 0);
}

TEST_CASE("DiskViewImpl with zero samples does not draw")
{
    auto prefs = CreateMapPreferences();
    prefs->SetStringSet("disk.devices", std::set<std::string>{"sda"});
    auto timer = CreateTimer();
    auto mockImgui = std::make_unique<MockImGui>();
    MockImGui* imguiPtr = mockImgui.get();
    DummyApp app;
    Context ctx(app, *mockImgui, *prefs, *timer);
    std::vector<std::string> names = {"sda"};
    std::vector<std::vector<DiskSample>> samples(
        1); // one channel but 0 samples
    auto sensor = std::make_unique<MockDiskSensor>(names, samples);
    auto view = CreateDiskView(ctx, std::move(sensor));
    view->Draw();
    CHECK(imguiPtr->beginPlotCalls == 0);
}
