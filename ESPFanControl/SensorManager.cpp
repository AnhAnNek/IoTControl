#include "SensorManager.h"
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <DHT_U.h>

// Constructor
SensorManager::SensorManager(int tempPin, int soundPin, int dhtPin)
    : tempPin(tempPin),
      soundPin(soundPin),
      dhtPin(dhtPin),
      oneWire(tempPin),
      sensors(&oneWire),
      dht(dhtPin, DHT11) { // Replace DHT11 with DHT22 if applicable
}

// Initialize sensors
void SensorManager::initializeSensors() {
  sensors.begin();
  dht.begin();
  pinMode(soundPin, INPUT);
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

// Read sound sensor value
int SensorManager::readSoundSensor() {
  return digitalRead(soundPin);
}
