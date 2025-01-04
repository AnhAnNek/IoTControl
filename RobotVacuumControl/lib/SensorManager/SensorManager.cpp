#include "SensorManager.h"

// Constructor to initialize sensor pins
SensorManager::SensorManager(SensorPins pins) {
    _pins = pins; // Store the passed pins
}

// Method to initialize the sensors
void SensorManager::begin() {
    // Initialize pins for IR sensor and ultrasonic sensors
    pinMode(_pins.irPin, INPUT);
    pinMode(_pins.frontPin, INPUT);
    pinMode(_pins.leftPin, INPUT);
    pinMode(_pins.rightPin, INPUT);
}

// Methods to check for obstacles based on ultrasonic sensor readings
bool SensorManager::isObstacleFront() {
    return readUltrasonic(_pins.frontPin) < OBSTACLE_THRESHOLD_CM;
}

bool SensorManager::isObstacleLeft() {
    return readUltrasonic(_pins.leftPin) < OBSTACLE_THRESHOLD_CM;
}

bool SensorManager::isObstacleRight() {
    return readUltrasonic(_pins.rightPin) < OBSTACLE_THRESHOLD_CM;
}

// Method to check for proximity to stairs using the IR sensor
bool SensorManager::isNearStairs() {
    int value = digitalRead(_pins.irPin);

    if (value == -1) { // digitalRead normally doesn't return -1, so you can simulate this
        Serial.println("Sensor not available. Returning true.");
        return true; // Fail-safe: assume proximity to stairs
    }

    Serial.print("IR Read: ");
    Serial.println(value);
    Serial.println();
    return value == HIGH;
}

// Helper method to read ultrasonic sensor
int SensorManager::readUltrasonic(int pin) {
    // Simulate reading ultrasonic sensor (use actual sensor code here)
    return random(20, 50); // Simulated distance in cm
}
