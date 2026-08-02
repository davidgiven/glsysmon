#include "preferences.h"

#include <fruit/fruit.h>

#include <charconv>
#include <map>
#include <optional>
#include <string>

#include "components.h"

namespace
{

    class CliPreferencesImpl : public Preferences
    {
    public:
        explicit CliPreferencesImpl(CliArgs args)
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
            }
        }

        using Inject = CliPreferencesImpl(CliArgs);

        std::optional<std::string> GetString(
            const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return std::nullopt;
            return it->second;
        }

        std::optional<int> GetInteger(const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return std::nullopt;
            int value = 0;
            const char* begin = it->second.data();
            const char* end = begin + it->second.size();
            const auto result = std::from_chars(begin, end, value);
            if (result.ec != std::errc{} || result.ptr != end)
                return std::nullopt;
            return value;
        }

        std::optional<std::vector<std::string>> GetStringList(
            const std::string& key) const override
        {
            const auto it = _values.find(key);
            if (it == _values.end())
                return std::nullopt;
            std::vector<std::string> result;
            std::size_t start = 0;
            while (start <= it->second.size())
            {
                const std::size_t comma = it->second.find(',', start);
                const std::size_t end =
                    comma == std::string::npos ? it->second.size() : comma;
                result.push_back(it->second.substr(start, end - start));
                if (comma == std::string::npos)
                    break;
                start = comma + 1;
            }
            return result;
        }

    private:
        std::map<std::string, std::string> _values;
    };

} // namespace

fruit::Component<fruit::Required<CliArgs>,
    fruit::Annotated<CliPreference, Preferences>>
GetCliPreferencesComponent()
{
    return fruit::createComponent()
        .bind<fruit::Annotated<CliPreference, Preferences>,
            CliPreferencesImpl>();
}
