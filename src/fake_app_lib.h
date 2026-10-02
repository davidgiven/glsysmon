#pragma once

#include "app.h"

class FakeApp : public App
{
public:
    void Setup() override;
    void MainLoop() override;
    void Shutdown() override;
    std::shared_ptr<Preferences> GetSharedPreferences() override;
    Preferences& GetPreferences() override;
    const Preferences& GetPreferences() const override;
    Timer& GetTimer() override;
    ImGuiIf& GetImGui() override;
    const Timer& GetTimer() const override;
    const ImGuiIf& GetImGui() const override;
    void Quit() override;
};
