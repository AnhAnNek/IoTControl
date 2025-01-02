#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <DHT_U.h>

class SensorManager {
public:
  // Constructor
  SensorManager(int tempPin, int dhtPin);

  // Initialize sensors
  void initializeSensors();

  // Get temperature readings
  float getTemperature();            // DS18B20
  float getEnvTemperature();         // DHT sensor
  float getEnvHumidity();            // DHT sensor humidity

private:
  // Pin configurations
  int tempPin;
  int dhtPin;

  // DS18B20 sensor
  OneWire oneWire;
  DallasTemperature sensors;

  // DHT sensor
  DHT dht;
};

#endif
