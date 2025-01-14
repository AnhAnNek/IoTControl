#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include "Utils.h"

// Define a structure to hold motor control pin configuration
struct MotorPins {
    unsigned short in1;
    unsigned short in2;
    unsigned short in3;
    unsigned short in4;
    unsigned short ena;
    unsigned short enb;
    unsigned short sideBrush;  // PWM pin for side brush
    unsigned short mainBrush;  // PWM pin for main brush
};

class MotorController {
public:
    // Constructor to initialize motor control pins
    MotorController(MotorPins pins);
    
    // Method to initialize the motors
    void begin();

    // Methods to control motor movements
    void setMovingSpeeds(int leftSpeed, int rightSpeed);
    void setBrushSpeeds(int sideBrushSpeed, int mainBrushSpeed);
    void stopBrushes();
    void moveForward();
    void moveBackward();
    void rotateLeft();
    void rotateRight();
    void stop();
    void setSideBrushSpeed(int speed);
    void setMainBrushSpeed(int speed);

private:
    MotorPins _pins;  // Struct to store motor control pins

    // Helper methods to set individual motor speeds and directions
    void setLeftMotorDirection(bool direction);
    void setRightMotorDirection(bool direction);
};

#endif // MOTORCONTROLLER_H
