#include "WebSocketManager.h"

// Initialize static instance
WebSocketManager& WebSocketManager::getInstance() {
  static WebSocketManager instance;
  return instance;
}

// Private constructor
WebSocketManager::WebSocketManager() : serverHost(nullptr), serverPort(0) {}

// Initialize WebSocket connection
void WebSocketManager::initialize(const char* serverHost, uint16_t serverPort) {
  this->serverHost = serverHost;
  this->serverPort = serverPort;

  Serial.print("Connecting to WebSocket server...");
  if (client.connect(serverHost, serverPort, "/ws")) {
    Serial.println("Connected to WebSocket server!");
    client.send("Hello Server");
  } else {
    Serial.println("Failed to connect to WebSocket server!");
  }

  // Set up message handler
  client.onMessage([this](websockets::WebsocketsMessage message) {
    handleMessage(message.data().c_str());
  });
}

// Handle a single WebSocket message
void WebSocketManager::handleMessage(const char* message) {
  if (onReceivedMessageCallback) {
    onReceivedMessageCallback(message); // Call the user-defined callback
  } else {
    Serial.println("No callback set for received messages.");
  }
}

// Handle incoming WebSocket messages
void WebSocketManager::handleMessages() {
  if (client.available()) {
    client.poll();
  }
}

// Send data to WebSocket server
void WebSocketManager::sendData(float temp, int fanSpeed) {
  if (!client.available()) {
    Serial.println("WebSocket client not available, attempting to reconnect...");
    if (!client.connect(serverHost, serverPort, "/ws")) {
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

// Set the callback for received messages
void WebSocketManager::setOnReceivedMessageCallback(std::function<void(const char*)> callback) {
  onReceivedMessageCallback = callback;
}
