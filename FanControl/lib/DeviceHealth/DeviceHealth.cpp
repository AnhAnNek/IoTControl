#include "DeviceHealth.h"

// Constructor
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
    return true; // Assume healthy for demonstration
}

void DeviceHealth::scanGPIOSensors(JsonArray& deviceList) {
    int pins[] = {TEMP_SENSOR_PIN, DHT_PIN, LED_PIN, RELAY1_PIN, RELAY2_PIN, RELAY3_PIN, RELAY4_PIN};
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
    // Map pins to specific devices using constants
    if (pin == TEMP_SENSOR_PIN) {
        return "Temperature Sensor (DS18B20)";
    } else if (pin == DHT_PIN) {
        return "DHT Sensor (Temp And Humality)";
    } else if (pin == LED_PIN) {
        return "LED";
    } else if (pin == RELAY1_PIN) {
        return "Relay 1";
    } else if (pin == RELAY2_PIN) {
        return "Relay 2";
    } else if (pin == RELAY3_PIN) {
        return "Relay 3";
    } else if (pin == RELAY4_PIN) {
        return "Relay 4";
    }
    return "Unknown Device";
}
