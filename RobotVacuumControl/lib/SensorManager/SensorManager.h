#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <Arduino.h>
#include "Constants.h"

// Define a structure to hold pin configuration for the sensors
struct SensorPins {
    int frontTrigPin;
    int frontEchoPin;
    int leftTrigPin;
    int leftEchoPin;
    int rightTrigPin;
    int rightEchoPin;
    int irPin;
};

class SensorManager {
public:
    // Constructor to initialize sensor pins
    SensorManager(SensorPins pins);

    // Method to initialize the sensors
    void begin();

    // Methods to check for obstacles
    bool isObstacleFront();
    bool isObstacleLeft();
    bool isObstacleRight();

    // Method to check for proximity to stairs
    bool isNearStairs();

private:
    SensorPins _pins;  // Struct to store sensor pins

    // Helper method to read ultrasonic sensor distance
    long readUltrasonic(int trigPin, int echoPin);
};

#endif // SENSORMANAGER_H
