#pragma once

#include "app.h"

class FakeApp : public App
{
public:
    void Setup() override;
    void MainLoop() override;
    void Shutdown() override;
    std::shared_ptr<Preferences> GetPreferences() override;
    Context& GetContext() override;
    void Quit() override;
};
