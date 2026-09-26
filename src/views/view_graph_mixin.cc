#include "views/view_graph_mixin.h"

#include <imgui.h>

#include <functional>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "views/style.h"

void ViewGraphMixin::DrawConfiguration(Preferences& preferences)
{
    View::DrawConfiguration(preferences);

    std::string key = GetPrefName() + ".graph_height";
    int height = preferences.GetInteger(key).value_or(40);
    if (ImGui::InputInt("Graph height", &height))
    {
        if (height < 1)
            height = 1;
        preferences.SetInteger(key, height);
    }

    bool show = GetShowChannelName(preferences);
    if (ImGui::Checkbox("Show labels", &show))
        preferences.SetBoolean(GetPrefName() + ".show_labels", show);
}

bool ViewGraphMixin::GetShowChannelName(const Preferences& prefs) const
{
    return prefs.GetBoolean(GetPrefName() + ".show_labels").value_or(true);
}

std::string ViewGraphMixin::FilterTitle(
    const std::string& title, bool show) const
{
    if (show)
        return title;
    const std::size_t pos = title.find('\n');
    if (pos != std::string::npos)
        return title.substr(pos + 1);
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

void ViewGraphMixin::DrawGraph(const std::string& title,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height)
{
    Style::DrawGraph(title, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(const char* title,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height)
{
    Style::DrawGraph(title, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(const std::vector<std::string>& titles,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height)
{
    Style::DrawGraph(titles, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(
    const std::string& title, std::function<void()> body)
{
    Style::DrawGraph(title, std::move(body));
}

void ViewGraphMixin::DrawGraph(const char* title, std::function<void()> body)
{
    Style::DrawGraph(title, std::move(body));
}

void ViewGraphMixin::DrawGraph(
    const std::vector<std::string>& titles, std::function<void()> body)
{
    Style::DrawGraph(titles, std::move(body));
}

void ViewGraphMixin::DrawGraph(const Preferences& prefs,
    const std::string& title,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height) const
{
    const bool show = GetShowChannelName(prefs);
    std::string filtered = FilterTitle(title, show);
    Style::DrawGraph(filtered, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(const Preferences& prefs,
    const char* title,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height) const
{
    DrawGraph(
        prefs, std::string(title), n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(const Preferences& prefs,
    const std::vector<std::string>& titles,
    int n,
    double yMin,
    double yMax,
    std::function<void()> body,
    float height) const
{
    const bool show = GetShowChannelName(prefs);
    std::vector<std::string> filtered = FilterTitles(titles, show);
    Style::DrawGraph(filtered, n, yMin, yMax, std::move(body), height);
}

void ViewGraphMixin::DrawGraph(const Preferences& prefs,
    const std::string& title,
    std::function<void()> body) const
{
    const bool show = GetShowChannelName(prefs);
    std::string filtered = FilterTitle(title, show);
    Style::DrawGraph(filtered, std::move(body));
}

void ViewGraphMixin::DrawGraph(const Preferences& prefs,
    const char* title,
    std::function<void()> body) const
{
    DrawGraph(prefs, std::string(title), std::move(body));
}

void ViewGraphMixin::DrawGraph(const Preferences& prefs,
    const std::vector<std::string>& titles,
    std::function<void()> body) const
{
    const bool show = GetShowChannelName(prefs);
    std::vector<std::string> filtered = FilterTitles(titles, show);
    Style::DrawGraph(filtered, std::move(body));
}
