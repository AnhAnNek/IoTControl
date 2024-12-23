#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <DallasTemperature.h>

void initializeSensors();
float getTemperature();

#endif