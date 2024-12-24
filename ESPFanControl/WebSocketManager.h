#ifndef WEBSOCKET_MANAGER_H
#define WEBSOCKET_MANAGER_H

#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "FanController.h"
#include <WiFi.h>
#include <Constants.h>
#include "Configuration.h"
#include "Utils.h"

// WebSocket configuration
extern const char* websockets_server_host;
extern const uint16_t websockets_server_port;
extern websockets::WebsocketsClient client;

void initializeWebSocket();
void handleWebSocketMessages();
void handleServerMessage(const char* message);
void sendToWebSocket(float temp, int fanSpeed);

#endif
