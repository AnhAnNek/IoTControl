#include "WebSocketManager.h"
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "FanController.h"
#include <WiFi.h>
#include <Constants.h>
#include "Configuration.h"
#include "Utils.h"

// WebSocket configuration
const char* websockets_server_host = "192.168.31.2";
const uint16_t websockets_server_port = 8000;
websockets::WebsocketsClient client;

// NTP Client setup for Vietnam (UTC+7)
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000);

// Function to initialize WebSocket connection
void initializeWebSocket() {
  Serial.print("Connecting to WebSocket server...");
  if (client.connect(websockets_server_host, websockets_server_port, "/ws")) {
    Serial.println("Connected to WebSocket server!");
    client.send("Hello Server");
  } else {
    Serial.println("Failed to connect to WebSocket server!");
  }

  // Set up message handler
  client.onMessage([](websockets::WebsocketsMessage message) {
    handleServerMessage(message.data().c_str()); // Ensure handleServerMessage is declared
  });


  // timeClient.begin();
  // timeClient.update();
}

// Function to handle WebSocket messages
void handleServerMessage(const char* message) {
  StaticJsonDocument<128> jsonDoc;

  if (deserializeJson(jsonDoc, message)) {
    Serial.println("Error parsing JSON message");
    return;
  }

if (jsonDoc.containsKey("type")) {
    String type = jsonDoc["type"];

    // Handle message types
    if (type == COMMAND_FAN) {
      if (jsonDoc.containsKey("fanSpeed")) {
        int newSpeed = jsonDoc["fanSpeed"];
        Serial.print("Setting fan speed to: ");
        Serial.println(newSpeed);
        setFanSpeed(map(newSpeed, 0, 100, 0, 255));
      }
    } else if (type == SETTING_TEMP) {
      if (jsonDoc.containsKey("minTemp")) minTemp = jsonDoc["minTemp"];
      if (jsonDoc.containsKey("maxTemp")) maxTemp = jsonDoc["maxTemp"];
      if (jsonDoc.containsKey("minFanSpeed")) minFanSpeed = jsonDoc["minFanSpeed"];
      if (jsonDoc.containsKey("maxFanSpeed")) maxFanSpeed = jsonDoc["maxFanSpeed"];

      Serial.println("Temperature and fan settings updated:");
      Serial.print("minTemp: ");
      Serial.println(minTemp);
      Serial.print("maxTemp: ");
      Serial.println(maxTemp);
      Serial.print("minFanSpeed: ");
      Serial.println(minFanSpeed);
      Serial.print("maxFanSpeed: ");
      Serial.println(maxFanSpeed);
    } else if (type == INFO) {
      Serial.println(message);
    } else {
      Serial.println("Unknown message type received.");
    }
  } else {
    Serial.println("No type field in message.");
  }
}

// Function to poll WebSocket for messages
void handleWebSocketMessages() {
  if (client.available()) {
    client.poll();
  }
}

// Function to send data to WebSocket server
void sendToWebSocket(float temp, int fanSpeed) {
  if (!client.available()) {
    Serial.println("WebSocket client not available, attempting to reconnect...");
    if (!client.connect(websockets_server_host, websockets_server_port, "/ws")) {
      Serial.println("Failed to reconnect to WebSocket server.");
      return;
    }
  }

  // Get current ISO 8601 timestamp
  // String currentDateTime = getISO8601Timestamp(timeClient);

  StaticJsonDocument<128> jsonDoc;
  jsonDoc["type"] = "INFO";
  jsonDoc["temp"] = temp;
  jsonDoc["fanSpeed"] = map(fanSpeed, 0, 255, 0, 100);
  // jsonDoc["datetime"] = currentDateTime;

  String jsonString;
  serializeJson(jsonDoc, jsonString);
  client.send(jsonString);

  Serial.print("Sent to WebSocket: ");
  Serial.println(jsonString);
}
