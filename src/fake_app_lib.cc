#include "fake_app_lib.h"

#include "context.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "timer.h"

void FakeApp::Setup() {}

void FakeApp::MainLoop() {}

void FakeApp::Shutdown() {}

std::shared_ptr<Preferences> FakeApp::GetPreferences()
{
    return nullptr;
}

Context& FakeApp::GetContext()
{
    static auto dp = CreateMapPreferences();
    static auto dt = CreateTimer();
    static auto di = CreateImGui();
    static Context dc(*this, *di, *dp, *dt);
    return dc;
}

void FakeApp::Quit() {}
