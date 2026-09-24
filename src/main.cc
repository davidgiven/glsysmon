#include "app.h"
#include "preferences/preferences.h"

#include <cstdio>
#include <exception>
#include <memory>
#include <string>

namespace
{

    void PrintUsage(const char* prog)
    {
        std::fprintf(stderr,
            "Usage: %s [--side=left|right] [--size=px] "
            "[--monitor=N] [--views=View1,View2] [--fps=N] "
            "[--help]\n",
            prog);
    }

} // namespace

int main(int argc, char** argv)
{
    auto args = std::make_unique<CliArgs>();
    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        if (arg == "--help" || arg == "-h")
        {
            PrintUsage(argv[0]);
            return 0;
        }
        if (arg.rfind("--side=", 0) != 0 && arg.rfind("--size=", 0) != 0 &&
            arg.rfind("--monitor=", 0) != 0 && arg.rfind("--views=", 0) != 0 &&
            arg.rfind("--fps=", 0) != 0)
        {
            std::fprintf(stderr, "unknown argument: %s\n", arg.c_str());
            PrintUsage(argv[0]);
            return 1;
        }
        args->values.push_back(arg);
    }

    try
    {
        auto app = CreateApp(*args);
        app->Setup();
        app->MainLoop();
        app->Shutdown();
    }
    catch (const std::exception& e)
    {
        std::fprintf(stderr, "glsysmon: %s\n", e.what());
        return 1;
    }
    return 0;
}
