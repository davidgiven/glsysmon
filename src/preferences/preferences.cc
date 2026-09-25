#include "preferences.h"

#include <charconv>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

class StringValue : public Value
{
public:
    explicit StringValue(std::string s): _str(std::move(s)) {}

    Type GetType() const override
    {
        if (_str.find(',') != std::string::npos)
            return Type::StringList;
        if (GetInteger().has_value())
            return Type::Integer;
        if (GetDouble().has_value())
            return Type::Double;
        if (GetBoolean().has_value())
            return Type::Boolean;
        return Type::String;
    }

    std::optional<std::string> GetString() const override
    {
        return _str;
    }

    std::optional<int> GetInteger() const override
    {
        int value = 0;
        const char* begin = _str.data();
        const char* end = begin + _str.size();
        const auto result = std::from_chars(begin, end, value);
        if (result.ec != std::errc{} || result.ptr != end)
            return std::nullopt;
        return value;
    }

    std::optional<double> GetDouble() const override
    {
        double value = 0;
        const char* begin = _str.data();
        const char* end = begin + _str.size();
        const auto result = std::from_chars(begin, end, value);
        if (result.ec != std::errc{} || result.ptr != end)
            return std::nullopt;
        return value;
    }

    std::optional<bool> GetBoolean() const override
    {
        if (_str == "true" || _str == "1")
            return true;
        if (_str == "false" || _str == "0")
            return false;
        return std::nullopt;
    }

    std::optional<std::vector<std::string>> GetStringList() const override
    {
        std::vector<std::string> result;
        std::size_t start = 0;
        while (start <= _str.size())
        {
            const std::size_t comma = _str.find(',', start);
            const std::size_t end =
                comma == std::string::npos ? _str.size() : comma;
            result.push_back(_str.substr(start, end - start));
            if (comma == std::string::npos)
                break;
            start = comma + 1;
        }
        return result;
    }

    std::optional<std::set<std::string>> GetStringSet() const override
    {
        auto list = GetStringList();
        if (!list)
            return std::nullopt;
        std::set<std::string> result;
        for (const auto& item : *list)
            result.insert(item);
        return result;
    }

private:
    std::string _str;
};

std::unique_ptr<Value> CreateStringValue(const std::string& s)
{
    return std::make_unique<StringValue>(s);
}

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
