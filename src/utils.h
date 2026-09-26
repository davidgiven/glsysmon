#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

extern std::string FormatBinary(
    double value, const std::string& unit = "B", int precision = 2);

extern std::string FormatBinary(
    std::int64_t value, const std::string& unit = "B", int precision = 2);

extern std::string FormatBinary(
    std::uint64_t value, const std::string& unit = "B", int precision = 2);

extern std::string Trim(const std::string& s);

template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::string FormatBinary(
    T value, const std::string& unit = "B", int precision = 2)
{
    return FormatBinary(static_cast<double>(value), unit, precision);
}
