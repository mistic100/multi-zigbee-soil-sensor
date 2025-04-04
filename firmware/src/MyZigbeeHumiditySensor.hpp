#pragma once

#include <Zigbee.h>

class MyZigbeeHumiditySensor : public ZigbeeTempSensor
{

public:
    MyZigbeeHumiditySensor(uint8_t endpoint) : ZigbeeTempSensor(endpoint)
    {
    }

    void init()
    {
        setManufacturerAndModel("StrangePlanet", "PlantsSensor");
        addHumiditySensor(0, 100, 0.1);
    }

    void sendHumidity(float value) {
        setHumidity(value);
        reportHumidity();
    }
};
