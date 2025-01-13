#include <Arduino.h>
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "SensorManager.h"
#include "Constants.h"
#include "MotorController.h"
#include "RobotController.h"

// Sensor and motor pin configurations
SensorPins sensorPins = {
    FRONT_SENSOR_TRIG_PIN, 
    FRONT_SENSOR_ECHO_PIN, 
    LEFT_SENSOR_TRIG_PIN,  
    LEFT_SENSOR_ECHO_PIN,  
    RIGHT_SENSOR_TRIG_PIN, 
    RIGHT_SENSOR_ECHO_PIN, 
    IR_OBSTACLE_PIN,
    IR_RECEIVER_PIN
};
MotorPins motorPins = {
    MOTOR_IN1_PIN, 
    MOTOR_IN2_PIN, 
    MOTOR_IN3_PIN, 
    MOTOR_IN4_PIN, 
    MOTOR_ENA_PIN, 
    MOTOR_ENB_PIN,
    SIDE_BRUSH_PIN,  
    MAIN_BRUSH_PIN
};

// Managers and controllers
SensorManager sensorManager(sensorPins);
MotorController motorController(motorPins);
RobotController robotController(sensorManager, motorController);

// Timers and durations
unsigned long previousMillis = 0;
const int ROTATION_DURATION = 100; // Time for rotation in milliseconds
const int MOVING_DURATION = 100;
const int STOP_DURATION = 1000;     // Pause time when stopping
const int BACK_DURATION = 1000;     // Pause time when stopping

// Function prototypes
void customMessageHandler(const char* message);
void transitionToState(RobotController::State newState, unsigned long currentMillis);
bool hasDurationElapsed(unsigned long currentMillis, unsigned long duration);

void setup() {
    Serial.begin(115200);

    // Initialize Wi-Fi
    WiFiManager& wifiManager = WiFiManager::getInstance();
    wifiManager.initializeWiFi(WIFI_SSID, WIFI_PASSWORD);

    // Initialize WebSocketManager
    WebSocketManager& webSocketManager = WebSocketManager::getInstance();
    webSocketManager.initialize(WEBSOCKET_SERVER_HOST, WEBSOCKET_SERVER_PORT);
    webSocketManager.setOnReceivedMessageCallback(customMessageHandler);

    robotController.begin();

    // robotController.setMovingSpeeds(DEFAULT_SPEED, DEFAULT_SPEED);
    robotController.stop();
    robotController.setBrushSpeeds(255, 255);
    Serial.println("Robot initialized!");
}

void loop() {
    WebSocketManager& webSocketManager = WebSocketManager::getInstance();
    webSocketManager.handleMessages();

    unsigned long currentMillis = millis();

    RobotController::State currentState = robotController.getCurrentState();
    switch (currentState) {
        case RobotController::IDLE: {
            // Store sensor readings in boolean variables
            bool isFrontObstacle = sensorManager.isObstacleFront();
            bool isLeftObstacle = sensorManager.isObstacleLeft();
            bool isRightObstacle = sensorManager.isObstacleRight();
            bool isNearStairs = sensorManager.isNearStairs();

            if ((isFrontObstacle && isLeftObstacle && isRightObstacle) || (isLeftObstacle && isRightObstacle))
            {
                Serial.println("Obstacles detected on all sides! Moving backward...");
                transitionToState(RobotController::MOVING_BACKWARD, currentMillis);
            } 
            else if (isFrontObstacle && isLeftObstacle) 
            {
                Serial.println("Obstacles detected on the front and left sides! Moving backward and turning right...");
                transitionToState(RobotController::MOVING_BACKWARD_AND_ROTATING_RIGHT, currentMillis);
            }
            else if (isFrontObstacle && isRightObstacle) 
            {
                Serial.println("Obstacles detected on the front and right sides! Moving backward and turning left...");
                transitionToState(RobotController::MOVING_BACKWARD_AND_ROTATING_LEFT, currentMillis);
            }
            else if (isFrontObstacle) 
            {
                Serial.println("Obstacle detected in front!");
                robotController.stop();
                transitionToState(RobotController::ROTATING_RIGHT, currentMillis);
            } 
            else if (isLeftObstacle) 
            {
                Serial.println("Obstacle detected on the left!");
                robotController.stop();
                transitionToState(RobotController::ROTATING_RIGHT, currentMillis);
            } 
            else if (isRightObstacle) 
            {
                Serial.println("Obstacle detected on the right!");
                robotController.stop();
                transitionToState(RobotController::ROTATING_LEFT, currentMillis);
            } 
            else if (isNearStairs) 
            {
                Serial.println("Stairs detected on the front!");
                robotController.stop();
                transitionToState(RobotController::MOVING_BACKWARD, currentMillis);
            }
            else 
            {
                Serial.println("Moving forward...");
                transitionToState(RobotController::MOVING_FORWARD, currentMillis);
            }
            break; // End of IDLE case
        }


        case RobotController::ROTATING_RIGHT:
            if (hasDurationElapsed(currentMillis, ROTATION_DURATION)) {
                Serial.println("Finished rotating right.");
                currentState = RobotController::IDLE;
            } else {
                robotController.rotateRight(DEFAULT_SPEED);
            }
            break;

        case RobotController::ROTATING_LEFT:
            if (hasDurationElapsed(currentMillis, ROTATION_DURATION)) {
                Serial.println("Finished rotating left.");
                currentState = RobotController::IDLE;
            } else {
                robotController.rotateLeft(DEFAULT_SPEED);
            }
            break;

        case RobotController::MOVING_FORWARD:
            if (hasDurationElapsed(currentMillis, MOVING_DURATION)) {
                Serial.println("Finished moving forward.");
                currentState = RobotController::IDLE;
            } else {
                robotController.moveForward(DEFAULT_SPEED);
            }
            break;

        case RobotController::MOVING_BACKWARD:
            if (hasDurationElapsed(currentMillis, BACK_DURATION)) {
                Serial.println("Finished moving backward.");
                currentState = RobotController::IDLE;
            } else {
                robotController.moveBackward(DEFAULT_SPEED);
            }
            break;

        case RobotController::STOPPING:
            if (hasDurationElapsed(currentMillis, STOP_DURATION)) {
                Serial.println("Finished stopping.");
                currentState = RobotController::IDLE;
            }
            break;
        
        case RobotController::MOVING_BACKWARD_AND_ROTATING_RIGHT:
            if (hasDurationElapsed(currentMillis, BACK_DURATION)) {
                Serial.println("Finished moving backward, now rotating right.");
                transitionToState(RobotController::ROTATING_RIGHT, currentMillis); // Transition to rotating right after moving backward
            } else {
                robotController.moveBackward(DEFAULT_SPEED);
            }
            break;

        case RobotController::MOVING_BACKWARD_AND_ROTATING_LEFT: 
            if (hasDurationElapsed(currentMillis, BACK_DURATION)) {
                Serial.println("Finished moving backward, now rotating left.");
                transitionToState(RobotController::ROTATING_LEFT, currentMillis); // Transition to rotating right after moving backward
            } else {
                robotController.moveBackward(DEFAULT_SPEED);
            }
            break;
    }

    if (sensorManager.isSignalFromBase()) {
        Serial.println("Signal from base station detected!");
    }
}

void customMessageHandler(const char* message) {
    StaticJsonDocument<256> doc; // Adjust size based on expected JSON complexity

    // Deserialize the JSON message
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Check for required fields
    if (!doc.containsKey("type") || !doc.containsKey("device") || !doc.containsKey("command")) {
        Serial.println("Missing required fields in JSON.");
        return;
    }

    const char* type = doc["type"];       // e.g., "mode" or "ROBOT_CONTROL"
    const char* device = doc["device"];   // e.g., "robot" or "sensor"
    const char* command = doc["command"]; // e.g., "MOVE_FORWARD", "STOP"
    int speed = doc.containsKey("speed") ? doc["speed"] : DEFAULT_SPEED; // Optional field

    // Process "type" field
    if (strcmp(type, "mode") == 0) {
        if (strcmp(command, "AUTO") == 0) {
            robotController.setAutoMode(true);
            Serial.println("Auto mode enabled.");
        } else if (strcmp(command, "MANUAL") == 0) {
            robotController.setAutoMode(false);
            Serial.println("Auto mode disabled. Manual control active.");
        }
    } 
    else if (strcmp(type, "ROBOT_CONTROL") == 0 && strcmp(device, "robot") == 0) {
        // Process robot commands if in manual mode
        if (!robotController.isAutoMode()) {
            if (strcmp(command, "MOVE_FORWARD") == 0) {
                robotController.moveForward(speed);
                Serial.print("Moving forward at speed: ");
                Serial.println(speed);
            } else if (strcmp(command, "MOVE_BACKWARD") == 0) {
                robotController.moveBackward(speed);
                Serial.print("Moving backward at speed: ");
                Serial.println(speed);
            } else if (strcmp(command, "ROTATE_LEFT") == 0) {
                robotController.rotateLeft(speed);
                Serial.print("Rotating left at speed: ");
                Serial.println(speed);
            } else if (strcmp(command, "ROTATE_RIGHT") == 0) {
                robotController.rotateRight(speed);
                Serial.print("Rotating right at speed: ");
                Serial.println(speed);
            } else if (strcmp(command, "STOP") == 0) {
                robotController.stop();
                Serial.println("Stopping the robot.");
            } else if (strcmp(command, "STOP_SIDE_BRUSH") == 0) {
                robotController.stopSideBrush();
                Serial.println("Stopping side brush.");
            } else if (strcmp(command, "STOP_MAIN_BRUSH") == 0) {
                robotController.stopMainBrush();
                Serial.println("Stopping main brush.");
            } else if (strcmp(command, "START_SIDE_BRUSH") == 0) {
                robotController.startSideBrush(DEFAULT_SPEED);
                Serial.println("Starting side brush.");
            } else if (strcmp(command, "START_MAIN_BRUSH") == 0) {
                robotController.startMainBrush(DEFAULT_SPEED);
                Serial.println("Starting side brush.");
            } else {
                Serial.print("Unknown command: ");
                Serial.println(command);
            }
        } else {
            Serial.println("Manual commands ignored in auto mode.");
        }
    } 
    else {
        Serial.print("Unknown type or device: ");
        Serial.print(type);
        Serial.print(", ");
        Serial.println(device);
    }
}

void transitionToState(RobotController::State newState, unsigned long currentMillis) {
    robotController.setCurrentState(newState);
    previousMillis = currentMillis;
}

bool hasDurationElapsed(unsigned long currentMillis, unsigned long duration) {
    return (currentMillis - previousMillis >= duration);
}
