#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

// Wi-Fi credentials
constexpr const char* WIFI_SSID = "Anh An";
constexpr const char* WIFI_PASSWORD = "qwertyuiop";

// WebSocket server details
constexpr const char* WEBSOCKET_SERVER_HOST = "192.168.31.100";
constexpr uint16_t WEBSOCKET_SERVER_PORT = 8000;

// Define pins for sensors
const int FRONT_SENSOR_TRIG_PIN = 23;   // Use GPIO 23 for Front Trigger
const int FRONT_SENSOR_ECHO_PIN = 22;   // Use GPIO 22 for Front Echo
const int LEFT_SENSOR_TRIG_PIN = 19;    // Use GPIO 19 for Left Trigger
const int LEFT_SENSOR_ECHO_PIN = 18;    // Use GPIO 18 for Left Echo
const int RIGHT_SENSOR_TRIG_PIN = 5;    // Use GPIO 5 for Right Trigger
const int RIGHT_SENSOR_ECHO_PIN = 4;    // Use GPIO 4 for Right Echo
const int IR_SENSOR_PIN = 17;           // Use GPIO 17 for IR Sensor

// Define pins for motor controller
const int MOTOR_IN1_PIN = 25;    // Use GPIO 25 for motor input 1
const int MOTOR_IN2_PIN = 26;    // Use GPIO 26 for motor input 2
const int MOTOR_IN3_PIN = 27;    // Use GPIO 27 for motor input 3
const int MOTOR_IN4_PIN = 14;    // Use GPIO 14 for motor input 4
const int MOTOR_ENA_PIN = 12;    // Use GPIO 12 for motor enable A
const int MOTOR_ENB_PIN = 13;    // Use GPIO 13 for motor enable B
const int SIDE_BRUSH_PIN = 32; 
const int MAIN_BRUSH_PIN = 33;

// Define speed constants
const int DEFAULT_SPEED = 255;
const int ROTATION_SPEED = 150;

// Define constants for thresholds and simulated values
const int OBSTACLE_THRESHOLD_CM = 5; // Threshold for detecting obstacles

// Other constants (add more as needed)
constexpr const char* MESSAGE_TYPE_INFO = "INFO";

#endif