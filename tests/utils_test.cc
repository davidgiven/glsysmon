#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cmath>
#include <string>

#include "utils.h"

TEST_CASE("FormatBinary zero and small values")
{
    CHECK(FormatBinary(0) == "0 B");
    CHECK(FormatBinary(0.0) == "0 B");
    CHECK(FormatBinary(1) == "1 B");
    CHECK(FormatBinary(1.0) == "1 B");
    CHECK(FormatBinary(512) == "512 B");
    CHECK(FormatBinary(1023) == "1023 B");
    CHECK(FormatBinary(1023.0) == "1023 B");
    CHECK(FormatBinary(1023.9) == "1023.9 B");
}

TEST_CASE("FormatBinary exact power-of-two boundaries")
{
    CHECK(FormatBinary(1024) == "1 KiB");
    CHECK(FormatBinary(1024.0) == "1 KiB");
    CHECK(FormatBinary(1'048'576) == "1 MiB");
    CHECK(FormatBinary(1'073'741'824) == "1 GiB");
    CHECK(FormatBinary(1'099'511'627'776ULL) == "1 TiB");
    CHECK(FormatBinary(1'125'899'906'842'624ULL) == "1 PiB");
    CHECK(FormatBinary(1'152'921'504'606'846'976ULL) == "1 EiB");
}

TEST_CASE("FormatBinary fractional values")
{
    CHECK(FormatBinary(1536) == "1.5 KiB");
    CHECK(FormatBinary(1536.0) == "1.5 KiB");
    CHECK(FormatBinary(1'572'864) == "1.5 MiB");
    CHECK(FormatBinary(1.5 * 1024 * 1024 * 1024) == "1.5 GiB");
    CHECK(FormatBinary(2'621'440) == "2.5 MiB");
    CHECK(FormatBinary(5.5 * 1024) == "5.5 KiB");
}

TEST_CASE("FormatBinary uses most appropriate unit")
{
    CHECK((FormatBinary(1024 * 1024 - 1) == "1024 KiB" ||
           FormatBinary(1024 * 1024 - 1) == "1023.99 KiB" ||
           FormatBinary(1024 * 1024 - 1).find("KiB") != std::string::npos));
    CHECK(FormatBinary(1500) == "1.46 KiB");
    CHECK(FormatBinary(2048) == "2 KiB");
    CHECK(FormatBinary(10 * 1024) == "10 KiB");
    CHECK(FormatBinary(10 * 1024 * 1024) == "10 MiB");
}

TEST_CASE("FormatBinary negative values")
{
    CHECK(FormatBinary(-1024) == "-1 KiB");
    CHECK(FormatBinary(-1536) == "-1.5 KiB");
    CHECK(FormatBinary(-1) == "-1 B");
    CHECK(FormatBinary(-0.0) == "0 B");
    CHECK(FormatBinary(static_cast<std::int64_t>(-1024)) == "-1 KiB");
}

TEST_CASE("FormatBinary custom unit and suffix")
{
    CHECK(FormatBinary(1024, "B") == "1 KiB");
    CHECK(FormatBinary(1024, "B/s") == "1 KiB/s");
    CHECK(FormatBinary(2048, "B/s") == "2 KiB/s");
    CHECK(FormatBinary(1024, "") == "1 Ki");
    CHECK(FormatBinary(512, "") == "512");
    CHECK(FormatBinary(1536, "iB") == "1.5 KiiB");
    // With empty unit the prefix alone is used
    CHECK(FormatBinary(1024, "") == "1 Ki");
    CHECK(FormatBinary(1'048'576, "") == "1 Mi");
    CHECK(FormatBinary(1536, "") == "1.5 Ki");
}

TEST_CASE("FormatBinary precision control")
{
    CHECK((FormatBinary(1536, "B", 0) == "2 KiB" ||
           FormatBinary(1536, "B", 0) == "1 KiB"));
    CHECK(FormatBinary(1536, "B", 1) == "1.5 KiB");
    CHECK(FormatBinary(1536, "B", 2) == "1.5 KiB");
    CHECK(FormatBinary(1024, "B", 2) == "1 KiB");
    CHECK(FormatBinary(1500, "B", 1) == "1.5 KiB");
    CHECK(FormatBinary(1500, "B", 2) == "1.46 KiB");
    CHECK(FormatBinary(1234567, "B", 2) == "1.18 MiB");
    CHECK(FormatBinary(1234567, "B", 1) == "1.2 MiB");
}

TEST_CASE("FormatBinary integer overloads")
{
    CHECK(FormatBinary(static_cast<std::uint64_t>(0)) == "0 B");
    CHECK(FormatBinary(static_cast<std::uint64_t>(1024)) == "1 KiB");
    CHECK(FormatBinary(static_cast<std::uint64_t>(1536)) == "1.5 KiB");
    CHECK(FormatBinary(static_cast<std::int64_t>(2048)) == "2 KiB");
    CHECK(FormatBinary(static_cast<std::uint64_t>(1'048'576)) == "1 MiB");
    CHECK(FormatBinary(std::int64_t(-2048)) == "-2 KiB");
}

TEST_CASE("FormatBinary large values clamp at max prefix")
{
    double huge = std::pow(1024.0, 9) * 5;
    std::string s = FormatBinary(huge);
    CHECK((s.find("Yi") != std::string::npos ||
           s.find("Zi") != std::string::npos));
    CHECK_FALSE(s.empty());
}

TEST_CASE("FormatBinary trims trailing zeros")
{
    CHECK(FormatBinary(1024, "B", 2) == "1 KiB");
    CHECK(FormatBinary(2048, "B", 2) == "2 KiB");
    CHECK(FormatBinary(1280, "B", 2) == "1.25 KiB");
    CHECK(FormatBinary(1126.4, "B", 2) == "1.1 KiB");
}

TEST_CASE("Trim empty and no whitespace")
{
    CHECK(Trim("") == "");
    CHECK(Trim("hello") == "hello");
    CHECK(Trim("a") == "a");
}

TEST_CASE("Trim leading and trailing spaces")
{
    CHECK(Trim("  hello") == "hello");
    CHECK(Trim("hello  ") == "hello");
    CHECK(Trim("  hello  ") == "hello");
    CHECK(Trim("   hello   ") == "hello");
}

TEST_CASE("Trim only whitespace")
{
    CHECK(Trim("   ") == "");
    CHECK(Trim("\t") == "");
    CHECK(Trim("\n") == "");
    CHECK(Trim(" \t\n\r\f\v ") == "");
}

TEST_CASE("Trim mixed whitespace characters")
{
    CHECK(Trim("\thello") == "hello");
    CHECK(Trim("\nhello\n") == "hello");
    CHECK(Trim("\r\nhello\r\n") == "hello");
    CHECK(Trim("\t\n hello \n\t") == "hello");
    CHECK(Trim(" \t hello \t ") == "hello");
    CHECK(Trim("\f\vhello\f\v") == "hello");
}

TEST_CASE("Trim preserves internal whitespace")
{
    CHECK(Trim("  hello world  ") == "hello world");
    CHECK(Trim("\thello world\t") == "hello world");
    CHECK(Trim("  hello  world  ") == "hello  world");
    CHECK(Trim(" \n hello \t world \n ") == "hello \t world");
    CHECK(Trim("a b") == "a b");
    CHECK(Trim(" a b ") == "a b");
}

TEST_CASE("Trim single character with whitespace")
{
    CHECK(Trim(" a ") == "a");
    CHECK(Trim("  a  ") == "a");
    CHECK(Trim("\ta\t") == "a");
}
