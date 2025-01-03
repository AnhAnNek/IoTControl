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
    pinMode(_pins.ena, OUTPUT);
    pinMode(_pins.enb, OUTPUT);
}

// Method to set motor speeds for left and right motors
void MotorController::setMotorSpeeds(int leftSpeed, int rightSpeed) {
    analogWrite(_pins.ena, constrain(leftSpeed, 0, 255));
    analogWrite(_pins.enb, constrain(rightSpeed, 0, 255));
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
    analogWrite(_pins.ena, constrain(speed, 0, 255));
}

// Helper method to set the right motor's speed and direction
void MotorController::setRightMotor(int speed, bool direction) {
    digitalWrite(_pins.in3, direction);
    digitalWrite(_pins.in4, !direction);
    analogWrite(_pins.enb, constrain(speed, 0, 255));
}
