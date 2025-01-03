#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include "Constants.h"

// Define a structure to hold motor control pin configuration
struct MotorPins {
    int in1;
    int in2;
    int in3;
    int in4;
    int ena;
    int enb;
};

class MotorController {
public:
    // Constructor to initialize motor control pins
    MotorController(MotorPins pins);
    
    // Method to initialize the motors
    void begin();

    // Methods to control motor movements
    void setMotorSpeeds(int leftSpeed, int rightSpeed);
    void moveForward();
    void moveBackward();
    void rotateLeft();
    void rotateRight();
    void stop();

private:
    MotorPins _pins;  // Struct to store motor control pins

    // Helper methods to set individual motor speeds and directions
    void setLeftMotor(int speed, bool direction);
    void setRightMotor(int speed, bool direction);
};

#endif // MOTORCONTROLLER_H
