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

TEST_CASE("TomlPreferences reads string list from TOML array")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "views = [\"A\", \"B\", \"C\"]\n");

    auto prefs = CreateTomlPreferences();
    REQUIRE(prefs->GetStringList("views").has_value());
    CHECK(prefs->GetStringList("views").value() ==
          std::vector<std::string>{"A", "B", "C"});
    CHECK_FALSE(prefs->GetString("views").has_value());
    CHECK_FALSE(prefs->GetInteger("views").has_value());

    std::filesystem::remove_all(tmp);
}

TEST_CASE("TomlPreferences reads string set deduplicates and sorts")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "views = [\"b\", \"a\", \"b\", \"c\"]\n");

    auto prefs = CreateTomlPreferences();
    REQUIRE(prefs->GetStringSet("views").has_value());
    CHECK(prefs->GetStringSet("views").value() ==
          std::set<std::string>{"a", "b", "c"});
    // GetStringList preserves duplicates and order
    REQUIRE(prefs->GetStringList("views").has_value());
    CHECK(prefs->GetStringList("views").value() ==
          std::vector<std::string>{"b", "a", "b", "c"});

    std::filesystem::remove_all(tmp);
}

TEST_CASE("TomlPreferences returns nullopt for missing list and set")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "[cpu]\nupdate_interval = 2\n");

    auto prefs = CreateTomlPreferences();
    CHECK_FALSE(prefs->GetStringList("views").has_value());
    CHECK_FALSE(prefs->GetStringSet("views").has_value());
    CHECK_FALSE(prefs->GetStringList("missing").has_value());
    CHECK_FALSE(prefs->GetStringSet("missing").has_value());

    std::filesystem::remove_all(tmp);
}

TEST_CASE("TomlPreferences handles temperature.sensors as list and set")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "temperature.sensors = [\"CPU\", \"GPU\"]\n");

    auto prefs = CreateTomlPreferences();
    REQUIRE(prefs->GetStringList("temperature.sensors").has_value());
    CHECK(prefs->GetStringList("temperature.sensors").value() ==
          std::vector<std::string>{"CPU", "GPU"});
    REQUIRE(prefs->GetStringSet("temperature.sensors").has_value());
    CHECK(prefs->GetStringSet("temperature.sensors").value() ==
          std::set<std::string>{"CPU", "GPU"});

    std::filesystem::remove_all(tmp);
}

TEST_CASE("CombinedPreferences prefers TOML list and set over default")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    WriteConfig(tmp, "views = [\"A\", \"B\", \"A\"]\n");

    CliArgs args;
    auto prefs = CreatePreferences(args);
    REQUIRE(prefs->GetStringList("views").has_value());
    CHECK(prefs->GetStringList("views").value() ==
          std::vector<std::string>{"A", "B", "A"});
    REQUIRE(prefs->GetStringSet("views").has_value());
    CHECK(prefs->GetStringSet("views").value() ==
          std::set<std::string>{"A", "B"});
    // Still a set, deduplicated
    CHECK(prefs->GetStringSet("views").value().size() == 2);

    std::filesystem::remove_all(tmp);
}

TEST_CASE(
    "CombinedPreferences falls back to default list and set when TOML missing")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    std::filesystem::create_directories(
        std::filesystem::path(tmp) / "glsysmon");

    CliArgs args;
    auto prefs = CreatePreferences(args);
    REQUIRE(prefs->GetStringList("views").has_value());
    CHECK(prefs->GetStringList("views").value() ==
          std::vector<std::string>{
              "HostnameView", "ClockView", "CpuView", "TemperatureView"});
    REQUIRE(prefs->GetStringSet("views").has_value());
    CHECK(prefs->GetStringSet("views").value() ==
          std::set<std::string>{
              "ClockView", "CpuView", "HostnameView", "TemperatureView"});

    std::filesystem::remove_all(tmp);
}

TEST_CASE("Value Get is source of truth and Type reflects stored type")
{
    // String-backed Value (Map) infers Type from string content
    auto map = CreateMapPreferences();
    map->SetString("plain", "hello");
    map->SetInteger("int_val", 42);
    map->SetDouble("dbl_val", 3.14);
    map->SetBoolean("b_true", true);
    map->SetStringList("list_val", {"a", "b"});

    // Missing key: Get returns nullptr, all typed getters via alias return
    // nullopt
    CHECK_FALSE(map->Get("missing"));
    CHECK_FALSE(map->GetString("missing").has_value());
    CHECK_FALSE(map->GetStringList("missing").has_value());
    CHECK_FALSE(map->GetStringSet("missing").has_value());

    // Existing key: Get returns Value whose methods are aliases
    {
        auto v = map->Get("plain");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::String);
        CHECK(v->GetString().value() == "hello");
        CHECK(v->GetStringList().value() == std::vector<std::string>{"hello"});
        // Alias via Preferences
        CHECK(map->GetString("plain").value() == v->GetString().value());
    }
    {
        auto v = map->Get("int_val");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::Integer);
        CHECK(v->GetInteger().value() == 42);
        CHECK(v->GetString().value() == "42");
        CHECK(map->GetInteger("int_val").value() == v->GetInteger().value());
    }
    {
        auto v = map->Get("dbl_val");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::Double);
        CHECK(v->GetDouble().value() == doctest::Approx(3.14));
    }
    {
        auto v = map->Get("b_true");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::Boolean);
        CHECK(v->GetBoolean().value() == true);
    }
    {
        auto v = map->Get("list_val");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::StringList);
        CHECK(v->GetStringList().value() == std::vector<std::string>{"a", "b"});
        CHECK(v->GetStringSet().value() == std::set<std::string>{"a", "b"});
        // Alias
        CHECK(map->GetStringList("list_val").value() ==
              v->GetStringList().value());
        CHECK(
            map->GetStringSet("list_val").value() == v->GetStringSet().value());
    }

    // Toml native types via Value
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);
    WriteConfig(tmp,
        "str_val = \"hello\"\n"
        "int_val = 42\n"
        "dbl_val = 3.14\n"
        "b_true = true\n"
        "list_val = [\"a\", \"b\", \"a\"]\n");
    auto toml = CreateTomlPreferences();
    {
        auto v = toml->Get("str_val");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::String);
        CHECK(v->GetString().value() == "hello");
        CHECK_FALSE(v->GetStringList().has_value());
    }
    {
        auto v = toml->Get("int_val");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::Integer);
        CHECK(v->GetInteger().value() == 42);
    }
    {
        auto v = toml->Get("dbl_val");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::Double);
        CHECK(v->GetDouble().value() == doctest::Approx(3.14));
    }
    {
        auto v = toml->Get("b_true");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::Boolean);
        CHECK(v->GetBoolean().value() == true);
    }
    {
        auto v = toml->Get("list_val");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::StringList);
        CHECK(v->GetStringList().value() ==
              std::vector<std::string>{"a", "b", "a"});
        CHECK(v->GetStringSet().value() == std::set<std::string>{"a", "b"});
        // Alias via Preferences must match Value
        CHECK(toml->GetStringList("list_val").value() ==
              v->GetStringList().value());
        CHECK(toml->GetStringSet("list_val").value() ==
              v->GetStringSet().value());
    }
    // Missing via Value
    CHECK_FALSE(toml->Get("missing"));
    CHECK_FALSE(toml->GetString("missing").has_value());

    // Combined delegates Get source of truth
    CliArgs args;
    auto combined = CreatePreferences(args);
    // Should prefer TOML list
    {
        auto v = combined->Get("list_val");
        REQUIRE(v);
        CHECK(v->GetType() == Value::Type::StringList);
        CHECK(v->GetStringList().value() ==
              std::vector<std::string>{"a", "b", "a"});
        CHECK(v->GetStringSet().value() == std::set<std::string>{"a", "b"});
    }
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

TEST_CASE("Preferences::ClearAll default throws unsupported operation")
{
    CliArgs args;
    auto cli = CreateCliPreferences(args);
    CHECK_THROWS_AS(cli->ClearAll(), std::runtime_error);
    CHECK_THROWS_WITH(cli->ClearAll(), "unsupported operation");

    auto toml = CreateTomlPreferences();
    CHECK_THROWS_AS(toml->ClearAll(), std::runtime_error);

    auto def = CreateDefaultPreferences();
    CHECK_THROWS_AS(def->ClearAll(), std::runtime_error);
    CHECK_THROWS_WITH(def->ClearAll(), "unsupported operation");
}

TEST_CASE("MapPreferences::ClearAll clears the underlying map")
{
    auto prefs = CreateMapPreferences();
    prefs->SetString("a", "1");
    prefs->SetString("b", "2");
    prefs->SetInteger("c", 3);
    REQUIRE(prefs->GetAll().size() == 3);
    prefs->ClearAll();
    CHECK(prefs->GetAll().empty());
    CHECK_FALSE(prefs->GetString("a").has_value());
    CHECK_FALSE(prefs->GetString("b").has_value());
    CHECK_FALSE(prefs->GetInteger("c").has_value());
}

TEST_CASE("CombinedPreferences::ClearAll calls ClearAll on the first child")
{
    auto map1 = CreateMapPreferences();
    auto map2 = CreateMapPreferences();
    map1->SetString("k1", "v1");
    map2->SetString("k2", "v2");
    auto combined = CreateCombinedPreferences({map1, map2});
    REQUIRE(combined->GetString("k1").has_value());
    REQUIRE(combined->GetString("k2").has_value());
    combined->ClearAll();
    CHECK_FALSE(map1->GetString("k1").has_value());
    CHECK(map1->GetAll().empty());
    CHECK(map2->GetString("k2").value() == "v2");
    CHECK_FALSE(combined->GetString("k1").has_value());
    CHECK(combined->GetString("k2").value() == "v2");
}

TEST_CASE("CombinedPreferences::ClearAll on empty sources does not throw")
{
    auto combined = CreateCombinedPreferences({});
    CHECK_NOTHROW(combined->ClearAll());
}

TEST_CASE("WriteTomlPreferences roundtrips typed values and preserves types")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    auto prefs = CreateMapPreferences();
    prefs->SetString("side", "right");
    prefs->SetInteger("size", 123);
    prefs->SetInteger("monitor", 2);
    prefs->SetDouble("fps", 60.5);
    prefs->SetBoolean("flag_true", true);
    prefs->SetBoolean("flag_false", false);
    prefs->SetString("plain", "hello");
    prefs->SetDouble("plain_double", 3.14);
    prefs->SetInteger("plain_int", 42);
    prefs->SetStringList("views", {"HostnameView", "CpuView"});
    prefs->SetStringList("temperature.sensors", {"CPU", "GPU"});

    WriteTomlPreferences(*prefs);

    auto loaded = CreateTomlPreferences();

    // String
    REQUIRE(loaded->GetString("plain").has_value());
    CHECK(loaded->GetString("plain").value() == "hello");
    CHECK(loaded->GetString("side").value() == "right");

    // Integer preserved as integer, not string/bool
    REQUIRE(loaded->GetInteger("size").has_value());
    CHECK(loaded->GetInteger("size").value() == 123);
    CHECK(loaded->GetInteger("monitor").value() == 2);
    REQUIRE(loaded->GetInteger("plain_int").has_value());
    CHECK(loaded->GetInteger("plain_int").value() == 42);
    CHECK_FALSE(loaded->GetInteger("flag_true").has_value());
    CHECK_FALSE(loaded->GetInteger("plain").has_value());

    // Double preserved as floating-point
    REQUIRE(loaded->GetDouble("fps").has_value());
    CHECK(loaded->GetDouble("fps").value() == doctest::Approx(60.5));
    REQUIRE(loaded->GetDouble("plain_double").has_value());
    CHECK(loaded->GetDouble("plain_double").value() == doctest::Approx(3.14));
    CHECK_FALSE(loaded->GetInteger("plain_double").has_value());
    // Integer also readable as double
    CHECK(loaded->GetDouble("plain_int").value() == doctest::Approx(42));

    // Boolean preserved as bool, not integer
    REQUIRE(loaded->GetBoolean("flag_true").has_value());
    CHECK(loaded->GetBoolean("flag_true").value() == true);
    REQUIRE(loaded->GetBoolean("flag_false").has_value());
    CHECK(loaded->GetBoolean("flag_false").value() == false);
    CHECK_FALSE(loaded->GetBoolean("plain").has_value());
    CHECK_FALSE(loaded->GetBoolean("plain_int").has_value());

    // String list preserved as TOML array
    REQUIRE(loaded->GetStringList("views").has_value());
    CHECK(loaded->GetStringList("views").value() ==
          std::vector<std::string>{"HostnameView", "CpuView"});
    REQUIRE(loaded->GetStringList("temperature.sensors").has_value());
    CHECK(loaded->GetStringList("temperature.sensors").value() ==
          std::vector<std::string>{"CPU", "GPU"});
    // String values are not misinterpreted as lists
    CHECK_FALSE(loaded->GetStringList("plain").has_value());

    // Const overload also works
    const Preferences& constPrefs = *prefs;
    WriteTomlPreferences(constPrefs);
    auto reloaded = CreateTomlPreferences();
    CHECK(reloaded->GetString("plain").value() == "hello");

    std::filesystem::remove_all(tmp);
}

TEST_CASE("WriteTomlPreferences reconstructs TOML groups from dotted paths")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    auto prefs = CreateMapPreferences();
    prefs->SetInteger("cpu.update_interval", 7);
    prefs->SetDouble("temperature.maximum", 80.5);
    prefs->SetInteger("temperature.minimum", 20);
    prefs->SetString("a.b.c", "hello");
    prefs->SetStringList("a.b.list", {"x", "y"});
    prefs->SetString("plain", "value");
    prefs->SetStringList("views", {"A", "B"});

    WriteTomlPreferences(*prefs);

    const std::filesystem::path file =
        std::filesystem::path(tmp) / "glsysmon" / "config.toml";
    REQUIRE(std::filesystem::exists(file));
    std::ifstream in(file);
    REQUIRE(in.is_open());
    const std::string content(
        (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    // Groups are reconstructed
    CHECK(content.find("[cpu]") != std::string::npos);
    CHECK(content.find("[temperature]") != std::string::npos);
    CHECK(content.find("[a.b]") != std::string::npos);
    // Dotted keys are not left as inline dotted keys
    CHECK(content.find("cpu.update_interval") == std::string::npos);
    CHECK(content.find("temperature.maximum") == std::string::npos);
    CHECK(content.find("a.b.c") == std::string::npos);

    // Values are readable via dotted keys
    auto loaded = CreateTomlPreferences();
    CHECK(loaded->GetInteger("cpu.update_interval").value() == 7);
    CHECK(loaded->GetDouble("temperature.maximum").value() ==
          doctest::Approx(80.5));
    CHECK(loaded->GetInteger("temperature.minimum").value() == 20);
    CHECK(loaded->GetString("a.b.c").value() == "hello");
    CHECK(loaded->GetStringList("a.b.list").value() ==
          std::vector<std::string>{"x", "y"});
    CHECK(loaded->GetString("plain").value() == "value");
    CHECK(loaded->GetStringList("views").value() ==
          std::vector<std::string>{"A", "B"});

    // Overload taking non-const Preferences& also works
    auto prefs2 = CreateMapPreferences();
    prefs2->SetString("extra", "1");
    WriteTomlPreferences(*prefs2);
    auto loaded2 = CreateTomlPreferences();
    CHECK(loaded2->GetString("extra").value() == "1");

    std::filesystem::remove_all(tmp);
}

TEST_CASE("WriteTomlPreferences writes to DefaultConfigPath and creates dirs")
{
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);

    // Ensure glsysmon subdir does not exist before write
    const std::filesystem::path dir = std::filesystem::path(tmp) / "glsysmon";
    std::filesystem::remove_all(dir);
    REQUIRE_FALSE(std::filesystem::exists(dir));

    auto prefs = CreateMapPreferences();
    prefs->SetString("side", "left");
    WriteTomlPreferences(*prefs);

    const std::filesystem::path file = dir / "config.toml";
    CHECK(std::filesystem::exists(file));

    // Overwrite with new value
    prefs->SetString("side", "right");
    WriteTomlPreferences(*prefs);
    auto loaded = CreateTomlPreferences();
    CHECK(loaded->GetString("side").value() == "right");

    std::filesystem::remove_all(tmp);
}

TEST_CASE("CombinedPreferences::GetAll enumerates overridden keys exactly once")
{
    auto map1 = CreateMapPreferences();
    auto map2 = CreateMapPreferences();

    map1->SetString("a", "1");
    map1->SetString("b", "override");
    map2->SetString("b", "original");
    map2->SetString("c", "3");

    auto combined = CreateCombinedPreferences({map1, map2});
    const std::set<std::string> all = combined->GetAll();

    CHECK(all.size() == 3);
    CHECK(all.count("a") == 1);
    CHECK(all.count("b") == 1);
    CHECK(all.count("c") == 1);
    CHECK(combined->GetString("a").value() == "1");
    CHECK(combined->GetString("b").value() == "override");
    CHECK(combined->GetString("c").value() == "3");
    CHECK(combined->GetString("b").value() != "original");

    // Writing the combined preferences must persist the overridden value
    const std::string tmp = MakeTempDir();
    ScopedEnv env("XDG_CONFIG_HOME", tmp);
    WriteTomlPreferences(*combined);
    auto loaded = CreateTomlPreferences();
    CHECK(loaded->GetString("a").value() == "1");
    CHECK(loaded->GetString("b").value() == "override");
    CHECK(loaded->GetString("c").value() == "3");
    std::filesystem::remove_all(tmp);
}
