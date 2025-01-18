#include "MotorController.h"

// Constructor to initialize motor control pins
MotorController::MotorController(MotorPins pins) {
    _pins = pins; // Store the passed motor pins configuration
}

// Method to initialize motor control pins
void MotorController::begin() {
    pinMode(_pins.in1, OUTPUT);
    pinMode(_pins.in2, OUTPUT);
    pinMode(_pins.in3, OUTPUT);
    pinMode(_pins.in4, OUTPUT);

    // // Configure PWM channels for ena and enb
    ledcSetup(0, 15000, 8); // Channel 0, 5 kHz, 8-bit resolution
    ledcAttachPin(_pins.ena, 0);

    ledcSetup(1, 15000, 8); // Channel 1, 5 kHz, 8-bit resolution
    ledcAttachPin(_pins.enb, 1);

    // Configure PWM channels for side and main brushes
    ledcSetup(2, 20000, 8); // Channel 2 for side brush
    ledcAttachPin(_pins.sideBrush, 2);

    ledcSetup(3, 20000, 8); // Channel 3 for main brush
    ledcAttachPin(_pins.mainBrush, 3);
}

// Method to set motor speeds for left and right motors
void MotorController::setMovingSpeeds(int leftSpeed, int rightSpeed) {
    ledcWrite(0, constrain(leftSpeed, 0, 255)); // Channel 0 for ena
    ledcWrite(1, constrain(rightSpeed, 0, 255)); // Channel 1 for enb
}

// Method to set brush speeds
void MotorController::setBrushSpeeds(int sideBrushSpeed, int mainBrushSpeed) {
    setSideBrushSpeed(sideBrushSpeed);
    setMainBrushSpeed(mainBrushSpeed);
}

void MotorController::stopBrushes() 
{
    setSideBrushSpeed(0);
    setMainBrushSpeed(0);
}

// Method to move the robot forward
void MotorController::moveForward() 
{
    setLeftMotorDirection(true);  // Forward for left motor
    setRightMotorDirection(true); // Forward for right motor
}

// Method to move the robot backward
void MotorController::moveBackward() 
{
    setLeftMotorDirection(false);   // Backward for left motor
    setRightMotorDirection(false);  // Backward for right motor
}

// Method to rotate the robot to the left
void MotorController::rotateLeft() 
{
    setLeftMotorDirection(false);   // Left motor backward
    setRightMotorDirection(true); // Right motor forward
}

// Method to rotate the robot to the right
void MotorController::rotateRight() 
{
    setLeftMotorDirection(true);  // Left motor forward
    setRightMotorDirection(false);  // Right motor backward
}

// Method to stop both motors
void MotorController::stop() 
{
    setMovingSpeeds(0, 0);
    digitalWrite(_pins.in1, LOW);
    digitalWrite(_pins.in2, LOW);
    digitalWrite(_pins.in3, LOW);
    digitalWrite(_pins.in4, LOW);
}

// Helper method to set the left motor's speed and direction
void MotorController::setLeftMotorDirection(bool direction) {
    digitalWrite(_pins.in1, direction);
    digitalWrite(_pins.in2, !direction);
}

// Helper method to set the right motor's speed and direction
void MotorController::setRightMotorDirection(bool direction) {
    digitalWrite(_pins.in3, direction);
    digitalWrite(_pins.in4, !direction);
}

void MotorController::setSideBrushSpeed(int speed)
{
    ledcWrite(2, constrain(speed, 0, 255)); // Channel 2 for side brush
}

void MotorController::setMainBrushSpeed(int speed)
{
    ledcWrite(3, constrain(speed, 0, 255)); // Channel 3 for main brush
}
