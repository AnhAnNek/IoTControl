#include "SensorManager.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin Configuration
#define TEMP_PIN 26 // DS18B20 DATA pin

// DS18B20 Setup
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

void initializeSensors() {
  sensors.begin();
}

float getTemperature() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  return temp;
}
