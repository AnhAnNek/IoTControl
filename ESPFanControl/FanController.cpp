#include "FanController.h"
#include <Arduino.h>

// Pin Configuration
#define FAN_PWM_PIN 16 // GPIO for fan speed control (L298)
#define FAN_PWM_PIN_1 5 // GPIO for fan speed control (MOS)

void initializeFan() {
  pinMode(FAN_PWM_PIN, OUTPUT);
  pinMode(FAN_PWM_PIN_1, OUTPUT);
}

void setFanSpeed(int speed) {
  analogWrite(FAN_PWM_PIN, speed);
  analogWrite(FAN_PWM_PIN_1, speed);
}
