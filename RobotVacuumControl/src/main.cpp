#include <Arduino.h>
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "SensorManager.h"
#include "Utils.h"
#include "MotorController.h"
#include "RobotController.h"
#include "esp_system.h"

// Sensor and motor pin configurations
SensorPins sensorPins = {
    Constants::FRONT_SENSOR_TRIG_PIN, 
    Constants::FRONT_SENSOR_ECHO_PIN, 
    Constants::LEFT_SENSOR_TRIG_PIN,  
    Constants::LEFT_SENSOR_ECHO_PIN,  
    Constants::RIGHT_SENSOR_TRIG_PIN, 
    Constants::RIGHT_SENSOR_ECHO_PIN, 
    Constants::IR_OBSTACLE_PIN,
    Constants::IR_RECEIVER_PIN
};
MotorPins motorPins = {
    Constants::MOTOR_IN1_PIN, 
    Constants::MOTOR_IN2_PIN, 
    Constants::MOTOR_IN3_PIN, 
    Constants::MOTOR_IN4_PIN, 
    Constants::MOTOR_ENA_PIN, 
    Constants::MOTOR_ENB_PIN,
    Constants::SIDE_BRUSH_PIN,  
    Constants::MAIN_BRUSH_PIN
};

// Managers and controllers
SensorManager sensorManager(sensorPins);
MotorController motorController(motorPins);
RobotController robotController(sensorManager, motorController);

// Function prototypes
void customMessageHandler(const char* message);
void printSystemStatus();

void setup() {
    Serial.begin(115200);

    // Initialize Wi-Fi
    WiFiManager& wifiManager = WiFiManager::getInstance();
    wifiManager.initializeWiFi(WiFiCredentials::SSID, WiFiCredentials::PASSWORD);

    // Initialize WebSocketManager
    WebSocketManager& webSocketManager = WebSocketManager::getInstance();
    webSocketManager.initialize(WebSocketServer::HOST, WebSocketServer::PORT);
    webSocketManager.setOnReceivedMessageCallback(customMessageHandler);

    robotController.begin();

    // robotController.setMovingSpeeds(DEFAULT_SPEED, DEFAULT_SPEED);
    robotController.setCurrentState(RobotController::IDLE);
    robotController.moveForward(Constants::DEFAULT_SPEED);
    robotController.setBrushSpeeds(255, 255);

    Serial.println("Robot initialized!");
}

void loop() {
    // printSystemStatus();

    WebSocketManager& webSocketManager = WebSocketManager::getInstance();
    webSocketManager.handleMessages();

    unsigned long currentMillis = millis();
    robotController.handleAutoMode(currentMillis);
    robotController.executeAction();

    if (sensorManager.isSignalFromBase()) {
        Serial.println("Signal from base station detected!");
    }
}

void customMessageHandler(const char* message) {
    StaticJsonDocument<256> doc; // Adjust size based on expected JSON complexity

    // Deserialize the JSON message
    DeserializationError error = deserializeJson(doc, message);
    if (error) 
    {
        Serial.print("JSON parse failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Check for required fields
    if (!doc.containsKey("type") || !doc.containsKey("device") || !doc.containsKey("command")) 
    {
        Serial.println("Missing required fields in JSON.");
        return;
    }

    const char* type = doc["type"];       // e.g., "mode" or "ROBOT_CONTROL"
    const char* device = doc["device"];   // e.g., "robot" or "sensor"
    const char* command = doc["command"]; // e.g., "MOVE_FORWARD", "STOP"
    int speed = doc.containsKey("speed") ? doc["speed"] : Constants::DEFAULT_SPEED; // Optional field

    // Process "type" field
    if (strcmp(type, "mode") == 0) 
    {
        if (strcmp(command, "AUTO") == 0) 
        {
            robotController.setAutoMode(true);
            Serial.println("Auto mode enabled.");
        } 
        else if (strcmp(command, "MANUAL") == 0) 
        {
            robotController.setAutoMode(false);
            Serial.println("Auto mode disabled. Manual control active.");
        }
    } 
    else if (strcmp(type, "ROBOT_CONTROL") == 0 && strcmp(device, "robot") == 0) 
    {
        // Process robot commands if in manual mode
        if (!robotController.isAutoMode()) 
        {
            if (strcmp(command, "MOVE_FORWARD") == 0) 
            {
                robotController.moveForward(speed);
                Serial.print("Moving forward at speed: ");
                Serial.println(speed);
            } 
            else if (strcmp(command, "MOVE_BACKWARD") == 0) 
            {
                robotController.moveBackward(speed);
                Serial.print("Moving backward at speed: ");
                Serial.println(speed);
            } 
            else if (strcmp(command, "ROTATE_LEFT") == 0) 
            {
                robotController.rotateLeft(speed);
                Serial.print("Rotating left at speed: ");
                Serial.println(speed);
            } 
            else if (strcmp(command, "ROTATE_RIGHT") == 0) 
            {
                robotController.rotateRight(speed);
                Serial.print("Rotating right at speed: ");
                Serial.println(speed);
            } 
            else if (strcmp(command, "STOP") == 0) 
            {
                robotController.stop();
                Serial.println("Stopping the robot.");
            } 
            else if (strcmp(command, "STOP_SIDE_BRUSH") == 0) 
            {
                robotController.stopSideBrush();
                Serial.println("Stopping side brush.");
            } 
            else if (strcmp(command, "STOP_MAIN_BRUSH") == 0) 
            {
                robotController.stopMainBrush();
                Serial.println("Stopping main brush.");
            } 
            else if (strcmp(command, "START_SIDE_BRUSH") == 0) 
            {
                robotController.startSideBrush(Constants::DEFAULT_SPEED);
                Serial.println("Starting side brush.");
            } 
            else if (strcmp(command, "START_MAIN_BRUSH") == 0) 
            {
                robotController.startMainBrush(Constants::DEFAULT_SPEED);
                Serial.println("Starting side brush.");
            } 
            else 
            {
                Serial.print("Unknown command: ");
                Serial.println(command);
            }
        } 
        else 
        {
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

void printSystemStatus() 
{
    // Get RAM (heap memory) usage
    uint32_t freeHeap = esp_get_free_heap_size();
    uint32_t totalHeap = heap_caps_get_total_size(MALLOC_CAP_8BIT);
    uint32_t usedHeap = totalHeap - freeHeap;

    // Get flash memory details
    // const esp_partition_t* runningPartition = esp_ota_get_running_partition();
    // uint32_t flashSize = 0;
    // esp_flash_get_size(NULL, &flashSize);

    // Calculate flash usage based on OTA partitions and their data
    // uint32_t usedFlash = runningPartition->address + runningPartition->size;

    // Print stats
    Serial.println("System Status:");
    Serial.printf("Used RAM: %u / %u bytes (%.2f%%)\n", usedHeap, totalHeap, 100.0f * (float)usedHeap / totalHeap);
    // Serial.printf("Used Flash (ROM): %u / %u bytes (%.2f%%)\n", usedFlash, flashSize, 100.0f * (float)usedFlash / flashSize);
}