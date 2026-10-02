#include "fake_app_lib.h"

#include "imguiif.h"
#include "preferences/preferences.h"
#include "timer.h"

void FakeApp::Setup() {}

void FakeApp::MainLoop() {}

void FakeApp::Shutdown() {}

std::shared_ptr<Preferences> FakeApp::GetPreferences()
{
    static auto dp = CreateMapPreferences();
    return dp;
}

Preferences& FakeApp::GetPreferencesRef()
{
    return *GetPreferences();
}

const Preferences& FakeApp::GetPreferencesRef() const
{
    return *const_cast<FakeApp*>(this)->GetPreferences();
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
