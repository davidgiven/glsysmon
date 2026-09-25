#include "preferences.h"

#include <charconv>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>

namespace
{

    class MapValue : public Value
    {
    public:
        MapValue(std::map<std::string, std::string>* map, std::string key):
            _map(map),
            _key(std::move(key))
        {
        }

        std::string GetName() const override
        {
            return _key;
        }

        Type GetType() const override
        {
            auto s = GetString();
            if (!s)
                return Type::String;
            if (s->find(',') != std::string::npos)
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
            auto it = _map->find(_key);
            if (it == _map->end())
                return std::nullopt;
            return it->second;
        }

        std::optional<int> GetInteger() const override
        {
            auto s = GetString();
            if (!s)
                return std::nullopt;
            int value = 0;
            const char* begin = s->data();
            const char* end = begin + s->size();
            const auto result = std::from_chars(begin, end, value);
            if (result.ec != std::errc{} || result.ptr != end)
                return std::nullopt;
            return value;
        }

        std::optional<double> GetDouble() const override
        {
            auto s = GetString();
            if (!s)
                return std::nullopt;
            double value = 0;
            const char* begin = s->data();
            const char* end = begin + s->size();
            const auto result = std::from_chars(begin, end, value);
            if (result.ec != std::errc{} || result.ptr != end)
                return std::nullopt;
            return value;
        }

        std::optional<bool> GetBoolean() const override
        {
            auto s = GetString();
            if (!s)
                return std::nullopt;
            if (*s == "true" || *s == "1")
                return true;
            if (*s == "false" || *s == "0")
                return false;
            return std::nullopt;
        }

        std::optional<std::vector<std::string>> GetStringList() const override
        {
            auto s = GetString();
            if (!s)
                return std::nullopt;
            std::vector<std::string> result;
            std::size_t start = 0;
            while (start <= s->size())
            {
                const std::size_t comma = s->find(',', start);
                const std::size_t end =
                    comma == std::string::npos ? s->size() : comma;
                result.push_back(s->substr(start, end - start));
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

        void SetString(const std::string& value) override
        {
            (*_map)[_key] = value;
        }

        void SetInteger(int value) override
        {
            (*_map)[_key] = std::to_string(value);
        }

        void SetDouble(double value) override
        {
            (*_map)[_key] = std::to_string(value);
        }

        void SetBoolean(bool value) override
        {
            (*_map)[_key] = value ? "true" : "false";
        }

        void SetStringList(const std::vector<std::string>& value) override
        {
            std::string joined;
            for (std::size_t i = 0; i < value.size(); ++i)
            {
                if (i != 0)
                    joined.push_back(',');
                joined += value[i];
            }
            (*_map)[_key] = joined;
        }

        void SetStringSet(const std::set<std::string>& value) override
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
            (*_map)[_key] = joined;
        }

    private:
        std::map<std::string, std::string>* _map;
        std::string _key;
    };

    class MapPreferencesImpl : public Preferences
    {
    public:
        explicit MapPreferencesImpl(
            const std::map<std::string, std::string>& values):
            _values(values)
        {
        }

        std::unique_ptr<Value> Get(const std::string& key) const override
        {
            auto it = _values.find(key);
            if (it == _values.end())
                return nullptr;
            return std::make_unique<MapValue>(
                const_cast<std::map<std::string, std::string>*>(&_values), key);
        }

        std::unique_ptr<Value> Add(const std::string& key) override
        {
            return std::make_unique<MapValue>(&_values, key);
        }

        std::set<std::unique_ptr<Value>> GetAll() const override
        {
            std::set<std::unique_ptr<Value>> result;
            for (const auto& [key, value] : _values)
                result.insert(std::make_unique<MapValue>(
                    const_cast<std::map<std::string, std::string>*>(&_values),
                    key));
            return result;
        }

        void ClearAll() override
        {
            _values.clear();
        }

    private:
        mutable std::map<std::string, std::string> _values;
    };

} // namespace

std::shared_ptr<Preferences> CreateMapPreferences()
{
    return std::make_shared<MapPreferencesImpl>(
        std::map<std::string, std::string>());
}
