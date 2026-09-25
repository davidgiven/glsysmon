#include "string_value.h"

#include <charconv>
#include <optional>
#include <set>
#include <string>
#include <vector>

StringValue::StringValue(std::string name, std::string s):
    _name(std::move(name)),
    _str(std::move(s))
{
}

std::string StringValue::GetName() const
{
    return _name;
}

Value::Type StringValue::GetType() const
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

std::optional<std::string> StringValue::GetString() const
{
    return _str;
}

std::optional<int> StringValue::GetInteger() const
{
    int value = 0;
    const char* begin = _str.data();
    const char* end = begin + _str.size();
    const auto result = std::from_chars(begin, end, value);
    if (result.ec != std::errc{} || result.ptr != end)
        return std::nullopt;
    return value;
}

std::optional<double> StringValue::GetDouble() const
{
    double value = 0;
    const char* begin = _str.data();
    const char* end = begin + _str.size();
    const auto result = std::from_chars(begin, end, value);
    if (result.ec != std::errc{} || result.ptr != end)
        return std::nullopt;
    return value;
}

std::optional<bool> StringValue::GetBoolean() const
{
    if (_str == "true" || _str == "1")
        return true;
    if (_str == "false" || _str == "0")
        return false;
    return std::nullopt;
}

std::optional<std::vector<std::string>> StringValue::GetStringList() const
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

std::optional<std::set<std::string>> StringValue::GetStringSet() const
{
    auto list = GetStringList();
    if (!list)
        return std::nullopt;
    std::set<std::string> result;
    for (const auto& item : *list)
        result.insert(item);
    return result;
}

void StringValue::SetString(const std::string& value)
{
    _str = value;
}

void StringValue::SetInteger(int value)
{
    _str = std::to_string(value);
}

void StringValue::SetDouble(double value)
{
    _str = std::to_string(value);
}

void StringValue::SetBoolean(bool value)
{
    _str = value ? "true" : "false";
}

void StringValue::SetStringList(const std::vector<std::string>& value)
{
    std::string joined;
    for (std::size_t i = 0; i < value.size(); ++i)
    {
        if (i != 0)
            joined.push_back(',');
        joined += value[i];
    }
    _str = joined;
}

void StringValue::SetStringSet(const std::set<std::string>& value)
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
    _str = joined;
}

std::unique_ptr<Value> CreateStringValue(
    const std::string& name, const std::string& s)
{
    return std::make_unique<StringValue>(name, s);
}
