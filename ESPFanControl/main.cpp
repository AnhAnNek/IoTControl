#include <Arduino.h>
#include "SensorManager.h"
#include "FanController.h"
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "Constants.h"

// Global Variables
float currentTemp = 0.0;
int fanSpeed = 0;

// Temperature and fan speed configuration
float minTemp = 28;  // Minimum temperature for fan to start
float maxTemp = 32;  // Maximum temperature for full fan speed
int minFanSpeed = 0; // Minimum fan speed (PWM value)
int maxFanSpeed = 255; // Maximum fan speed (PWM value)

// Instantiate SensorManager and FanController
SensorManager sensorManager(TEMP_SENSOR_PIN, PHOTORESISTOR_PIN, INFRARED_SENSOR_PIN, SOUND_SENSOR_PIN);
FanController fanController(FAN_PWM_PIN, FAN_PWM_PIN_1);

// Custom WebSocket message handler
void customMessageHandler(const char* message) {
  Serial.println("Custom Handler: Received message from server:");
  Serial.println(message);

  // Parse JSON
  StaticJsonDocument<128> jsonDoc;
  if (deserializeJson(jsonDoc, message) == DeserializationError::Ok) {
    if (jsonDoc.containsKey("type")) {
      String type = jsonDoc["type"];
      if (type == "COMMAND_FAN") {
        if (jsonDoc.containsKey("fanSpeed")) {
          int newSpeed = jsonDoc["fanSpeed"];
          fanController.setFanSpeed(map(newSpeed, 0, 100, 0, 255));
          Serial.print("Set fan speed to: ");
          Serial.println(newSpeed);
        }
      }
    }
  }
}

// Function to initialize the system
void initializeSystem() {
  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFiManager& wifiManager = WiFiManager::getInstance();
  wifiManager.initializeWiFi(WIFI_SSID, WIFI_PASSWORD);

  // Initialize sensors
  sensorManager.initializeSensors();

  // Initialize fan controller
  fanController.initialize();

  // Initialize WebSocketManager
  WebSocketManager& webSocketManager = WebSocketManager::getInstance();
  webSocketManager.initialize(WEBSOCKET_SERVER_HOST, WEBSOCKET_SERVER_PORT);
  webSocketManager.setOnReceivedMessageCallback(customMessageHandler);
}

// Function to run the main system loop
void runSystem() {
  // Handle WebSocket messages
  WebSocketManager::getInstance().handleMessages();

  // Read sensors
  int lightLevel = sensorManager.readPhotoresistor();
  Serial.print("Photoresistor Value: ");
  Serial.println(lightLevel);

  int infraredValue = sensorManager.readInfraredSensor();
  Serial.print("Infrared Sensor Value: ");
  Serial.println(infraredValue);

  int soundValue = sensorManager.readSoundSensor();
  Serial.print("Sound Sensor Value: ");
  Serial.println(soundValue);

  currentTemp = sensorManager.getTemperature();
  if (currentTemp == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature!");
    return;
  }

  // Calculate fan speed
  if (currentTemp < minTemp) {
    fanSpeed = minFanSpeed;
  } else if (currentTemp >= minTemp && currentTemp <= maxTemp) {
    fanSpeed = map(currentTemp, minTemp, maxTemp, minFanSpeed, maxFanSpeed);
  } else {
    fanSpeed = maxFanSpeed;
  }

  // Ensure fanSpeed is at least 90% of maxFanSpeed
  int minimumAllowedSpeed = 0.9 * maxFanSpeed;
  if (fanSpeed > 0 && fanSpeed < minimumAllowedSpeed) {
    fanSpeed = minimumAllowedSpeed;
  }

  // Set fan speed
  fanController.setFanSpeed(fanSpeed);

  // Debug output
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.print(" °C, Fan Speed: ");
  Serial.println(map(fanSpeed, 0, 255, 0, 100));

  // Send data to WebSocket server
  WebSocketManager::getInstance().sendData(currentTemp, fanSpeed);

  delay(1000); // Update every second
}
