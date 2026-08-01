#include "components.h"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

void PrintUsage(const char *prog)
{
    std::fprintf(stderr,
                 "Usage: %s [--side=left|right|top|bottom] [--size=px] "
                 "[--monitor=N] [--help]\n",
                 prog);
}

}  // namespace

int main(int argc, char **argv)
{
    DockConfig cfg;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintUsage(argv[0]);
            return 0;
        }
        if (arg.rfind("--side=", 0) == 0) {
            cfg.side = arg.substr(7);
            continue;
        }
        if (arg.rfind("--size=", 0) == 0) {
            cfg.size = std::atoi(arg.substr(7).c_str());
            continue;
        }
        if (arg.rfind("--monitor=", 0) == 0) {
            cfg.monitor = std::atoi(arg.substr(10).c_str());
            continue;
        }
        std::fprintf(stderr, "unknown argument: %s\n", arg.c_str());
        PrintUsage(argv[0]);
        return 1;
    }

    fruit::Injector<App> injector(GetAppComponent);
    return injector.get<App &>().Run(cfg);
}
