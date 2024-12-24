#include "SensorManager.h"

// Constructor
SensorManager::SensorManager(int tempPin, int photoresistorPin, int infraredPin, int soundPin)
    : tempPin(tempPin),
      photoresistorPin(photoresistorPin),
      infraredPin(infraredPin),
      soundPin(soundPin),
      oneWire(tempPin),
      sensors(&oneWire) {}

// Initialize sensors
void SensorManager::initializeSensors() {
  sensors.begin();
  
  pinMode(photoresistorPin, INPUT);
  pinMode(infraredPin, INPUT);
  pinMode(soundPin, INPUT);
}

// Get temperature from DS18B20
float SensorManager::getTemperature() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

// Read photoresistor value
int SensorManager::readPhotoresistor() {
  return analogRead(photoresistorPin);
}

// Read infrared sensor value
int SensorManager::readInfraredSensor() {
  return analogRead(infraredPin);
}

// Read sound sensor value
int SensorManager::readSoundSensor() {
  return analogRead(soundPin);
}
