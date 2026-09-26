#include "utils.h"

#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace
{

    std::string FormatWithPrecision(double scaled, int precision)
    {
        if (precision < 0)
            precision = 0;
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(precision) << scaled;
        std::string s = oss.str();
        if (precision > 0)
        {
            while (!s.empty() && s.back() == '0')
                s.pop_back();
            if (!s.empty() && s.back() == '.')
                s.pop_back();
        }
        if (s == "-0")
            s = "0";
        return s;
    }

} // namespace

std::string FormatBinary(double value, const std::string& unit, int precision)
{
    static constexpr const char* kPrefixes[] = {
        "", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei", "Zi", "Yi"};
    static constexpr std::size_t kPrefixCount =
        sizeof(kPrefixes) / sizeof(kPrefixes[0]);

    if (std::isnan(value) || std::isinf(value))
        return FormatWithPrecision(value, precision) + " " + unit;

    double absValue = std::fabs(value);
    std::size_t idx = 0;
    double scaledAbs = absValue;
    while (scaledAbs >= 1024.0 && idx + 1 < kPrefixCount)
    {
        scaledAbs /= 1024.0;
        ++idx;
    }

    double divisor = 1.0;
    for (std::size_t i = 0; i < idx; ++i)
        divisor *= 1024.0;
    double scaled = (divisor == 0.0) ? value : value / divisor;

    std::string number = FormatWithPrecision(scaled, precision);

    std::string prefix = kPrefixes[idx];
    std::string suffix;
    if (prefix.empty())
        suffix = unit;
    else
        suffix = prefix + unit;

    if (suffix.empty())
        return number;
    return number + " " + suffix;
}

std::string FormatBinary(
    std::int64_t value, const std::string& unit, int precision)
{
    return FormatBinary(static_cast<double>(value), unit, precision);
}

std::string FormatBinary(
    std::uint64_t value, const std::string& unit, int precision)
{
    return FormatBinary(static_cast<double>(value), unit, precision);
}

std::string Trim(const std::string& s)
{
    std::size_t start = 0;
    while (
        start < s.size() && std::isspace(static_cast<unsigned char>(s[start])))
        start++;
    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1])))
        end--;
    return s.substr(start, end - start);
}
