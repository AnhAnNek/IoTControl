#include "WebSocketManager.h"
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "FanController.h"
#include <WiFi.h>

// WebSocket configuration
const char* websockets_server_host = "192.168.31.2";
const uint16_t websockets_server_port = 8000;
websockets::WebsocketsClient client;

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
}

// Function to handle WebSocket messages
void handleServerMessage(const char* message) {
  StaticJsonDocument<128> jsonDoc;

  if (deserializeJson(jsonDoc, message)) {
    Serial.println("Error parsing JSON message");
    return;
  }

  // Process fan speed command
  if (jsonDoc.containsKey("setFanSpeed")) {
    int newSpeed = jsonDoc["setFanSpeed"];
    Serial.print("Setting new fan speed to: ");
    Serial.println(newSpeed);
    setFanSpeed(map(newSpeed, 0, 100, 0, 255)); // Use setFanSpeed from FanController
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

  StaticJsonDocument<128> jsonDoc;
  jsonDoc["type"] = "INFO";
  jsonDoc["temp"] = temp;
  jsonDoc["fanSpeed"] = map(fanSpeed, 0, 255, 0, 100);

  String jsonString;
  serializeJson(jsonDoc, jsonString);
  client.send(jsonString);

  Serial.print("Sent to WebSocket: ");
  Serial.println(jsonString);
}
