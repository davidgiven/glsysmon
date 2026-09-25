#include "preferences.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <memory>
#include <set>
#include <string>
#include <toml++/toml.h>

namespace
{

    std::string DefaultConfigPath()
    {
        const char* xdg = std::getenv("XDG_CONFIG_HOME");
        if (xdg != nullptr && *xdg != '\0')
            return std::string(xdg) + "/glsysmon/config.toml";
        const char* home = std::getenv("HOME");
        if (home != nullptr && *home != '\0')
            return std::string(home) + "/.config/glsysmon/config.toml";
        return "glsysmon.toml";
    }

    template <typename T>
    void InsertDotted(toml::table& root, const std::string& dotted, T&& value)
    {
        std::size_t start = 0;
        toml::table* cur = &root;
        while (true)
        {
            const std::size_t dot = dotted.find('.', start);
            if (dot == std::string::npos)
            {
                const std::string last = dotted.substr(start);
                cur->insert_or_assign(last, std::forward<T>(value));
                break;
            }
            const std::string part = dotted.substr(start, dot - start);
            toml::node* node = cur->get(part);
            if (node == nullptr || !node->is_table())
            {
                cur->insert_or_assign(part, toml::table{});
                node = cur->get(part);
            }
            cur = node->as_table();
            if (cur == nullptr)
                break;
            start = dot + 1;
        }
    }

    class TomlValue : public Value
    {
    public:
        TomlValue(toml::table* table, std::string key):
            _table(table),
            _key(std::move(key))
        {
        }
        TomlValue(const toml::table* table, std::string key):
            _table(const_cast<toml::table*>(table)),
            _key(std::move(key))
        {
        }

        std::string GetName() const override
        {
            return _key;
        }

        Type GetType() const override
        {
            const auto node = _table->at_path(_key);
            if (node.as_array() != nullptr)
                return Type::StringList;
            if (node.as_boolean() != nullptr)
                return Type::Boolean;
            if (node.as_integer() != nullptr)
                return Type::Integer;
            if (node.as_floating_point() != nullptr)
                return Type::Double;
            if (node.as_string() != nullptr)
                return Type::String;
            return Type::String;
        }

        std::optional<std::string> GetString() const override
        {
            if (auto value = _table->at_path(_key).value<std::string>())
                return value;
            if (auto node = _table->at_path(_key).as_boolean())
                return node->get() ? "true" : "false";
            if (auto node = _table->at_path(_key).as_integer())
                return std::to_string(node->get());
            if (auto node = _table->at_path(_key).as_floating_point())
                return std::to_string(node->get());
            return std::nullopt;
        }

        std::optional<int> GetInteger() const override
        {
            if (auto node = _table->at_path(_key).as_integer())
                return static_cast<int>(node->get());
            return std::nullopt;
        }

        std::optional<double> GetDouble() const override
        {
            if (auto node = _table->at_path(_key).as_floating_point())
                return node->get();
            if (auto node = _table->at_path(_key).as_integer())
                return static_cast<double>(node->get());
            return std::nullopt;
        }

        std::optional<bool> GetBoolean() const override
        {
            if (auto node = _table->at_path(_key).as_boolean())
                return node->get();
            if (auto node = _table->at_path(_key).as_integer())
            {
                const int64_t v = node->get();
                if (v == 1)
                    return true;
                if (v == 0)
                    return false;
                return std::nullopt;
            }
            if (auto value = _table->at_path(_key).value<std::string>())
            {
                if (*value == "true" || *value == "1")
                    return true;
                if (*value == "false" || *value == "0")
                    return false;
                return std::nullopt;
            }
            return std::nullopt;
        }

        std::optional<std::vector<std::string>> GetStringList() const override
        {
            const toml::array* array = _table->at_path(_key).as_array();
            if (array == nullptr)
                return std::nullopt;
            std::vector<std::string> result;
            for (const toml::node& item : *array)
            {
                const toml::value<std::string>* value = item.as_string();
                if (value != nullptr)
                    result.push_back(value->get());
            }
            return result;
        }

        std::optional<std::set<std::string>> GetStringSet() const override
        {
            const toml::array* array = _table->at_path(_key).as_array();
            if (array == nullptr)
                return std::nullopt;
            std::set<std::string> result;
            for (const toml::node& item : *array)
            {
                const toml::value<std::string>* value = item.as_string();
                if (value != nullptr)
                    result.insert(value->get());
            }
            return result;
        }

        void SetString(const std::string& value) override
        {
            InsertDotted(*_table, _key, value);
        }

        void SetInteger(int value) override
        {
            InsertDotted(*_table, _key, static_cast<int64_t>(value));
        }

        void SetDouble(double value) override
        {
            InsertDotted(*_table, _key, value);
        }

        void SetBoolean(bool value) override
        {
            InsertDotted(*_table, _key, value);
        }

        void SetStringList(const std::vector<std::string>& value) override
        {
            toml::array arr;
            for (const auto& s : value)
                arr.emplace_back(s);
            InsertDotted(*_table, _key, std::move(arr));
        }

        void SetStringSet(const std::set<std::string>& value) override
        {
            toml::array arr;
            for (const auto& s : value)
                arr.emplace_back(s);
            InsertDotted(*_table, _key, std::move(arr));
        }

    private:
        toml::table* _table;
        std::string _key;
    };

    class TomlPreferencesImpl : public Preferences
    {
    public:
        TomlPreferencesImpl()
        {
            const std::string path = DefaultConfigPath();
            std::error_code ec;
            if (std::filesystem::exists(path, ec))
            {
                try
                {
                    _table = toml::parse_file(path);
                }
                catch (const toml::parse_error&)
                {
                    _table = toml::table{};
                }
            }
        }

        std::unique_ptr<Value> Get(const std::string& key) const override
        {
            auto node = _table.at_path(key);
            if (!node)
                return nullptr;
            if (node.is_table())
                return nullptr;
            return std::make_unique<TomlValue>(&_table, key);
        }

        std::unique_ptr<Value> Add(const std::string& key) override
        {
            return std::make_unique<TomlValue>(&_table, key);
        }

        std::set<std::unique_ptr<Value>> GetAll() const override
        {
            std::set<std::string> keys;
            CollectKeys(_table, "", keys);
            std::set<std::unique_ptr<Value>> result;
            for (const auto& key : keys)
                result.insert(std::make_unique<TomlValue>(&_table, key));
            return result;
        }

    private:
        toml::table _table;

        static void CollectKeys(const toml::table& table,
            const std::string& prefix,
            std::set<std::string>& out)
        {
            for (auto&& [key, node] : table)
            {
                const std::string full =
                    prefix.empty() ? std::string(key.str())
                                   : prefix + "." + std::string(key.str());
                if (node.is_table())
                {
                    if (const toml::table* sub = node.as_table())
                        CollectKeys(*sub, full, out);
                }
                else
                {
                    out.insert(full);
                }
            }
        }
    };

} // namespace

std::unique_ptr<Preferences> CreateTomlPreferences()
{
    return std::make_unique<TomlPreferencesImpl>();
}

void WriteTomlPreferences(const Preferences& prefs)
{
    const std::string path = DefaultConfigPath();
    const std::filesystem::path filePath(path);
    const std::filesystem::path dir = filePath.parent_path();
    if (!dir.empty())
    {
        std::error_code ec;
        std::filesystem::create_directories(dir, ec);
    }

    toml::table table;

    for (const auto& v : prefs.GetAll())
    {
        const std::string& key = v->GetName();

        switch (v->GetType())
        {
            case Value::Type::StringList:
            {
                if (auto listOpt = v->GetStringList())
                {
                    toml::array arr;
                    for (const std::string& s : *listOpt)
                        arr.emplace_back(s);
                    InsertDotted(table, key, std::move(arr));
                    continue;
                }
                break;
            }
            case Value::Type::Integer:
            {
                if (auto iv = v->GetInteger())
                {
                    InsertDotted(table, key, static_cast<int64_t>(*iv));
                    continue;
                }
                break;
            }
            case Value::Type::Double:
            {
                if (auto dv = v->GetDouble())
                {
                    InsertDotted(table, key, *dv);
                    continue;
                }
                break;
            }
            case Value::Type::Boolean:
            {
                if (auto bv = v->GetBoolean())
                {
                    // Only write as boolean if GetString would be
                    // "true"/"false" to avoid writing integer 1/0 as boolean
                    auto str = v->GetString();
                    if (str && (*str == "true" || *str == "false"))
                    {
                        InsertDotted(table, key, *bv);
                        continue;
                    }
                }
                break;
            }
            case Value::Type::String:
            default:
                break;
        }

        if (auto strOpt = v->GetString())
            InsertDotted(table, key, *strOpt);
    }

    std::ofstream out(path);
    out << table;
}

void WriteTomlPreferences(Preferences& prefs)
{
    WriteTomlPreferences(static_cast<const Preferences&>(prefs));
}
