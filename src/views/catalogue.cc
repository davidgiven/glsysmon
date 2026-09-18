#include "catalogue.h"

#include <map>
#include <string>

#include "components.h"
#include "view.h"

const std::map<std::string, ViewFactory>& GetViewCatalogue()
{
    static const std::map<std::string, ViewFactory> catalogue = {
        {"HostnameView",
            []() -> std::unique_ptr<View> { return CreateHostnameView(); }},
    };
    return catalogue;
}
