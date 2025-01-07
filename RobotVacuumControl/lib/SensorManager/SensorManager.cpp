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
    delayMicroseconds(2); // Ensure a clean LOW pulse
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Send a 10µs HIGH pulse
    digitalWrite(trigPin, LOW);

    // Measure the duration of the echo pulse
    long duration = pulseIn(echoPin, HIGH);

    // Calculate the distance in cm
    long distance = duration * 0.034 / 2;

    return distance; // Return valid distance
}
