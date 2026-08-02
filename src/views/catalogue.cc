#include "catalogue.h"

#include <fruit/fruit.h>

#include <map>
#include <string>
#include <utility>

#include "components.h"
#include "view.h"

namespace
{

    std::map<std::string, fruit::Component<View> (*)()> BuildViewCatalogue()
    {
        std::map<std::string, fruit::Component<View> (*)()> catalogue;
        catalogue.emplace("HostnameView", GetViewComponent);
        return catalogue;
    }

} // namespace

const std::map<std::string, fruit::Component<View> (*)()>& GetViewCatalogue()
{
    static const std::map<std::string, fruit::Component<View> (*)()> catalogue =
        BuildViewCatalogue();
    return catalogue;
}
