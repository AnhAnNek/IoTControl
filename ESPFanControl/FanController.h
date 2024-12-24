#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

#include <Arduino.h>

class FanController {
public:
  // Constructor to initialize pins
  FanController(int pwmPin, int pwmPin1);

  // Initialize fan pins
  void initialize();

  // Set fan speed
  void setFanSpeed(int speed);

private:
  int pwmPin;  // Primary fan control pin
  int pwmPin1; // Secondary fan control pin
};

#endif
