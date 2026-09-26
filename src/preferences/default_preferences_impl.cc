#include "preferences.h"

#include <charconv>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>

namespace
{
    const std::map<std::string, std::string> kDefaultValues{
        {"side",                        "left"},
        {"size",                        "100" },
        {"monitor",                     "0"   },
        {"views",
         "HostnameView,"
            "ClockView,"
            "CpuView,"
            "TemperatureView,"
            "NetworkView"
            "DiskView"                        },
        {"fps",                         "30"  },
        {"cpu.graph_height",            "40"  },
        {"cpu.update_interval",         "2"   },
        {"temperature.graph_height",    "40"  },
        {"temperature.update_interval", "1"   },
        {"temperature.minimum",         "20"  },
        {"temperature.maximum",         "80"  },
        {"temperature.sensors",         "CPU" },
        {"network.graph_height",        "40"  },
        {"network.update_interval",     "1"   },
        {"network.maximum",             "0"   },
        {"network.interfaces",          ""    },
        {"network.sensors",             ""    },
        {"disk.graph_height",           "40"  },
        {"disk.update_interval",        "1"   },
        {"disk.maximum",                "0"   },
        {"disk.devices",                "sda" },
    };

    class DefaultValue : public Value
    {
    public:
        DefaultValue(std::string name, std::string s):
            _name(std::move(name)),
            _str(std::move(s))
        {
        }

        std::string GetName() const override
        {
            return _name;
        }

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

        void SetString(const std::string&) override
        {
            throw std::runtime_error("unsupported operation");
        }

        void SetInteger(int) override
        {
            throw std::runtime_error("unsupported operation");
        }

        void SetDouble(double) override
        {
            throw std::runtime_error("unsupported operation");
        }

        void SetBoolean(bool) override
        {
            throw std::runtime_error("unsupported operation");
        }

        void SetStringList(const std::vector<std::string>&) override
        {
            throw std::runtime_error("unsupported operation");
        }

        void SetStringSet(const std::set<std::string>&) override
        {
            throw std::runtime_error("unsupported operation");
        }

    private:
        std::string _name;
        std::string _str;
    };

    class DefaultPreferencesImpl : public Preferences
    {
    public:
        explicit DefaultPreferencesImpl(
            const std::map<std::string, std::string>& values):
            _values(values)
        {
        }

        std::unique_ptr<Value> Get(const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return nullptr;
            return std::make_unique<DefaultValue>(key, it->second);
        }

        std::unique_ptr<Value> Add(const std::string& key) override
        {
            auto it = _values.find(key);
            if (it != _values.end())
                return std::make_unique<DefaultValue>(key, it->second);
            return std::make_unique<DefaultValue>(key, "");
        }

        std::set<std::unique_ptr<Value>> GetAll() const override
        {
            std::set<std::unique_ptr<Value>> result;
            for (const auto& [key, value] : _values)
                result.insert(std::make_unique<DefaultValue>(key, value));
            return result;
        }

    private:
        std::map<std::string, std::string> _values;
    };

} // namespace

std::unique_ptr<Preferences> CreateDefaultPreferences()
{
    return std::make_unique<DefaultPreferencesImpl>(kDefaultValues);
}
