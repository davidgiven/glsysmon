#pragma once

#include "app.h"

class FakeApp : public App
{
public:
    void Setup() override;
    void MainLoop() override;
    void Shutdown() override;
    std::shared_ptr<Preferences> GetPreferences() override;
    Preferences& GetPreferencesRef() override;
    const Preferences& GetPreferencesRef() const override;
    Timer& GetTimer() override;
    ImGuiIf& GetImGui() override;
    const Timer& GetTimer() const override;
    const ImGuiIf& GetImGui() const override;
    void Quit() override;
};
