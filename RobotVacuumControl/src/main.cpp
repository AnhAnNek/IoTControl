#include <Arduino.h>
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "SensorManager.h"
#include "Constants.h"
#include "MotorController.h"
#include "RobotController.h"

SensorPins sensorPins = {
  FRONT_SENSOR_PIN, 
  LEFT_SENSOR_PIN, 
  RIGHT_SENSOR_PIN, 
  IR_SENSOR_PIN
};
MotorPins motorPins = {
  MOTOR_IN1_PIN, 
  MOTOR_IN2_PIN, 
  MOTOR_IN3_PIN, 
  MOTOR_IN4_PIN, 
  MOTOR_ENA_PIN, 
  MOTOR_ENB_PIN
};

SensorManager sensorManager(sensorPins);
MotorController motorController(motorPins);
RobotController robotController(sensorManager, motorController);

unsigned long previousMillis = 0;

void customMessageHandler(const char* message);

void setup() {
  Serial.begin(9600);

  // Initialize Wi-Fi
  WiFiManager& wifiManager = WiFiManager::getInstance();
  wifiManager.initializeWiFi(WIFI_SSID, WIFI_PASSWORD);

  // Initialize WebSocketManager
  WebSocketManager& webSocketManager = WebSocketManager::getInstance();
  webSocketManager.initialize(WEBSOCKET_SERVER_HOST, WEBSOCKET_SERVER_PORT);
  webSocketManager.setOnReceivedMessageCallback(customMessageHandler);

  sensorManager.begin();
  motorController.begin();
  Serial.println("Robot initialized!");
}

void loop() {
  if (sensorManager.isObstacleFront()) {
    Serial.println("Obstacle detected in front!");
    robotController.stop();
    delay(DELAY_TIME);
    robotController.rotateRight(ROTATION_SPEED);
    delay(DELAY_TIME);
  } else if (sensorManager.isNearStairs()) {
    Serial.println("Ledge detected! Stopping...");
    robotController.stop();
    delay(DELAY_TIME);
  } else {
    Serial.println("moveForward");
    robotController.moveForward(DEFAULT_SPEED);
  }
}

void customMessageHandler(const char* message) 
{
}