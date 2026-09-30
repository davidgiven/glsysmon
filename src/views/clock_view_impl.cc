#include "views/view.h"

#include <imgui.h>

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "context.h"
#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "views/style.h"

namespace
{

    class ClockViewImpl : public View
    {
    public:
        explicit ClockViewImpl(const Context& ctx, Sensors& sensors):
            _context(ctx),
            _sensor(sensors.CreateClockSensor(GetPrefName()))
        {
        }

        explicit ClockViewImpl(
            const Context& ctx, std::unique_ptr<ClockSensor> sensor):
            _context(ctx),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            std::tm tm = _sensor->GetLocalTime();
            std::string format = GetFormat(_context.preferences);
            std::ostringstream oss;
            oss << std::put_time(&tm, format.c_str());

            std::string text = oss.str();
            std::istringstream iss(text);
            std::string line;
            while (std::getline(iss, line))
                Style::DrawCentredText(_context.imgui, line);
        }

        void DrawConfiguration(Preferences& preferences) override
        {
            std::string current = GetFormat(preferences);

            struct Callback
            {
                static int Resize(ImGuiInputTextCallbackData* data)
                {
                    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
                    {
                        std::string* str =
                            static_cast<std::string*>(data->UserData);
                        str->resize(static_cast<std::size_t>(data->BufTextLen));
                        data->Buf = str->data();
                    }
                    return 0;
                }
            };

            std::string buf = current;
            buf.reserve(8192);

            int lines = 1;
            for (char c : buf)
                if (c == '\n')
                    ++lines;
            float line_h = _context.imgui.GetTextLineHeight();
            float h =
                line_h * static_cast<float>(std::clamp(lines + 1, 3, 10)) +
                _context.imgui.GetStyle().FramePadding.y * 2.0f;
            ImVec2 size(-FLT_MIN, h);

            if (_context.imgui.InputTextMultiline("Format",
                    buf.data(),
                    buf.capacity() + 1,
                    size,
                    ImGuiInputTextFlags_CallbackResize,
                    Callback::Resize,
                    &buf))
                SetFormat(preferences, buf);
        }

        std::string GetHumanName() const override
        {
            return "Clock";
        }

        std::string GetPrefName() const override
        {
            return "clock";
        }

        ImGuiIf& GetImGui() override
        {
            return _context.imgui;
        }

        const ImGuiIf& GetImGui() const override
        {
            return _context.imgui;
        }

        std::vector<Sensor*> GetSensors() override
        {
            return {static_cast<Sensor*>(_sensor.get())};
        }

        std::vector<Sensor*> GetSensors() const override
        {
            return {static_cast<Sensor*>(_sensor.get())};
        }

    private:
        std::string GetFormat(const Preferences& prefs) const
        {
            return prefs.GetString(GetPrefName() + ".format")
                .value_or("%Y-%m-%d\n%H:%M:%S");
        }

        void SetFormat(Preferences& prefs, const std::string& value) const
        {
            prefs.SetString(GetPrefName() + ".format", value);
        }

        const Context& _context;
        std::unique_ptr<ClockSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateClockView(const Context& ctx, Sensors& sensors)
{
    return std::make_unique<ClockViewImpl>(ctx, sensors);
}

std::unique_ptr<View> CreateClockView(
    const Context& ctx, std::unique_ptr<ClockSensor> sensor)
{
    return std::make_unique<ClockViewImpl>(ctx, std::move(sensor));
}
