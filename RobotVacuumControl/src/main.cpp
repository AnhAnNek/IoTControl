#include <Arduino.h>
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "SensorManager.h"
#include "Constants.h"
#include "MotorController.h"
#include "RobotController.h"
#include <IRremote.h>

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

const int PWM_PIN = 21;
const int recvPin = 22;
unsigned long previousMillis = 0;

String receivedData = "";

void customMessageHandler(const char* message);

void setup() {
  ledcSetup(0, 5000, 8); // Set up PWM channel 0, 5kHz, 8-bit resolution
  ledcAttachPin(PWM_PIN, 0); // Attach PWM to Trig/PWM pin

  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFiManager& wifiManager = WiFiManager::getInstance();
  wifiManager.initializeWiFi(WIFI_SSID, WIFI_PASSWORD);

  // Initialize WebSocketManager
  WebSocketManager& webSocketManager = WebSocketManager::getInstance();
  webSocketManager.initialize(WEBSOCKET_SERVER_HOST, WEBSOCKET_SERVER_PORT);
  webSocketManager.setOnReceivedMessageCallback(customMessageHandler);

  sensorManager.begin();
  motorController.begin();

  IrReceiver.begin(recvPin, ENABLE_LED_FEEDBACK); // Initialize IR receiver
  Serial.println("IR Receiver is ready...");

  Serial.println("Robot initialized!");
}

void loop() {
  if (IrReceiver.decode()) {
    char receivedChar = (char)IrReceiver.decodedIRData.decodedRawData; // Convert received data to char
    receivedData += receivedChar; // Append character to the received string

    // Check for end of transmission (e.g., newline or specific marker)
    if (receivedChar == '}') { 
      Serial.print("Received JSON: ");
      Serial.println(receivedData);
      receivedData = ""; // Clear the buffer for the next message
    }

    IrReceiver.resume(); // Prepare to receive the next signal
  }

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

  // Gradually increase speed
  for (int speed = 0; speed <= 255; speed += 5) {
    ledcWrite(0, speed); // Set PWM duty cycle
    delay(50);
  }

  delay(1000); // Run at full speed for 1 second

  // Gradually decrease speed
  for (int speed = 255; speed >= 0; speed -= 5) {
    ledcWrite(0, speed); // Set PWM duty cycle
    delay(50);
  }

  delay(1000); // Stop for 1 second
}

void customMessageHandler(const char* message) 
{
}