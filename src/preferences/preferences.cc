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

void Preferences::SetString(const std::string& key, const std::string& value)
{
    auto v = Add(key);
    if (v)
        v->SetString(value);
}

void Preferences::SetInteger(const std::string& key, int value)
{
    auto v = Add(key);
    if (v)
        v->SetInteger(value);
}

void Preferences::SetDouble(const std::string& key, double value)
{
    auto v = Add(key);
    if (v)
        v->SetDouble(value);
}

void Preferences::SetBoolean(const std::string& key, bool value)
{
    auto v = Add(key);
    if (v)
        v->SetBoolean(value);
}

void Preferences::SetStringList(
    const std::string& key, const std::vector<std::string>& value)
{
    auto v = Add(key);
    if (v)
        v->SetStringList(value);
}

void Preferences::SetStringSet(
    const std::string& key, const std::set<std::string>& value)
{
    auto v = Add(key);
    if (v)
        v->SetStringSet(value);
}

void Preferences::ClearAll()
{
    throw std::runtime_error("unsupported operation");
}
