#include "temperature_sensor.h"

#include <imgui.h>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <functional>
#include <glob.h>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "context.h"
#include "preferences/preferences.h"
#include "sensor_graph_mixin.h"
#include "timer.h"
#include "utils.h"

namespace
{

    std::string ExtractTempBase(const std::string& inputPath)
    {
        std::size_t slash = inputPath.rfind('/');
        std::string base = slash == std::string::npos
                               ? inputPath
                               : inputPath.substr(slash + 1);
        std::size_t pos = base.rfind("_input");
        if (pos != std::string::npos)
            return base.substr(0, pos);
        return base;
    }

    std::vector<std::string> DiscoverInputs(const std::string& hwmonRoot)
    {
        std::string pattern = hwmonRoot + "/hwmon*/temp*_input";
        glob_t g{};
        int ret = glob(pattern.c_str(), 0, nullptr, &g);
        std::vector<std::string> result;
        if (ret == 0)
        {
            for (std::size_t i = 0; i < g.gl_pathc; ++i)
                result.emplace_back(g.gl_pathv[i]);
        }
        globfree(&g);
        std::sort(result.begin(), result.end());
        return result;
    }

    class TemperatureSensorImpl : public TemperatureSensor
    {
    public:
        explicit TemperatureSensorImpl(const Context& ctx,
            const std::string& prefPrefix,
            const std::string& hwmonRoot): TemperatureSensor(ctx, prefPrefix),
            _ctx(ctx),
            _hwmonRoot(hwmonRoot)
        {
            _inputPaths = DiscoverInputs(_hwmonRoot);
            std::vector<std::string> baseNames;
            baseNames.reserve(_inputPaths.size());
            for (const std::string& input : _inputPaths)
            {
                std::string hwmonDir;
                std::size_t slash = input.rfind('/');
                if (slash != std::string::npos)
                    hwmonDir = input.substr(0, slash);
                std::string namePath = hwmonDir + "/name";
                std::ifstream nf(namePath);
                std::string name;
                if (nf)
                    std::getline(nf, name);
                name = Trim(name);

                std::string labelPath = input;
                std::size_t pos = labelPath.rfind("_input");
                if (pos != std::string::npos)
                    labelPath.replace(pos, 6, "_label");
                std::ifstream lf(labelPath);
                std::string label;
                if (lf)
                    std::getline(lf, label);
                label = Trim(label);
                if (label.empty())
                    label = ExtractTempBase(input);

                std::string combined;
                if (!name.empty())
                    combined = name + " " + label;
                else
                    combined = label;
                baseNames.push_back(combined);
            }

            std::unordered_map<std::string, int> counts;
            for (const auto& n : baseNames)
                counts[n]++;

            std::unordered_map<std::string, int> seen;
            _names.reserve(baseNames.size());
            for (const auto& base : baseNames)
            {
                if (counts[base] > 1)
                {
                    int idx = ++seen[base];
                    _names.push_back(base + " #" + std::to_string(idx));
                }
                else
                    _names.push_back(base);
            }

            InitGraph(_ctx,
                _prefPrefix,
                _inputPaths.size(),
                std::numeric_limits<double>::quiet_NaN());
            Tick(_ctx.timer.Now());
        }

        const double* GetSamples(std::size_t channel) const override
        {
            if (channel >= GetChannels())
                return nullptr;
            return this->SensorGraphMixin<double>::GetSamples(channel);
        }

        std::string GetChannelName(std::size_t channel) const override
        {
            if (channel < _names.size())
                return _names[channel];
            return "temp" + std::to_string(channel + 1);
        }

        std::string GetHumanName() const override
        {
            return "Temperature";
        }

        std::string GetPrefName() const override
        {
            return "temperature";
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            DrawIntervalConfiguration(preferences, _prefPrefix);
        }

    private:
        void Tick(Timer::Time t)
        {
            for (std::size_t i = 0; i < _inputPaths.size(); ++i)
            {
                std::ifstream f(_inputPaths[i]);
                if (!f)
                {
                    AddSample(i, std::numeric_limits<double>::quiet_NaN());
                    continue;
                }
                std::string line;
                std::getline(f, line);
                line = Trim(line);
                if (line.empty())
                {
                    AddSample(i, std::numeric_limits<double>::quiet_NaN());
                    continue;
                }
                try
                {
                    long value = std::stol(line);
                    double c = static_cast<double>(value) / 1000.0;
                    AddSample(i, c);
                }
                catch (...)
                {
                    AddSample(i, std::numeric_limits<double>::quiet_NaN());
                }
            }
            _ctx.timer.Schedule(t + this->_delta,
                std::bind(
                    &TemperatureSensorImpl::Tick, this, std::placeholders::_1));
        }

        const Context& _ctx;
        std::string _hwmonRoot;
        std::vector<std::string> _inputPaths;
        std::vector<std::string> _names;
    };

} // namespace

std::unique_ptr<TemperatureSensor> CreateTemperatureSensor(const Context& ctx,
    const std::string& prefPrefix,
    const std::string& hwmonRoot)
{
    return std::make_unique<TemperatureSensorImpl>(ctx, prefPrefix, hwmonRoot);
}
