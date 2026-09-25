#include "preferences.h"

#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

std::optional<std::string> Preferences::GetString(const std::string& key) const
{
    auto v = Get(key);
    if (!v)
        return std::nullopt;
    return v->GetString();
}

std::optional<int> Preferences::GetInteger(const std::string& key) const
{
    auto v = Get(key);
    if (!v)
        return std::nullopt;
    return v->GetInteger();
}

std::optional<double> Preferences::GetDouble(const std::string& key) const
{
    auto v = Get(key);
    if (!v)
        return std::nullopt;
    return v->GetDouble();
}

std::optional<bool> Preferences::GetBoolean(const std::string& key) const
{
    auto v = Get(key);
    if (!v)
        return std::nullopt;
    return v->GetBoolean();
}

std::optional<std::vector<std::string>> Preferences::GetStringList(
    const std::string& key) const
{
    auto v = Get(key);
    if (!v)
        return std::nullopt;
    return v->GetStringList();
}

std::optional<std::set<std::string>> Preferences::GetStringSet(
    const std::string& key) const
{
    auto v = Get(key);
    if (!v)
        return std::nullopt;
    return v->GetStringSet();
}

void Preferences::SetInteger(const std::string& key, int value)
{
    SetString(key, std::to_string(value));
}

void Preferences::SetDouble(const std::string& key, double value)
{
    SetString(key, std::to_string(value));
}

void Preferences::SetBoolean(const std::string& key, bool value)
{
    SetString(key, value ? "true" : "false");
}

void Preferences::SetStringList(
    const std::string& key, const std::vector<std::string>& value)
{
    std::string joined;
    for (std::size_t i = 0; i < value.size(); ++i)
    {
        if (i != 0)
            joined.push_back(',');
        joined += value[i];
    }
    SetString(key, joined);
}

void Preferences::SetStringSet(
    const std::string& key, const std::set<std::string>& value)
{
    std::string joined;
    bool first = true;
    for (const auto& item : value)
    {
        if (!first)
            joined.push_back(',');
        joined += item;
        first = false;
    }
    SetString(key, joined);
}

void Preferences::ClearAll()
{
    throw std::runtime_error("unsupported operation");
}
