#include "app.h"
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

#include "imguiif.h"
#include "preferences/preferences.h"
#include "sensors/sensors.h"
#include "views/style.h"

namespace
{

    class ClockViewImpl : public View
    {
    public:
        explicit ClockViewImpl(App& app, Sensors& sensors):
            _app(app),
            _sensor(sensors.CreateClockSensor(GetPrefName()))
        {
        }

        explicit ClockViewImpl(
            App& app, std::unique_ptr<ClockSensor> sensor):
            _app(app),
            _sensor(std::move(sensor))
        {
        }

        void Draw() override
        {
            std::tm tm = _sensor->GetLocalTime();
            std::string format = GetFormat(_app.GetPreferencesRef());
            std::ostringstream oss;
            oss << std::put_time(&tm, format.c_str());

            std::string text = oss.str();
            std::istringstream iss(text);
            std::string line;
            double scale = GetTextScale(_app.GetPreferencesRef());
            _app.GetImGui().SetWindowFontScale(static_cast<float>(scale));
            while (std::getline(iss, line))
                Style::DrawCentredText(_app.GetImGui(), line);
            _app.GetImGui().SetWindowFontScale(1.0f);
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
            float line_h = _app.GetImGui().GetTextLineHeight();
            float h =
                line_h * static_cast<float>(std::clamp(lines + 1, 3, 10)) +
                _app.GetImGui().GetStyle().FramePadding.y * 2.0f;
            ImVec2 size(-FLT_MIN, h);

            if (_app.GetImGui().InputTextMultiline("Format",
                    buf.data(),
                    buf.capacity() + 1,
                    size,
                    ImGuiInputTextFlags_CallbackResize,
                    Callback::Resize,
                    &buf))
                SetFormat(preferences, buf);

            double textScale = GetTextScale(preferences);
            if (_app.GetImGui().InputDouble("Text scale", &textScale))
                SetTextScale(preferences, textScale);
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
            return _app.GetImGui();
        }

        const ImGuiIf& GetImGui() const override
        {
            return _app.GetImGui();
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

        double GetTextScale(const Preferences& prefs) const
        {
            return prefs.GetDouble(GetPrefName() + ".text_scale").value_or(1.2);
        }

        void SetTextScale(Preferences& prefs, double value) const
        {
            prefs.SetDouble(GetPrefName() + ".text_scale", value);
        }

        App& _app;
        std::unique_ptr<ClockSensor> _sensor;
    };

} // namespace

std::unique_ptr<View> CreateClockView(App& app, Sensors& sensors)
{
    return std::make_unique<ClockViewImpl>(app, sensors);
}

std::unique_ptr<View> CreateClockView(
    App& app, std::unique_ptr<ClockSensor> sensor)
{
    return std::make_unique<ClockViewImpl>(app, std::move(sensor));
}
