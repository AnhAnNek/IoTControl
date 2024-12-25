#include "SensorManager.h"
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <DHT_U.h>

// Constructor
SensorManager::SensorManager(int tempPin, int dhtPin)
    : tempPin(tempPin),
      dhtPin(dhtPin),
      oneWire(tempPin),
      sensors(&oneWire),
      dht(dhtPin, DHT11) { // Replace DHT11 with DHT22 if applicable
}

// Initialize sensors
void SensorManager::initializeSensors() {
  sensors.begin();
  dht.begin();
}

// Get temperature from DS18B20
float SensorManager::getTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

// Get temperature from DHT sensor
float SensorManager::getEnvTemperature() {
  return dht.readTemperature(); // Returns temperature in Celsius
}

// Get humidity from DHT sensor
float SensorManager::getEnvHumidity() {
  return dht.readHumidity();
}
