#include <Arduino.h>
#include "SensorManager.h"
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "Constants.h"
#include "RelayControl.h"

// Global Variables
float currentTemp = 0.0;
int fanSpeed = 0;

// Temperature and fan speed configuration
float minTemp = 28;  // Minimum temperature for fan to start
float maxTemp = 32;  // Maximum temperature for full fan speed
int minFanSpeed = 0; // Minimum fan speed (PWM value)
int maxFanSpeed = 255; // Maximum fan speed (PWM value)

SensorManager sensorManager(TEMP_SENSOR_PIN, PHOTORESISTOR_PIN, INFRARED_SENSOR_PIN, SOUND_SENSOR_PIN);

RelayControl relay1(23, LOW);
RelayControl relay2(21, LOW);
RelayControl relay3(19, LOW);
RelayControl relay4(18, LOW);

// Message handlers for specific destinations
void handleGreetings(const String& message) {
    Serial.println("Greetings received: " + message);
}

void handleAlerts(const String& message) {
    Serial.println("Alert received: " + message);
}

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

  // Initialize WebSocketManager
  WebSocketManager& webSocketManager = WebSocketManager::getInstance();
  webSocketManager.initialize(WEBSOCKET_SERVER_HOST, WEBSOCKET_SERVER_PORT);
  webSocketManager.setOnReceivedMessageCallback(customMessageHandler);

  relay1.begin();
  relay2.begin();
  relay3.begin();
  relay4.begin();
  
  relay1.setAutoMode(true, 22, 4);
  relay2.setAutoMode(true, 24, 4);
  relay3.setAutoMode(true, 26, 4);
  relay4.setAutoMode(true, 28, 4);
  Serial.println("Relay initialized");
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
  if (currentTemp < minTemp) 
  {
    fanSpeed = minFanSpeed;
  } 
  else 
  {
    fanSpeed = maxFanSpeed;
  }

  // Debug output
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.print(" °C, Fan Speed: ");
  Serial.println(map(fanSpeed, 0, 255, 0, 100));

  relay1.update(currentTemp);
  relay2.update(currentTemp);
  relay3.update(currentTemp);
  relay4.update(currentTemp);

  // Send data to WebSocket server
  WebSocketManager::getInstance().sendData(currentTemp, fanSpeed);

  delay(1000); // Update every second
}
