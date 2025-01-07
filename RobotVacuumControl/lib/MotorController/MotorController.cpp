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

    // Configure PWM channels for ena and enb
    ledcSetup(0, 10000, 8); // Channel 0, 10 kHz, 8-bit resolution
    ledcAttachPin(_pins.ena, 0);

    ledcSetup(1, 10000, 8); // Channel 1, 10 kHz, 8-bit resolution
    ledcAttachPin(_pins.enb, 1);

    // Configure PWM channels for side and main brushes
    ledcSetup(2, 20000, 8); // Channel 2 for side brush
    ledcAttachPin(_pins.sideBrush, 2);

    ledcSetup(3, 20000, 8); // Channel 3 for main brush
    ledcAttachPin(_pins.mainBrush, 3);
}

// Method to set motor speeds for left and right motors
void MotorController::setMotorSpeeds(int leftSpeed, int rightSpeed) {
    ledcWrite(0, constrain(leftSpeed, 0, 255)); // Channel 0 for ena
    ledcWrite(1, constrain(rightSpeed, 0, 255)); // Channel 1 for enb
}

// Method to set brush speeds
void MotorController::setBrushSpeeds(int sideBrushSpeed, int mainBrushSpeed) {
    ledcWrite(2, constrain(sideBrushSpeed, 0, 255)); // Channel 2 for side brush
    ledcWrite(3, constrain(mainBrushSpeed, 0, 255)); // Channel 3 for main brush
}

void MotorController::stopBrushes() {
    setBrushSpeeds(0, 0);
}

// Method to move the robot forward
void MotorController::moveForward() {
    setLeftMotor(HIGH, false);  // Forward for left motor
    setRightMotor(HIGH, false); // Forward for right motor
}

// Method to move the robot backward
void MotorController::moveBackward() {
    setLeftMotor(HIGH, true);   // Backward for left motor
    setRightMotor(HIGH, true);  // Backward for right motor
}

// Method to rotate the robot to the left
void MotorController::rotateLeft() {
    setLeftMotor(HIGH, true);   // Left motor backward
    setRightMotor(HIGH, false); // Right motor forward
}

// Method to rotate the robot to the right
void MotorController::rotateRight() {
    setLeftMotor(HIGH, false);  // Left motor forward
    setRightMotor(HIGH, true);  // Right motor backward
}

// Method to stop both motors
void MotorController::stop() {
    setLeftMotor(LOW, false);   // Stop left motor
    setRightMotor(LOW, false);  // Stop right motor
}

// Helper method to set the left motor's speed and direction
void MotorController::setLeftMotor(int speed, bool direction) {
    digitalWrite(_pins.in1, direction);
    digitalWrite(_pins.in2, !direction);
    ledcWrite(0, constrain(speed, 0, 255)); // Channel 0 for ena
}

// Helper method to set the right motor's speed and direction
void MotorController::setRightMotor(int speed, bool direction) {
    digitalWrite(_pins.in3, direction);
    digitalWrite(_pins.in4, !direction);
    ledcWrite(1, constrain(speed, 0, 255)); // Channel 1 for enb
}