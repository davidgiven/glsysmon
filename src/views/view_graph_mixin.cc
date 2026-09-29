#include "views/view_graph_mixin.h"

#include "imguiif.h"
#include <imgui.h>

#include <functional>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "views/style.h"

void ViewGraphMixin::DrawConfiguration(Preferences& preferences)
{
    View::DrawConfiguration(preferences);

    ImGuiIf& imgui = GetImGui();
    int height = GetGraphHeight(preferences);
    if (imgui.InputInt("Graph height", &height))
        SetGraphHeight(preferences, height);

    bool show = GetShowChannelName(preferences);
    if (imgui.Checkbox("Show labels", &show))
        SetShowChannelName(preferences, show);
}

int ViewGraphMixin::GetGraphHeight(const Preferences& prefs) const
{
    return prefs.GetInteger(GetPrefName() + ".graph_height").value_or(40);
}

void ViewGraphMixin::SetGraphHeight(Preferences& prefs, int height) const
{
    if (height < 1)
        height = 1;
    prefs.SetInteger(GetPrefName() + ".graph_height", height);
}

bool ViewGraphMixin::GetShowChannelName(const Preferences& prefs) const
{
    return prefs.GetBoolean(GetPrefName() + ".show_labels").value_or(true);
}

void ViewGraphMixin::SetShowChannelName(Preferences& prefs, bool value) const
{
    prefs.SetBoolean(GetPrefName() + ".show_labels", value);
}

std::string ViewGraphMixin::FilterTitle(
    const std::string& title, bool show) const
{
    if (show)
        return title;
    return "";
}

std::vector<std::string> ViewGraphMixin::FilterTitles(
    const std::vector<std::string>& titles, bool show) const
{
    if (show)
        return titles;
    if (titles.empty())
        return titles;
    std::vector<std::string> filtered = titles;
    filtered[0].clear();
    return filtered;
}

void ViewGraphMixin::DrawGraph(ImGuiIf& imgui, const std::string& title,
    const std::string& subtitle,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height)
{
    Style::DrawGraph(imgui, title, subtitle, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(ImGuiIf& imgui, const std::vector<std::string>& titles,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height)
{
    Style::DrawGraph(imgui, titles, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(ImGuiIf& imgui, const std::string& title,
    const std::string& subtitle,
    std::function<void()> body)
{
    Style::DrawGraph(imgui, title, subtitle, std::move(body));
}

void ViewGraphMixin::DrawGraph(ImGuiIf& imgui,
    const std::vector<std::string>& titles, std::function<void()> body)
{
    Style::DrawGraph(imgui, titles, std::move(body));
}

void ViewGraphMixin::DrawGraph(ImGuiIf& imgui, const Preferences& prefs,
    const std::string& title,
    const std::string& subtitle,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height) const
{
    const bool show = GetShowChannelName(prefs);
    std::string filtered = FilterTitle(title, show);
    Style::DrawGraph(imgui,
        filtered, subtitle, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(ImGuiIf& imgui, const Preferences& prefs,
    const std::vector<std::string>& titles,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height) const
{
    const bool show = GetShowChannelName(prefs);
    std::vector<std::string> filtered = FilterTitles(titles, show);
    Style::DrawGraph(imgui, filtered, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(ImGuiIf& imgui, const Preferences& prefs,
    const std::string& title,
    const std::string& subtitle,
    std::function<void()> body) const
{
    const bool show = GetShowChannelName(prefs);
    std::string filtered = FilterTitle(title, show);
    Style::DrawGraph(imgui, filtered, subtitle, std::move(body));
}

void ViewGraphMixin::DrawGraph(ImGuiIf& imgui, const Preferences& prefs,
    const std::vector<std::string>& titles,
    std::function<void()> body) const
{
    const bool show = GetShowChannelName(prefs);
    std::vector<std::string> filtered = FilterTitles(titles, show);
    Style::DrawGraph(imgui, filtered, std::move(body));
}
