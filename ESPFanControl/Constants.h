#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

// Wi-Fi credentials
constexpr const char* WIFI_SSID = "Anh An";
constexpr const char* WIFI_PASSWORD = "qwertyuiop";

// WebSocket server details
constexpr const char* WEBSOCKET_SERVER_HOST = "192.168.31.2";
constexpr uint16_t WEBSOCKET_SERVER_PORT = 8000;

// Sensor pins
constexpr int TEMP_SENSOR_PIN = 26;       // DS18B20 DATA pin
constexpr int SOUND_SENSOR_PIN = 16;     // Sound sensor pin

constexpr int DHT_PIN = 25;

// Relay Pins
constexpr int RELAY1_PIN = 23;           // Relay 1 pin
constexpr int RELAY2_PIN = 22;           // Relay 2 pin
constexpr int RELAY3_PIN = 21;           // Relay 3 pin
constexpr int RELAY4_PIN = 19;           // Relay 4 pin

// Other constants (add more as needed)
constexpr const char* MESSAGE_TYPE_INFO = "INFO";
constexpr const char* COMMAND_TURN_ON_RELAY = "COMMAND_TURN_ON_RELAY";
constexpr const char* COMMAND_TURN_OFF_RELAY = "COMMAND_TURN_OFF_RELAY";
constexpr const char* COMMAND_SET_RELAY_TEMP = "COMMAND_SET_RELAY_TEMP";

#endif