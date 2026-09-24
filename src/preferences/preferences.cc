#include "preferences.h"

#include <charconv>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

std::optional<int> Preferences::GetInteger(const std::string& key) const
{
    auto str = GetString(key);
    if (!str)
        return std::nullopt;
    int value = 0;
    const char* begin = str->data();
    const char* end = begin + str->size();
    const auto result = std::from_chars(begin, end, value);
    if (result.ec != std::errc{} || result.ptr != end)
        return std::nullopt;
    return value;
}

std::optional<double> Preferences::GetDouble(const std::string& key) const
{
    auto str = GetString(key);
    if (!str)
        return std::nullopt;
    double value = 0;
    const char* begin = str->data();
    const char* end = begin + str->size();
    const auto result = std::from_chars(begin, end, value);
    if (result.ec != std::errc{} || result.ptr != end)
        return std::nullopt;
    return value;
}

std::optional<bool> Preferences::GetBoolean(const std::string& key) const
{
    auto str = GetString(key);
    if (!str)
        return std::nullopt;
    if (*str == "true" || *str == "1")
        return true;
    if (*str == "false" || *str == "0")
        return false;
    return std::nullopt;
}

std::optional<std::vector<std::string>> Preferences::GetStringList(
    const std::string& key) const
{
    auto str = GetString(key);
    if (!str)
        return std::nullopt;
    std::vector<std::string> result;
    std::size_t start = 0;
    while (start <= str->size())
    {
        const std::size_t comma = str->find(',', start);
        const std::size_t end =
            comma == std::string::npos ? str->size() : comma;
        result.push_back(str->substr(start, end - start));
        if (comma == std::string::npos)
            break;
        start = comma + 1;
    }
    return result;
}

std::optional<std::set<std::string>> Preferences::GetStringSet(
    const std::string& key) const
{
    auto list = GetStringList(key);
    if (!list)
        return std::nullopt;
    std::set<std::string> result;
    for (const auto& item : *list)
        result.insert(item);
    return result;
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
