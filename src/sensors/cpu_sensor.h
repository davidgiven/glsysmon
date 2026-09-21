#pragma once

#include "graph_mixin.h"
#include "sensor.h"

struct CpuSample
{
    float user;
    float system;
    float nice;
};

class CpuSensor : public Sensor, public GraphMixin<CpuSample>
{
public:
    virtual ~CpuSensor() = default;
};
