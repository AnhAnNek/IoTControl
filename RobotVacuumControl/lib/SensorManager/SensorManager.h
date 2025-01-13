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
    int irObstaclePin; // Pin for LM393 IR obstacle sensor
    int irReceiverPin; // Pin for 38kHz IR receiver
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

    // Method to check for signal from 38kHz IR transmitter
    bool isSignalFromBase();

private:
    SensorPins _pins; // Struct to store sensor pins

    // Helper method to read ultrasonic sensor distance
    long readUltrasonic(int trigPin, int echoPin);
};

#endif // SENSORMANAGER_H