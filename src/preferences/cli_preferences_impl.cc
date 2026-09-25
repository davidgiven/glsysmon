#include "preferences.h"

#include <memory>
#include <map>
#include <optional>
#include <set>
#include <string>

namespace
{

    class CliPreferencesImpl : public Preferences
    {
    public:
        explicit CliPreferencesImpl(const CliArgs& args)
        {
            for (const std::string& arg : args.values)
            {
                if (arg.rfind("--side=", 0) == 0)
                    _values["side"] = arg.substr(7);
                else if (arg.rfind("--size=", 0) == 0)
                    _values["size"] = arg.substr(7);
                else if (arg.rfind("--monitor=", 0) == 0)
                    _values["monitor"] = arg.substr(10);
                else if (arg.rfind("--views=", 0) == 0)
                    _values["views"] = arg.substr(8);
                else if (arg.rfind("--fps=", 0) == 0)
                    _values["fps"] = arg.substr(6);
                else if (arg.rfind("--", 0) == 0)
                {
                    const std::size_t eq = arg.find('=');
                    if (eq != std::string::npos)
                    {
                        const std::string key = arg.substr(2, eq - 2);
                        const std::string value = arg.substr(eq + 1);
                        _values[key] = value;
                    }
                }
            }
        }

        std::unique_ptr<Value> Get(const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return nullptr;
            return CreateStringValue(it->second);
        }

        std::set<std::string> GetAll() const override
        {
            std::set<std::string> result;
            for (const auto& [key, _] : _values)
                result.insert(key);
            return result;
        }

    private:
        std::map<std::string, std::string> _values;
    };

} // namespace

std::unique_ptr<Preferences> CreateCliPreferences(const CliArgs& args)
{
    return std::make_unique<CliPreferencesImpl>(args);
}
