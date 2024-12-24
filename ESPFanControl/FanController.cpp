#include "FanController.h"

// Constructor to initialize pins
FanController::FanController(int pwmPin, int pwmPin1) : pwmPin(pwmPin), pwmPin1(pwmPin1) {}

// Initialize fan pins
void FanController::initialize() {
  pinMode(pwmPin, OUTPUT);
  pinMode(pwmPin1, OUTPUT);
}

// Set fan speed
void FanController::setFanSpeed(int speed) {
  analogWrite(pwmPin, speed);
  analogWrite(pwmPin1, speed);
}
