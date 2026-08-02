#include "catalogue.h"

#include <fruit/fruit.h>

#include <map>
#include <string>

#include "components.h"
#include "view.h"

const std::map<std::string, fruit::Component<View> (*)()>& GetViewCatalogue()
{
    static const std::map<std::string, fruit::Component<View> (*)()> catalogue =
        {
            {"HostnameView", GetViewComponent},
    };
    return catalogue;
}
