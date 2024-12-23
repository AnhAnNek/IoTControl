#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>
#include "SensorManager.h"
#include "FanController.h"
#include "WebSocketManager.h"
#include "WiFiManager.h"

// Pin Configuration
#define PHOTORESISTOR_PIN 17

// Global Variables
float currentTemp = 0.0;
int fanSpeed = 0; // PWM value (0-255)

void setup() {
  Serial.begin(115200);

  // Initialize Wi-Fi
  initializeWiFi();

  // Initialize peripherals
  initializeSensors();
  initializeFan();
  initializeWebSocket();
  pinMode(PHOTORESISTOR_PIN, OUTPUT);
}

void loop() {
  // Check for WebSocket messages
  handleWebSocketMessages();

  // Read temperature
  currentTemp = getTemperature();
  if (currentTemp == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature!");
    return;
  }

  // Map temperature to fan speed (example: 30°C = 50%, 60°C = 100%)
  if (currentTemp < 28) {
    fanSpeed = 0; // Turn off fan
  } else if (currentTemp > 28 && currentTemp <= 32) {
    fanSpeed = map(currentTemp, 30, 60, 128, 255); // Gradual increase
  } else {
    fanSpeed = 255; // Full speed
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