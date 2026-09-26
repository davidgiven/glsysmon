#define _GNU_SOURCE
#define GLSYSMON_BFM
#define ENABLE_FISH
#define ENABLE_DUCK
#define ENABLE_CPU
#define UPSIDE_DOWN_DUCK

#include <sys/types.h>
#include "include/bubblemon.h"
#include "include/sys_include.h"

#include <stdlib.h>

extern BubbleMonData bm;

static int g_bfm_cpu_percent = 0;

void bfm_set_cpu_percent(int percent)
{
    if (percent < 0)
        percent = 0;
    if (percent > 100)
        percent = 100;
    g_bfm_cpu_percent = percent;
}

void bfm_set_cpu_usage(int percent)
{
    bfm_set_cpu_percent(percent);
}

void bfm_set_cpu(int percent)
{
    bfm_set_cpu_percent(percent);
}

void bfm_set_cpu_load(int percent)
{
    bfm_set_cpu_percent(percent);
}

int system_cpu(void)
{
    return g_bfm_cpu_percent;
}

int system_memory(void)
{
    static int once = 0;
    if (once == 0)
    {
        once = 1;
        bm.mem_used = 4ULL * 1024 * 1024 * 1024;
        bm.mem_max = 8ULL * 1024 * 1024 * 1024;
        bm.swap_used = 512ULL * 1024 * 1024;
        bm.swap_max = 2ULL * 1024 * 1024 * 1024;
        bm.mem_percent = 50;
        bm.swap_percent = 10;
        return 1;
    }
    if ((rand() % 20) == 0)
    {
        int d = (rand() % 7) - 3;
        int p = (int)bm.mem_percent + d;
        if (p < 10)
            p = 10;
        if (p > 90)
            p = 90;
        bm.mem_percent = (unsigned int)p;
        bm.mem_used = bm.mem_max * bm.mem_percent / 100;
        return 1;
    }
    return 0;
}

static int g_bfm_net_tx = 0;
static int g_bfm_net_rx = 0;

void bfm_set_network_speed(int rx, int tx)
{
    g_bfm_net_rx = rx;
    g_bfm_net_tx = tx;
}

int net_tx_speed(void)
{
    return g_bfm_net_tx;
}

int net_rx_speed(void)
{
    return g_bfm_net_rx;
}
