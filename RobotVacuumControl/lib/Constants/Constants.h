#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

// Wi-Fi credentials
constexpr const char* WIFI_SSID = "Anh An";
constexpr const char* WIFI_PASSWORD = "qwertyuiop";

// WebSocket server details
constexpr const char* WEBSOCKET_SERVER_HOST = "192.168.31.100";
constexpr uint16_t WEBSOCKET_SERVER_PORT = 8000;

// Define pins for sensors
const int FRONT_SENSOR_PIN = 32; // Use GPIO 32 for front sensor
const int LEFT_SENSOR_PIN = 33;  // Use GPIO 33 for left sensor
const int RIGHT_SENSOR_PIN = 34; // Use GPIO 34 for right sensor (input-only)
const int IR_SENSOR_PIN = 35;    // Use GPIO 35 for IR sensor (input-only)

// Define pins for motor controller
const int MOTOR_IN1_PIN = 25;    // Use GPIO 25 for motor input 1
const int MOTOR_IN2_PIN = 26;    // Use GPIO 26 for motor input 2
const int MOTOR_IN3_PIN = 27;    // Use GPIO 27 for motor input 3
const int MOTOR_IN4_PIN = 14;    // Use GPIO 14 for motor input 4
const int MOTOR_ENA_PIN = 12;    // Use GPIO 12 for motor enable A
const int MOTOR_ENB_PIN = 13;    // Use GPIO 13 for motor enable B

// Define speed constants
const int DEFAULT_SPEED = 200;
const int ROTATION_SPEED = 150;

const int DELAY_TIME = 500; // Delay time in milliseconds

// Define constants for thresholds and simulated values
const int OBSTACLE_THRESHOLD_CM = 20; // Threshold for detecting obstacles

// Other constants (add more as needed)
constexpr const char* MESSAGE_TYPE_INFO = "INFO";
constexpr const char* COMMAND_TURN_ON_RELAY = "COMMAND_TURN_ON_RELAY";
constexpr const char* COMMAND_TURN_OFF_RELAY = "COMMAND_TURN_OFF_RELAY";
constexpr const char* COMMAND_SET_RELAY_TEMP = "COMMAND_SET_RELAY_TEMP";

#endif