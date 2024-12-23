#include "FanController.h"
#include <Arduino.h>

// Pin Configuration
#define FAN_PWM_PIN 16 // GPIO for fan speed control

void initializeFan() {
  pinMode(FAN_PWM_PIN, OUTPUT);
}

void setFanSpeed(int speed) {
  analogWrite(FAN_PWM_PIN, speed);
}
