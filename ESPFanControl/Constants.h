#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

// Wi-Fi credentials
constexpr const char* WIFI_SSID = "Anh An";
constexpr const char* WIFI_PASSWORD = "qwertyuiop";

// WebSocket server details
constexpr const char* WEBSOCKET_SERVER_HOST = "192.168.31.100";
constexpr uint16_t WEBSOCKET_SERVER_PORT = 8000;

// Sensor pins
constexpr int TEMP_SENSOR_PIN = 26;       // DS18B20 DATA pin
constexpr int PHOTORESISTOR_PIN = 4;     // Photoresistor pin
constexpr int INFRARED_SENSOR_PIN = 18;  // Infrared sensor pin
constexpr int SOUND_SENSOR_PIN = 15;     // Sound sensor pin

// Fan control pins
constexpr int FAN_PWM_PIN = 16;          // PWM pin for fan
constexpr int FAN_PWM_PIN_1 = 5;         // Secondary PWM pin for fan

// Other constants (add more as needed)
// Example: Message types
constexpr const char* MESSAGE_TYPE_COMMAND_FAN = "COMMAND_FAN";
constexpr const char* MESSAGE_TYPE_SETTING_TEMP = "SETTING_TEMP";
constexpr const char* MESSAGE_TYPE_INFO = "INFO";

#endif