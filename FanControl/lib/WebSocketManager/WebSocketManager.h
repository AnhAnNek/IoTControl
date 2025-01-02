#ifndef WEBSOCKET_MANAGER_H
#define WEBSOCKET_MANAGER_H

#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <functional> // For std::function

class WebSocketManager {
public:
  // Get the singleton instance
  static WebSocketManager& getInstance();

  // Delete copy constructor and assignment operator to enforce singleton
  WebSocketManager(const WebSocketManager&) = delete;
  WebSocketManager& operator=(const WebSocketManager&) = delete;

  // Initialize WebSocket connection
  void initialize(const char* serverHost, uint16_t serverPort);

  // Handle incoming WebSocket messages
  void handleMessages();

  void send(const char* jsonText);

  // Set the callback for received messages
  void setOnReceivedMessageCallback(std::function<void(const char*)> callback);

private:
  // Private constructor for singleton
  WebSocketManager();

  // WebSocket configuration
  const char* serverHost;
  uint16_t serverPort;
  websockets::WebsocketsClient client;

  // Callback for received messages
  std::function<void(const char*)> onReceivedMessageCallback;

  // Internal function to handle a single WebSocket message
  void handleMessage(const char* message);
};

#endif
