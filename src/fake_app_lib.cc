#include "fake_app_lib.h"

#include "imguiif.h"
#include "preferences/preferences.h"
#include "timer.h"

void FakeApp::Setup() {}

void FakeApp::MainLoop() {}

void FakeApp::Shutdown() {}

std::shared_ptr<Preferences> FakeApp::GetSharedPreferences()
{
    static auto dp = CreateMapPreferences();
    return dp;
}

Preferences& FakeApp::GetPreferences()
{
    return *GetSharedPreferences();
}

const Preferences& FakeApp::GetPreferences() const
{
    return *const_cast<FakeApp*>(this)->GetSharedPreferences();
}

Timer& FakeApp::GetTimer()
{
    static auto dt = CreateTimer();
    return *dt;
}

ImGuiIf& FakeApp::GetImGui()
{
    static auto di = CreateImGui();
    return *di;
}

const Timer& FakeApp::GetTimer() const
{
    return const_cast<FakeApp*>(this)->GetTimer();
}

const ImGuiIf& FakeApp::GetImGui() const
{
    return const_cast<FakeApp*>(this)->GetImGui();
}

void FakeApp::Quit() {}
