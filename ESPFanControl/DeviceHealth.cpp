#include "DeviceHealth.h"

DeviceHealth::DeviceHealth() {
    // Initialization if needed
}

String DeviceHealth::checkHealth() {
    // Create a JSON document
    StaticJsonDocument<1024> jsonDoc; // Adjust size for larger payload
    JsonArray deviceList = jsonDoc.createNestedArray("devices");

    // Scan for I2C devices
    scanI2CDevices(deviceList);

    // Scan all GPIO sensors
    scanGPIOSensors(deviceList);

    // Serialize JSON to a string
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    return jsonString;
}

void DeviceHealth::scanI2CDevices(JsonArray& deviceList) {
    Wire.begin();
    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) { // Device responded
            JsonObject device = deviceList.createNestedObject();
            device["type"] = "I2C";
            device["address"] = address;
            device["name"] = "I2C Device"; // Placeholder, add specific logic if needed
            device["status"] = checkI2CHealth(address) ? "Healthy" : "Unresponsive";
        }
    }
}

bool DeviceHealth::checkI2CHealth(uint8_t address) {
    // Placeholder for device-specific health check
    // Send a command and expect a response to verify health
    return true; // Assume healthy for demonstration
}

void DeviceHealth::scanGPIOSensors(JsonArray& deviceList) {
    // List of GPIO pins to scan (specific to your ESP32 model)
    int pins[] = {2, 4, 5, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};
    for (int pin : pins) {
        pinMode(pin, INPUT);
        JsonObject device = deviceList.createNestedObject();
        device["type"] = "GPIO";
        device["pin"] = pin;
        device["name"] = identifyDeviceOnPin(pin);
        device["status"] = checkGPIOHealth(pin) ? "Healthy" : "Unresponsive";
    }
}

bool DeviceHealth::checkGPIOHealth(uint8_t pin) {
    // Example: Simulate health check by reading pin state
    return digitalRead(pin) == HIGH; // Modify based on your sensor logic
}

String DeviceHealth::identifyDeviceOnPin(uint8_t pin) {
    // Placeholder logic to identify device type based on pin
    // Customize this based on your hardware configuration
    if (pin == 2) {
        return "LED";
    } else if (pin == 12 || pin == 13) {
        return "Temperature Sensor";
    } else if (pin == 14 || pin == 15) {
        return "Button";
    }
    return "Unknown Device"; // Default for unconfigured pins
}
