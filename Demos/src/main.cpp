#include <Arduino.h>
#include "ACS712.h"

#define RADAR_PIN 5  // Connect OUT of RCWL-0516 to GPIO 5

ACS712 sensor(ACS712_30A, 34);

#define IR_SENSOR_PIN 21  // Connect OUT of HW-488 to GPIO 21

const int motorPin = 13;
const int pwmFreq = 20000;        // Tần số PWM: 5 kHz
const int pwmChannel = 0;        // Kênh PWM
const int pwmResolution = 8;     // Độ phân giải 8 bit (0-255)
const int maxSpeed = 255;        // Maximum speed

void setup() {
  Serial.begin(115200);

  Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
  int zero = sensor.calibrate();
  Serial.println("Done!");
  Serial.println("Zero point for this sensor = " + zero);

  pinMode(RADAR_PIN, INPUT);

  pinMode(IR_SENSOR_PIN, INPUT);

  // Thiết lập kênh PWM
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  ledcAttachPin(motorPin, pwmChannel);
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

  // Gradually increase speed
  for (int speed = 0; speed <= maxSpeed; speed++) {
    ledcWrite(pwmChannel, speed);
    delay(20);  // Adjust to control speed ramp-up time
  }

  delay(2000);  // Maintain full speed for 2 seconds

  // Gradually decrease speed
  for (int speed = maxSpeed; speed >= 0; speed--) {
    ledcWrite(pwmChannel, speed);
    delay(20);
  }

  delay(2000);
  
  delay(1000);  // Reduce signal read frequency
}
