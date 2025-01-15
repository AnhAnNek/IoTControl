#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <Arduino.h>
#include "Utils.h"

// Define a structure to hold pin configuration for the sensors
struct SensorPins {
    unsigned short frontTrigPin;
    unsigned short frontEchoPin;
    unsigned short leftTrigPin;
    unsigned short leftEchoPin;
    unsigned short rightTrigPin;
    unsigned short rightEchoPin;
    unsigned short irObstaclePin; // Pin for LM393 IR obstacle sensor
    unsigned short irReceiverPin; // Pin for 38kHz IR receiver
};

class SensorManager {
public:
    // Constructor to initialize sensor pins
    SensorManager(SensorPins pins);

    // Method to initialize the sensors
    void begin();

    long getFrontObstacleDistance();
    long getLeftObstacleDistance();
    long getRightObstacleDistance();

    // Method to check for proximity to stairs
    bool isNearStairs();

    // Method to check for signal from 38kHz IR transmitter
    bool isSignalFromBase();

private:
    SensorPins _pins; // Struct to store sensor pins

    // Helper method to read ultrasonic sensor distance
    long readUltrasonic(unsigned short trigPin, unsigned short echoPin);
};

#endif // SENSORMANAGER_H