#include "app.h"
#include "preferences/preferences.h"
#include "globals.h"

#include <cerrno>
#include <cstdio>
#include <csetjmp>
#include <cstring>
#include <exception>
#include <memory>
#include <string>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (setjmp(g_restartJmp) != 0)
    {
        execv(argv[0], argv);
        std::fprintf(
            stderr, "glsysmon: execv failed: %s\n", std::strerror(errno));
        return 1;
    }

    auto args = std::make_unique<CliArgs>();
    for (int i = 1; i < argc; ++i)
        args->values.push_back(argv[i]);

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
