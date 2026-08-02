#include "components.h"

#include <cstdio>
#include <memory>
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
    // Heap-allocated so the reference Fruit binds (and later reads during
    // injection) outlives the Injector.
    auto args = std::make_unique<CliArgs>();
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            PrintUsage(argv[0]);
            return 0;
        }
        if (arg.rfind("--side=", 0) != 0 && arg.rfind("--size=", 0) != 0 &&
            arg.rfind("--monitor=", 0) != 0) {
            std::fprintf(stderr, "unknown argument: %s\n", arg.c_str());
            PrintUsage(argv[0]);
            return 1;
        }
        args->values.push_back(arg);
    }

    fruit::Injector<App> injector(GetAppComponent, args.get());
    return injector.get<App &>().Run();
}
