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

#ifdef ENABLE_PROFILER
#include <gperftools/profiler.h>
#include <cstdlib>
#endif

int main(int argc, char** argv)
{
#ifdef ENABLE_PROFILER
    const char* profilePath = std::getenv("CPUPROFILE");
    bool profilerStarted = false;
    if (profilePath != nullptr && profilePath[0] != '\0')
    {
        if (ProfilingIsEnabledForAllThreads())
            profilerStarted = true;
        else if (ProfilerStart(profilePath))
            profilerStarted = true;
        else
            std::fprintf(stderr, "glsysmon: ProfilerStart(%s) failed\n",
                profilePath);
    }
#endif
    if (setjmp(g_restartJmp) != 0)
    {
#ifdef ENABLE_PROFILER
        if (profilerStarted)
            ProfilerStop();
#endif
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
#ifdef ENABLE_PROFILER
        if (profilerStarted)
            ProfilerStop();
#endif
        return 1;
    }
#ifdef ENABLE_PROFILER
    if (profilerStarted)
        ProfilerStop();
#endif
    return 0;
}
