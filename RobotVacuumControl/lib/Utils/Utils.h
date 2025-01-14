#ifndef UTILS_H
#define UTILS_H

// Wi-Fi credentials
namespace WiFiCredentials {
    constexpr const char* SSID = "Anh An";
    constexpr const char* PASSWORD = "qwertyuiop";
}

// WebSocket server details
namespace WebSocketServer {
    constexpr const char* HOST = "192.168.31.100";
    constexpr uint16_t PORT = 8000;
}

namespace Constants {
    // Define pins for sensors
    const unsigned short FRONT_SENSOR_TRIG_PIN = 23;   // Use GPIO 23 for Front Trigger
    const unsigned short FRONT_SENSOR_ECHO_PIN = 22;   // Use GPIO 22 for Front Echo
    const unsigned short LEFT_SENSOR_TRIG_PIN = 19;    // Use GPIO 19 for Left Trigger
    const unsigned short LEFT_SENSOR_ECHO_PIN = 18;    // Use GPIO 18 for Left Echo
    const unsigned short RIGHT_SENSOR_TRIG_PIN = 5;    // Use GPIO 5 for Right Trigger
    const unsigned short RIGHT_SENSOR_ECHO_PIN = 4;    // Use GPIO 4 for Right Echo
    const unsigned short IR_OBSTACLE_PIN = 17;           // Use GPIO 17 for IR Sensor
    const unsigned short IR_RECEIVER_PIN = 34;

    // Define pins for motor controller
    const unsigned short MOTOR_IN1_PIN = 25;    // Use GPIO 25 for motor input 1
    const unsigned short MOTOR_IN2_PIN = 26;    // Use GPIO 26 for motor input 2
    const unsigned short MOTOR_IN3_PIN = 27;    // Use GPIO 27 for motor input 3
    const unsigned short MOTOR_IN4_PIN = 14;    // Use GPIO 14 for motor input 4
    const unsigned short MOTOR_ENA_PIN = 12;    // Use GPIO 12 for motor enable A
    const unsigned short MOTOR_ENB_PIN = 13;    // Use GPIO 13 for motor enable B
    const unsigned short SIDE_BRUSH_PIN = 32; 
    const unsigned short MAIN_BRUSH_PIN = 33;

    // Define speed constants
    const unsigned short DEFAULT_SPEED = 200;

    // Define constants for thresholds and simulated values
    const unsigned short OBSTACLE_THRESHOLD_CM = 5; // Threshold for detecting obstacles

    // Other constants (add more as needed)
    constexpr const char* MESSAGE_TYPE_INFO = "INFO";

    // Timers and durations
    const unsigned int ROTATION_DURATION = 100; // Time for rotation in milliseconds
    const unsigned int MOVING_DURATION = 100;
    const unsigned int STOP_DURATION = 1000;     // Pause time when stopping
    const unsigned int BACK_DURATION = 1000;     // Pause time when stopping
}

// --- Timer Utilities ---
namespace TimerUtils {
    inline bool hasDurationElapsed(unsigned long previousMillis, unsigned long currentMillis, unsigned int duration) {
        return (currentMillis - previousMillis >= duration);
    }
}

#endif