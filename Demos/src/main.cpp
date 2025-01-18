#include <Arduino.h>
#include "ACS712.h"

#define RADAR_PIN 5  // Connect OUT of RCWL-0516 to GPIO 5

ACS712 sensor(ACS712_30A, 34);

#define IR_SENSOR_PIN 21  // Connect OUT of HW-488 to GPIO 21
#define VIBRATION_SENSOR_PIN 23
#define VOLTAGE_SENSOR_PIN 34

const int motorPin = 13;
const int pwmFreq = 20000;        // Tần số PWM: 5 kHz
const int pwmChannel = 0;        // Kênh PWM
const int pwmResolution = 8;     // Độ phân giải 8 bit (0-255)
const int maxSpeed = 255;        // Maximum speed

// Voltage sensor calibration
const float voltageDividerRatio = 5.0; // For 0-25V sensors with a 5:1 divider
const float adcResolution = 3.3 / 4095.0; // Assuming a 12-bit ADC (ESP32)

void setup() {
  Serial.begin(115200);

  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  int zero = sensor.calibrate();
  Serial.println("Done!");
  Serial.println("Zero point for this sensor = " + zero);

  pinMode(RADAR_PIN, INPUT);

  pinMode(IR_SENSOR_PIN, INPUT);

  pinMode(VIBRATION_SENSOR_PIN, INPUT);

  pinMode(VOLTAGE_SENSOR_PIN, INPUT);
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

  int vibrationSensorValue = digitalRead(VIBRATION_SENSOR_PIN);

  if (vibrationSensorValue == HIGH) {
    Serial.println("Vibration detected!");
  } else {
    Serial.println("No vibration.");
  }

  // Read voltage from the voltage sensor
  int rawVoltage = analogRead(VOLTAGE_SENSOR_PIN);
  float measuredVoltage = rawVoltage * adcResolution * voltageDividerRatio + 0.6;
  Serial.println(String("Voltage = ") + measuredVoltage + " V");

  float temp; 
  int val11=rawVoltage; 
  temp=val11/4.092; 
  val11=(int)temp;// 
  int val2=((val11%100)/10); 
  Serial.println(val2); 

  delay(1000);  // Reduce signal read frequency
}
