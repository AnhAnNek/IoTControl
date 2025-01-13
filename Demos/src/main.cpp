#include <Arduino.h>
#include "ACS712.h"

#define RADAR_PIN 5  // Connect OUT of RCWL-0516 to GPIO 5

ACS712 sensor(ACS712_30A, 34);

#define IR_SENSOR_PIN 21  // Connect OUT of HW-488 to GPIO 21

void setup() {
  Serial.begin(115200);

  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  int zero = sensor.calibrate();
  Serial.println("Done!");
  Serial.println("Zero point for this sensor = " + zero);

  pinMode(RADAR_PIN, INPUT);

  pinMode(IR_SENSOR_PIN, INPUT);
}

void loop() {
  int motionDetected = digitalRead(RADAR_PIN);

  int sensorValue = digitalRead(IR_SENSOR_PIN);
  
  if (motionDetected == HIGH) {
    Serial.println("Motion detected!");
  } else {
    Serial.println("No motion detected.");
  }

  // Read current from sensor
  float I = sensor.getCurrentDC();
  Serial.println(String("I = ") + I + " A");
  
  delay(1000);  // Reduce signal read frequency
}
