#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <DHT_U.h>

class SensorManager {
public:
  // Constructor
  SensorManager(int tempPin, int photoresistorPin, int infraredPin, int soundPin);

  // Initialize sensors
  void initializeSensors();

  // Get sensor readings
  float getTemperature();
  int readPhotoresistor();
  int readInfraredSensor();
  int readSoundSensor();

private:
  // Pin configurations
  int tempPin;
  int photoresistorPin;
  int infraredPin;
  int soundPin;

  // DS18B20 sensor
  OneWire oneWire;
  DallasTemperature sensors;
};

#endif
