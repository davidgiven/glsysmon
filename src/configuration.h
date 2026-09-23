#pragma once

class Sensors;
class Views;

class ConfigurationWindow
{
public:
    ConfigurationWindow(const Views& views, const Sensors& sensors);

    void Draw();

private:
    const Views& _views;
    const Sensors& _sensors;
};
