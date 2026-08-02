#include "preferences.h"

#include <fruit/fruit.h>

#include <charconv>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "components.h"

namespace {

class CliPreferencesImpl : public Preferences {
public:
    explicit CliPreferencesImpl(std::vector<std::string> args)
    {
        for (const std::string &arg : args) {
            if (arg.rfind("--side=", 0) == 0)
                _values["side"] = arg.substr(7);
            else if (arg.rfind("--size=", 0) == 0)
                _values["size"] = arg.substr(7);
            else if (arg.rfind("--monitor=", 0) == 0)
                _values["monitor"] = arg.substr(10);
        }
    }

    using Inject = CliPreferencesImpl(std::vector<std::string>);

    std::optional<std::string> GetString(const std::string &key) const override
    {
        const auto it = _values.find(key);
        if (it == _values.end())
            return std::nullopt;
        return it->second;
    }

    std::optional<int> GetInteger(const std::string &key) const override
    {
        const auto it = _values.find(key);
        if (it == _values.end())
            return std::nullopt;
        int value = 0;
        const char *begin = it->second.data();
        const char *end = begin + it->second.size();
        const auto result = std::from_chars(begin, end, value);
        if (result.ec != std::errc{} || result.ptr != end)
            return std::nullopt;
        return value;
    }

private:
    std::map<std::string, std::string> _values;
};

}  // namespace

fruit::Component<fruit::Required<std::vector<std::string>>,
                 fruit::Annotated<CliPreference, Preferences>>
GetCliPreferencesComponent()
{
    return fruit::createComponent()
        .bind<fruit::Annotated<CliPreference, Preferences>, CliPreferencesImpl>();
}
