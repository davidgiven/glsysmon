#pragma once

#include <optional>
#include <set>
#include <string>
#include <vector>

#include "preferences.h"

class StringValue : public Value
{
public:
    StringValue(std::string name, std::string s);

    std::string GetName() const override;
    Type GetType() const override;
    std::optional<std::string> GetString() const override;
    std::optional<int> GetInteger() const override;
    std::optional<double> GetDouble() const override;
    std::optional<bool> GetBoolean() const override;
    std::optional<std::vector<std::string>> GetStringList() const override;
    std::optional<std::set<std::string>> GetStringSet() const override;

private:
    std::string _name;
    std::string _str;
};
