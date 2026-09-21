#include "preferences.h"

#include <charconv>
#include <optional>
#include <set>
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
