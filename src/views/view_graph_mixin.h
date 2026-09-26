#pragma once

#include <functional>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "views/view.h"

class ViewGraphMixin : public View
{
public:
    void DrawConfiguration(Preferences& preferences) override;

protected:
    void DrawGraph(const std::string& title,
        const std::string& subtitle,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40);
    void DrawGraph(const std::vector<std::string>& titles,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40);
    void DrawGraph(const std::string& title,
        const std::string& subtitle,
        std::function<void()> body);
    void DrawGraph(
        const std::vector<std::string>& titles, std::function<void()> body);

    void DrawGraph(const Preferences& prefs,
        const std::string& title,
        const std::string& subtitle,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40) const;
    void DrawGraph(const Preferences& prefs,
        const std::vector<std::string>& titles,
        int n,
        double yMin,
        double yMax,
        std::function<void()> body,
        float height = 40) const;
    void DrawGraph(const Preferences& prefs,
        const std::string& title,
        const std::string& subtitle,
        std::function<void()> body) const;
    void DrawGraph(const Preferences& prefs,
        const std::vector<std::string>& titles,
        std::function<void()> body) const;

    int GetGraphHeight(const Preferences& prefs) const;
    void SetGraphHeight(Preferences& prefs, int height) const;

private:
    bool GetShowChannelName(const Preferences& prefs) const;
    void SetShowChannelName(Preferences& prefs, bool value) const;
    std::string FilterTitle(const std::string& title, bool show) const;
    std::vector<std::string> FilterTitles(
        const std::vector<std::string>& titles, bool show) const;
};
