#include "WiFiManager.h"

// Initialize static instance
WiFiManager& WiFiManager::getInstance() {
  static WiFiManager instance;
  return instance;
}

// Private constructor
WiFiManager::WiFiManager() : ssid(nullptr), password(nullptr) {}

// Initialize Wi-Fi connection
void WiFiManager::initializeWiFi(const char* ssid, const char* password) {
  this->ssid = ssid;
  this->password = password;

  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  // Wait for connection
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
    Serial.print(".");
    delay(1000);
  }

  // Check connection status
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi.");
  }
}

// Get the Wi-Fi connection status
bool WiFiManager::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}
