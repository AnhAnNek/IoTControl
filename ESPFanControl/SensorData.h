#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <Arduino.h>

class SensorData {
public:
    // Properties
    const char* type;
    float temp;
    float envTemp;
    float envHumidity;

    // Constructor
    SensorData(const char* type, float temp, float envTemp, float envHumidity)
        : type(type), temp(temp), envTemp(envTemp), envHumidity(envHumidity) {}

    // Convert to JSON string
    const char* toJSON();
};

#endif
