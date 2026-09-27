#include "views/view.h"

#include <imgui.h>
#include <imhtml.hpp>

#include <algorithm>
#include <ctime>
#include <format>
#include <iomanip>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "preferences/preferences.h"
#include "sensors/sensors.h"

namespace
{

    class ClockViewImpl : public View
    {
    public:
        explicit ClockViewImpl(const Preferences& prefs, Sensors& sensors):
            _prefs(prefs),
            _sensor(sensors.CreateClockSensor(GetPrefName()))
        {
        }

        explicit ClockViewImpl(
            const Preferences& prefs, std::unique_ptr<ClockSensor> sensor):
            _prefs(prefs),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            std::tm tm = _sensor->GetLocalTime();
            std::string format = GetFormat(_prefs);
            ImVec4 col = ImGui::GetStyle().Colors[ImGuiCol_Text];
            std::ostringstream oss;
            oss << std::format("<div style=\"color: #{:02X}{:02X}{:02X};\">",
                       static_cast<int>(col.x * 255),
                       static_cast<int>(col.y * 255),
                       static_cast<int>(col.z * 255))
                << std::put_time(&tm, format.c_str()) << "</div>";
            std::string html = oss.str();
            ImHTML::Canvas("clock", html.c_str());
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
            float line_h = ImGui::GetTextLineHeight();
            float h =
                line_h * static_cast<float>(std::clamp(lines + 1, 3, 10)) +
                ImGui::GetStyle().FramePadding.y * 2.0f;
            ImVec2 size(-FLT_MIN, h);

            if (ImGui::InputTextMultiline("Format",
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
                .value_or("<center>%Y-%m-%d<br>%H:%M:%S</center>");
        }

        void SetFormat(Preferences& prefs, const std::string& value) const
        {
            prefs.SetString(GetPrefName() + ".format", value);
        }

        const Preferences& _prefs;
        std::unique_ptr<ClockSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateClockView(
    const Preferences& prefs, Sensors& sensors)
{
    return std::make_unique<ClockViewImpl>(prefs, sensors);
}

std::unique_ptr<View> CreateClockView(
    const Preferences& prefs, std::unique_ptr<ClockSensor> sensor)
{
    return std::make_unique<ClockViewImpl>(prefs, std::move(sensor));
}
