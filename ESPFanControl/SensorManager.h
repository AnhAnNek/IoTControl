#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <OneWire.h>
#include <DallasTemperature.h>

void initializeSensors();
float getTemperature();
int readPhotoresistor();
int readInfraredSensor();
int readSoundSensor();

#endif