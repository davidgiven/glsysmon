#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

#include "preferences/preferences.h"

namespace
{

    struct ScopedEnv
    {
        std::string key;
        std::string oldValue;
        bool hadOld = false;

        explicit ScopedEnv(const std::string& k, const std::string& newValue):
            key(k)
        {
            const char* old = std::getenv(k.c_str());
            if (old != nullptr)
            {
                hadOld = true;
                oldValue = old;
            }
            setenv(k.c_str(), newValue.c_str(), 1);
        }

        ~ScopedEnv()
        {
            if (hadOld)
                setenv(key.c_str(), oldValue.c_str(), 1);
            else
                unsetenv(key.c_str());
        }
    };

    std::string MakeTempDir()
    {
        char tmpl[] = "/tmp/glsysmon_toml_test_XXXXXX";
        char* dir = mkdtemp(tmpl);
        REQUIRE(dir != nullptr);
        return std::string(dir);
    }

    void WriteConfig(const std::string& baseDir, const std::string& content)
    {
        const std::filesystem::path dir =
            std::filesystem::path(baseDir) / "glsysmon";
        std::filesystem::create_directories(dir);
        std::ofstream out(dir / "config.toml");
        REQUIRE(out.is_open());
        out << content;
    }

} // namespace

TEST_CASE("Default preferences provide cpu.update_interval = 2")
{
    auto prefs = CreateDefaultPreferences();
    REQUIRE(prefs->GetInteger("cpu.update_interval").has_value());
    CHECK(prefs->GetInteger("cpu.update_interval").value() == 2);
}

TEST_CASE("Default preferences provide single fps for redraw = 30")
{
    auto prefs = CreateDefaultPreferences();
    REQUIRE(prefs->GetInteger("fps").has_value());
    CHECK(prefs->GetInteger("fps").value() == 30);
    CHECK_FALSE(prefs->GetInteger("redraw_fps").has_value());
    CHECK(GlobalPreferencesFetcher::GetFps(*prefs) == 30);
}

TEST_CASE(
    "TomlPreferences reads cpu.update_interval from [cpu] table via dotted key")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "[cpu]\nupdate_interval = 7\n");

    auto prefs = CreateTomlPreferences();
    REQUIRE(prefs->GetInteger("cpu.update_interval").has_value());
    CHECK(prefs->GetInteger("cpu.update_interval").value() == 7);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("TomlPreferences reads cpu.update_interval from dotted inline key")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "cpu.update_interval = 9\n");

    auto prefs = CreateTomlPreferences();
    REQUIRE(prefs->GetInteger("cpu.update_interval").has_value());
    CHECK(prefs->GetInteger("cpu.update_interval").value() == 9);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("TomlPreferences returns nullopt when cpu.update_interval missing")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "[cpu]\n# empty\n");

    auto prefs = CreateTomlPreferences();
    CHECK_FALSE(prefs->GetInteger("cpu.update_interval").has_value());

    std::filesystem::remove_all(tmp);
}

TEST_CASE(
    "TomlPreferences does not map top-level update_interval to "
    "cpu.update_interval")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "update_interval = 99\n");

    auto prefs = CreateTomlPreferences();
    CHECK_FALSE(prefs->GetInteger("cpu.update_interval").has_value());
    REQUIRE(prefs->GetInteger("update_interval").has_value());
    CHECK(prefs->GetInteger("update_interval").value() == 99);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("CombinedPreferences prefers TOML cpu.update_interval over default")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "[cpu]\nupdate_interval = 42\n");

    CliArgs args;
    auto prefs = CreatePreferences(args);
    REQUIRE(prefs->GetInteger("cpu.update_interval").has_value());
    CHECK(prefs->GetInteger("cpu.update_interval").value() == 42);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("CombinedPreferences falls back to default when TOML missing")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    // No config file at all – ensure directory exists but file does not
    std::filesystem::create_directories(
        std::filesystem::path(tmp) / "glsysmon");

    CliArgs args;
    auto prefs = CreatePreferences(args);
    REQUIRE(prefs->GetInteger("cpu.update_interval").has_value());
    CHECK(prefs->GetInteger("cpu.update_interval").value() == 2);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("TomlPreferences reads fps as single redraw fps")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "fps = 60\n");

    auto prefs = CreateTomlPreferences();
    REQUIRE(prefs->GetInteger("fps").has_value());
    CHECK(prefs->GetInteger("fps").value() == 60);
    CHECK_FALSE(prefs->GetInteger("redraw_fps").has_value());

    CliArgs args;
    auto combined = CreatePreferences(args);
    CHECK(combined->GetInteger("fps").value() == 60);
    CHECK(GlobalPreferencesFetcher::GetFps(*combined) == 60);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("CombinedPreferences falls back to default fps when TOML missing")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    std::filesystem::create_directories(
        std::filesystem::path(tmp) / "glsysmon");

    CliArgs args;
    auto prefs = CreatePreferences(args);
    REQUIRE(prefs->GetInteger("fps").has_value());
    CHECK(prefs->GetInteger("fps").value() == 30);
    CHECK(GlobalPreferencesFetcher::GetFps(*prefs) == 30);

    std::filesystem::remove_all(tmp);
}

TEST_CASE("MapPreferences string parsing and serialising for integers")
{
    auto prefs = CreateMapPreferences();

    // Missing key returns nullopt for all typed getters
    CHECK_FALSE(prefs->GetString("missing").has_value());
    CHECK_FALSE(prefs->GetInteger("missing").has_value());
    CHECK_FALSE(prefs->GetDouble("missing").has_value());
    CHECK_FALSE(prefs->GetBoolean("missing").has_value());
    CHECK_FALSE(prefs->GetStringList("missing").has_value());
    CHECK_FALSE(prefs->GetStringSet("missing").has_value());

    // Direct string storage
    prefs->SetString("s", "hello");
    REQUIRE(prefs->GetString("s").has_value());
    CHECK(prefs->GetString("s").value() == "hello");

    // Valid integers are parsed from strings
    prefs->SetString("i0", "0");
    prefs->SetString("i1", "42");
    prefs->SetString("i2", "-7");
    CHECK(prefs->GetInteger("i0").value() == 0);
    CHECK(prefs->GetInteger("i1").value() == 42);
    CHECK(prefs->GetInteger("i2").value() == -7);

    // Invalid integers return nullopt
    prefs->SetString("bad1", "");
    prefs->SetString("bad2", "abc");
    prefs->SetString("bad3", "12a");
    prefs->SetString("bad4", " 42");
    prefs->SetString("bad5", "42 ");
    CHECK_FALSE(prefs->GetInteger("bad1").has_value());
    CHECK_FALSE(prefs->GetInteger("bad2").has_value());
    CHECK_FALSE(prefs->GetInteger("bad3").has_value());
    CHECK_FALSE(prefs->GetInteger("bad4").has_value());
    CHECK_FALSE(prefs->GetInteger("bad5").has_value());

    // Serialising integers stores their decimal string form
    prefs->SetInteger("si", 123);
    REQUIRE(prefs->GetString("si").has_value());
    CHECK(prefs->GetString("si").value() == "123");
    CHECK(prefs->GetInteger("si").value() == 123);
    prefs->SetInteger("si_neg", -99);
    CHECK(prefs->GetString("si_neg").value() == "-99");
    CHECK(prefs->GetInteger("si_neg").value() == -99);
}

TEST_CASE("MapPreferences string parsing and serialising for doubles")
{
    auto prefs = CreateMapPreferences();

    prefs->SetString("d0", "0");
    prefs->SetString("d1", "3.14");
    prefs->SetString("d2", "-0.5");
    REQUIRE(prefs->GetDouble("d0").has_value());
    CHECK(prefs->GetDouble("d0").value() == doctest::Approx(0.0));
    CHECK(prefs->GetDouble("d1").value() == doctest::Approx(3.14));
    CHECK(prefs->GetDouble("d2").value() == doctest::Approx(-0.5));

    prefs->SetString("bad_d1", "abc");
    prefs->SetString("bad_d2", "12a");
    prefs->SetString("bad_d3", "");
    CHECK_FALSE(prefs->GetDouble("bad_d1").has_value());
    CHECK_FALSE(prefs->GetDouble("bad_d2").has_value());
    CHECK_FALSE(prefs->GetDouble("bad_d3").has_value());

    // Serialising doubles stores a string that parses back
    prefs->SetDouble("sd", 2.5);
    REQUIRE(prefs->GetString("sd").has_value());
    REQUIRE(prefs->GetDouble("sd").has_value());
    CHECK(prefs->GetDouble("sd").value() == doctest::Approx(2.5));
    prefs->SetDouble("sd2", -1.25);
    CHECK(prefs->GetDouble("sd2").value() == doctest::Approx(-1.25));
}

TEST_CASE("MapPreferences string parsing and serialising for booleans")
{
    auto prefs = CreateMapPreferences();

    // true values
    prefs->SetString("b_true", "true");
    prefs->SetString("b_one", "1");
    CHECK(prefs->GetBoolean("b_true").value() == true);
    CHECK(prefs->GetBoolean("b_one").value() == true);

    // false values
    prefs->SetString("b_false", "false");
    prefs->SetString("b_zero", "0");
    CHECK(prefs->GetBoolean("b_false").value() == false);
    CHECK(prefs->GetBoolean("b_zero").value() == false);

    // Invalid booleans return nullopt – case sensitive and no other strings
    prefs->SetString("bad_b1", "True");
    prefs->SetString("bad_b2", "FALSE");
    prefs->SetString("bad_b3", "yes");
    prefs->SetString("bad_b4", "2");
    prefs->SetString("bad_b5", "");
    prefs->SetString("bad_b6", " true");
    CHECK_FALSE(prefs->GetBoolean("bad_b1").has_value());
    CHECK_FALSE(prefs->GetBoolean("bad_b2").has_value());
    CHECK_FALSE(prefs->GetBoolean("bad_b3").has_value());
    CHECK_FALSE(prefs->GetBoolean("bad_b4").has_value());
    CHECK_FALSE(prefs->GetBoolean("bad_b5").has_value());
    CHECK_FALSE(prefs->GetBoolean("bad_b6").has_value());

    // Serialising booleans stores "true"/"false" strings
    prefs->SetBoolean("sb_true", true);
    prefs->SetBoolean("sb_false", false);
    REQUIRE(prefs->GetString("sb_true").has_value());
    REQUIRE(prefs->GetString("sb_false").has_value());
    CHECK(prefs->GetString("sb_true").value() == "true");
    CHECK(prefs->GetString("sb_false").value() == "false");
    CHECK(prefs->GetBoolean("sb_true").value() == true);
    CHECK(prefs->GetBoolean("sb_false").value() == false);
}

TEST_CASE(
    "MapPreferences string parsing and serialising for string lists and sets")
{
    auto prefs = CreateMapPreferences();

    // Comma-separated lists are parsed
    prefs->SetString("list1", "a,b,c");
    REQUIRE(prefs->GetStringList("list1").has_value());
    CHECK(prefs->GetStringList("list1").value() ==
          std::vector<std::string>{"a", "b", "c"});

    prefs->SetString("list2", "single");
    CHECK(prefs->GetStringList("list2").value() ==
          std::vector<std::string>{"single"});

    // Empty entries are preserved
    prefs->SetString("list3", "a,,b");
    CHECK(prefs->GetStringList("list3").value() ==
          std::vector<std::string>{"a", "", "b"});

    // Empty string yields a single empty element
    prefs->SetString("list_empty", "");
    CHECK(prefs->GetStringList("list_empty").value() ==
          std::vector<std::string>{""});

    // Serialising lists joins with commas
    prefs->SetStringList("sl", {"x", "y", "z"});
    CHECK(prefs->GetString("sl").value() == "x,y,z");
    CHECK(prefs->GetStringList("sl").value() ==
          std::vector<std::string>{"x", "y", "z"});

    prefs->SetStringList("sl_empty", {});
    CHECK(prefs->GetString("sl_empty").value() == "");
    // Reading back an empty stored string gives one empty element per parser
    CHECK(prefs->GetStringList("sl_empty").value() ==
          std::vector<std::string>{""});

    // Sets serialise in sorted order and parse back
    prefs->SetStringSet("ss", {"b", "a", "c"});
    CHECK(prefs->GetString("ss").value() == "a,b,c");
    REQUIRE(prefs->GetStringSet("ss").has_value());
    CHECK(prefs->GetStringSet("ss").value() ==
          std::set<std::string>{"a", "b", "c"});
    // GetStringSet via comma parsing deduplicates
    prefs->SetString("dup", "a,b,a");
    CHECK(
        prefs->GetStringSet("dup").value() == std::set<std::string>{"a", "b"});
}

TEST_CASE("CliPreferences string parsing for booleans and other types")
{
    CliArgs args;
    args.values = {"--flag_true=true",
        "--flag_one=1",
        "--flag_false=false",
        "--flag_zero=0",
        "--my_int=42",
        "--my_double=3.14",
        "--my_list=a,b"};
    auto prefs = CreateCliPreferences(args);

    CHECK(prefs->GetBoolean("flag_true").value() == true);
    CHECK(prefs->GetBoolean("flag_one").value() == true);
    CHECK(prefs->GetBoolean("flag_false").value() == false);
    CHECK(prefs->GetBoolean("flag_zero").value() == false);
    CHECK(prefs->GetInteger("my_int").value() == 42);
    CHECK(prefs->GetDouble("my_double").value() == doctest::Approx(3.14));
    CHECK(prefs->GetStringList("my_list").value() ==
          std::vector<std::string>{"a", "b"});
    // Generic --key=value fallback stores string verbatim
    CHECK(prefs->GetString("flag_true").value() == "true");
    CHECK(prefs->GetString("flag_one").value() == "1");
}

TEST_CASE("TomlPreferences string serialisation of native types")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp,
        "str_val = \"hello\"\n"
        "bool_true = true\n"
        "bool_false = false\n"
        "int_val = 42\n"
        "float_val = 3.14\n"
        "str_true = \"true\"\n"
        "str_one = \"1\"\n"
        "int_one = 1\n"
        "int_zero = 0\n");
    auto prefs = CreateTomlPreferences();

    // GetString converts native types to their string form
    CHECK(prefs->GetString("str_val").value() == "hello");
    CHECK(prefs->GetString("bool_true").value() == "true");
    CHECK(prefs->GetString("bool_false").value() == "false");
    CHECK(prefs->GetString("int_val").value() == "42");
    REQUIRE(prefs->GetString("float_val").has_value());
    CHECK(prefs->GetDouble("float_val").value() == doctest::Approx(3.14));

    // GetBoolean handles bool, integer 1/0 and string "true"/"1"/"false"/"0"
    CHECK(prefs->GetBoolean("bool_true").value() == true);
    CHECK(prefs->GetBoolean("bool_false").value() == false);
    CHECK(prefs->GetBoolean("str_true").value() == true);
    CHECK(prefs->GetBoolean("str_one").value() == true);
    CHECK(prefs->GetBoolean("int_one").value() == true);
    CHECK(prefs->GetBoolean("int_zero").value() == false);
    CHECK_FALSE(prefs->GetBoolean("str_val").has_value());
    CHECK_FALSE(prefs->GetBoolean("int_val").has_value());

    // Integer and double getters are typed
    CHECK(prefs->GetInteger("int_val").value() == 42);
    CHECK_FALSE(prefs->GetInteger("bool_true").has_value());
    CHECK_FALSE(prefs->GetInteger("str_val").has_value());

    std::filesystem::remove_all(tmp);
}

TEST_CASE("CombinedPreferences delegates string serialisation to first source")
{
    auto map1 = CreateMapPreferences();
    auto map2 = CreateMapPreferences();
    map2->SetString("key", "from_map2");
    map2->SetInteger("int_key", 99);

    // GetString prefers first source that has the key
    auto combined = CreateCombinedPreferences({map1, map2});
    CHECK(combined->GetString("key").value() == "from_map2");
    CHECK(combined->GetInteger("int_key").value() == 99);

    // Set* delegates to the first source
    combined->SetString("new_key", "new_val");
    CHECK(map1->GetString("new_key").value() == "new_val");
    CHECK_FALSE(map2->GetString("new_key").has_value());
    // Get now prefers the first source after write
    CHECK(combined->GetString("new_key").value() == "new_val");

    combined->SetInteger("new_int", 7);
    CHECK(map1->GetString("new_int").value() == "7");
    CHECK(combined->GetInteger("new_int").value() == 7);

    combined->SetBoolean("new_bool", true);
    CHECK(map1->GetString("new_bool").value() == "true");
    CHECK(combined->GetBoolean("new_bool").value() == true);

    combined->SetDouble("new_double", 1.5);
    REQUIRE(combined->GetDouble("new_double").has_value());
    CHECK(combined->GetDouble("new_double").value() == doctest::Approx(1.5));

    combined->SetStringList("new_list", {"a", "b"});
    CHECK(map1->GetString("new_list").value() == "a,b");
    CHECK(combined->GetStringList("new_list").value() ==
          std::vector<std::string>{"a", "b"});
}
