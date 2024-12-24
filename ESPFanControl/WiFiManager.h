#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <Arduino.h>

class WiFiManager {
public:
  // Get the singleton instance
  static WiFiManager& getInstance();

  // Delete copy constructor and assignment operator to enforce singleton
  WiFiManager(const WiFiManager&) = delete;
  WiFiManager& operator=(const WiFiManager&) = delete;

  // Initialize Wi-Fi connection
  void initializeWiFi(const char* ssid, const char* password);

  // Get the Wi-Fi connection status
  bool isConnected() const;

private:
  // Private constructor for singleton
  WiFiManager();

  // Wi-Fi credentials
  const char* ssid;
  const char* password;
};

#endif
