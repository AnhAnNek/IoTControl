#include "SensorManager.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin Configuration
#define TEMP_PIN 26 // DS18B20 DATA pin
#define PHOTORESISTOR_PIN 17  
#define INFRARED_SENSOR_PIN 19
#define SOUND_SENSOR_PIN 18

// DS18B20 Setup
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

void initializeSensors() {
  sensors.begin();
  
  pinMode(PHOTORESISTOR_PIN, OUTPUT);
  pinMode(INFRARED_SENSOR_PIN, OUTPUT);
  pinMode(SOUND_SENSOR_PIN, OUTPUT);
}

float getTemperature() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  return temp;
}

int readPhotoresistor() {
  int value = analogRead(PHOTORESISTOR_PIN);
  return value;
}

int readInfraredSensor() {
  int value = analogRead(INFRARED_SENSOR_PIN);
  return value;
}

int readSoundSensor() {
  int value = analogRead(SOUND_SENSOR_PIN);
  return value;
}