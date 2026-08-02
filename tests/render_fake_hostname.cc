// Visual-snapshot test: runs the app with only HostnameView enabled, using a
// fake HostnameSensor, and captures a PNG of the rendered result.

#include <fruit/fruit.h>

#include <string>

#include "components.h"
#include "imgui_frame_renderer.h"
#include "render_frame.h"
#include "sensors/hostname_sensor.h"
#include "ui.h"

namespace
{

    class FakeHostnameSensor : public HostnameSensor
    {
    public:
        using Inject = FakeHostnameSensor();

        std::string GetHostname() override
        {
            return "fake-hostname";
        }
    };

    fruit::Component<Ui, ImGuiFrameRenderer> GetFakeComponents(CliArgs* args)
    {
        return fruit::createComponent()
            .install(GetUiComponent)
            .install(GetImGuiFrameRendererComponent)
            .bindInstance(*args)
            .bind<HostnameSensor, FakeHostnameSensor>();
    }

} // namespace

int main()
{
    CliArgs args;
    args.values = {"--views=HostnameView"};
    return render_frame::RenderFrame(240,
        0,
        "tests/render_fake_hostname.bad.png",
        GetFakeComponents,
        &args);
}
