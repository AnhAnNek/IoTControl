#include "SensorManager.h"

// Constructor to initialize sensor pins
SensorManager::SensorManager(SensorPins pins) {
    _pins = pins; // Store the passed pins
}

// Method to initialize the sensors
void SensorManager::begin() {
    // Initialize pins for IR sensor and ultrasonic sensors
    pinMode(_pins.irPin, INPUT);
    pinMode(_pins.frontTrigPin, OUTPUT);
    pinMode(_pins.frontEchoPin, INPUT);
    pinMode(_pins.leftTrigPin, OUTPUT);
    pinMode(_pins.leftEchoPin, INPUT);
    pinMode(_pins.rightTrigPin, OUTPUT);
    pinMode(_pins.rightEchoPin, INPUT);
}

bool SensorManager::isObstacleFront() {
    long distance = readUltrasonic(_pins.frontTrigPin, _pins.frontEchoPin);  // Get the distance from the front sensor
    Serial.print("Front Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance < OBSTACLE_THRESHOLD_CM;  // Return true if obstacle is within threshold
}

bool SensorManager::isObstacleLeft() {
    long distance = readUltrasonic(_pins.leftTrigPin, _pins.leftEchoPin);  // Get the distance from the left sensor
    Serial.print("Left Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance < OBSTACLE_THRESHOLD_CM;  // Return true if obstacle is within threshold
}

bool SensorManager::isObstacleRight() {
    long distance = readUltrasonic(_pins.rightTrigPin, _pins.rightEchoPin);  // Get the distance from the right sensor
    Serial.print("Right Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance < OBSTACLE_THRESHOLD_CM;  // Return true if obstacle is within threshold
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
long SensorManager::readUltrasonic(int trigPin, int echoPin) {
    // Send a pulse to trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the duration of the pulse that comes back
    long duration = pulseIn(echoPin, HIGH, 38000); // Timeout at ~400cm (38ms)

    // Check for out-of-range conditions
    if (duration == 0) {
        Serial.println("No echo detected (object too close or out of range).");
        return -1; // Indicates out of range
    }

    // Calculate the distance in cm
    long distance = duration * 0.0344 / 2; // Speed of sound is 0.0344 cm/us

    if (distance < 2) {
        Serial.println("Object too close (< 2 cm).");
        return -1; // Indicates out of range
    } else if (distance > 400) {
        Serial.println("Object too far (> 400 cm).");
        return -1; // Indicates out of range
    }
    return distance;
}
