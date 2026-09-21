#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

#include "components.h"
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

TEST_CASE("Default preferences provide cpu.update_interval = 1")
{
    auto prefs = CreateDefaultPreferences();
    REQUIRE(prefs->GetInteger("cpu.update_interval").has_value());
    CHECK(prefs->GetInteger("cpu.update_interval").value() == 1);
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
    CHECK(prefs->GetInteger("cpu.update_interval").value() == 1);

    std::filesystem::remove_all(tmp);
}
