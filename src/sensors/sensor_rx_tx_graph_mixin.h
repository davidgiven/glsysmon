#pragma once

#include "sensor_graph_mixin.h"

struct RxTxSample
{
    double txBps;
    double rxBps;
};

class SensorRxTxGraphMixin : public SensorGraphMixin<RxTxSample>
{
public:
    virtual ~SensorRxTxGraphMixin() = default;
};
