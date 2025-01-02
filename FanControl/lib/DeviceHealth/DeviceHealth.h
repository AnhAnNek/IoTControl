#ifndef DEVICEHEALTH_H
#define DEVICEHEALTH_H

#include <Arduino.h>
#include <Wire.h>       // For I2C communication
#include <ArduinoJson.h> // JSON library
#include "Constants.h" // Include the constants defined in MessageTypes.h

class DeviceHealth {
public:
    // Constructor
    DeviceHealth();

    // Method to scan devices and return their health status as a JSON string
    String checkHealth();

private:
    void scanI2CDevices(JsonArray& deviceList);
    bool checkI2CHealth(uint8_t address);
    void scanGPIOSensors(JsonArray& deviceList);
    bool checkGPIOHealth(uint8_t pin);
    String identifyDeviceOnPin(uint8_t pin);
};

#endif
