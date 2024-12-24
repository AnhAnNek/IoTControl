#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include "SensorManager.h"
#include "FanController.h"
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "Configuration.h"

// Global Variables
float currentTemp = 0.0;
int fanSpeed = 0; // PWM value (0-255)

float minTemp = 28;  // Minimum temperature for fan to start
float maxTemp = 32;  // Maximum temperature for full fan speed
int minFanSpeed = 0; // Minimum fan speed (PWM value)
int maxFanSpeed = 255; // Maximum fan speed (PWM value)

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFiManager& wifiManager = WiFiManager::getInstance();
  wifiManager.initializeWiFi("Anh An", "qwertyuiop");

  // Initialize peripherals
  initializeSensors();
  initializeFan();
  initializeWebSocket();
}

void loop() {
  // Check for WebSocket messages
  handleWebSocketMessages();
  
  // Read photoresistor value
  int lightLevel = readPhotoresistor();
  Serial.print("Photoresistor Value: ");
  Serial.println(lightLevel);

  // Read Infrared sensor value
  int infraredValue = readInfraredSensor();
  Serial.print("Infrared sensor Value: ");
  Serial.println(infraredValue);

  // Read Sound sensor value
  int soundValue = readSoundSensor();
  Serial.print("Sound sensor Value: ");
  Serial.println(soundValue);

  // Read temperature
  currentTemp = getTemperature();
  if (currentTemp == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature!");
    return;
  }

  if (currentTemp < minTemp) {
    fanSpeed = minFanSpeed; // Turn off fan
  } else if (currentTemp >= minTemp && currentTemp <= maxTemp) {
    fanSpeed = map(currentTemp, minTemp, maxTemp, minFanSpeed, maxFanSpeed);
  } else {
    fanSpeed = maxFanSpeed; // Full speed
  }

  // Set fan speed
  setFanSpeed(fanSpeed);

  // Debug output
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.print(" °C, Fan Speed: ");
  Serial.println(map(fanSpeed, 0, 255, 0, 100)); // Display fan speed as percentage

  // Send data to WebSocket server in JSON format
  sendToWebSocket(currentTemp, fanSpeed);

  delay(1000); // Update every second
}