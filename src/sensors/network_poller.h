#pragma once

#include "app.h"

#include "poller.h"
#include "sensor_rx_tx_graph_mixin.h"

class App;

#include <map>
#include <memory>
#include <string>

class NetworkPoller : public Poller<RxTxSample>
{
public:
    virtual ~NetworkPoller() = default;
};

extern std::unique_ptr<NetworkPoller> CreateNetworkPoller(
    const std::string& procNetDevPath = "/proc/net/dev");

extern std::unique_ptr<NetworkPoller> CreateNetworkPoller(
    App& app, const std::string& procNetDevPath = "/proc/net/dev");
