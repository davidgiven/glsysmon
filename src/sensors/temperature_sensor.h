#pragma once

#include "graph_mixin.h"
#include "sensor.h"

class TemperatureSensor : public Sensor, public GraphMixin<double>
{
public:
    virtual ~TemperatureSensor() = default;
};
