#include "view.h"

#include <fruit/fruit.h>
#include <imgui.h>

namespace
{

    class HostnameViewImpl : public View
    {
    public:
        using Inject = HostnameViewImpl();

        void Tick() override
        {
            ImGui::Text("glrellm");
        }
    };

} // namespace

fruit::Component<View> GetViewComponent()
{
    return fruit::createComponent().bind<View, HostnameViewImpl>();
}
