#include <Arduino.h>
#include "WebSocketManager.h"
#include "WiFiManager.h"
#include "SensorManager.h"
#include "Constants.h"
#include "MotorController.h"
#include "RobotController.h"
#include <IRremote.h>

SensorPins sensorPins = {
  FRONT_SENSOR_TRIG_PIN, 
  FRONT_SENSOR_ECHO_PIN, 
  LEFT_SENSOR_TRIG_PIN,  
  LEFT_SENSOR_ECHO_PIN,  
  RIGHT_SENSOR_TRIG_PIN, 
  RIGHT_SENSOR_ECHO_PIN, 
  IR_SENSOR_PIN
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

SensorManager sensorManager(sensorPins);
MotorController motorController(motorPins);
RobotController robotController(sensorManager, motorController);

// const int PWM_PIN = 21;
// const int recvPin = 22;

unsigned long previousMillis = 0;
unsigned long rotationDuration = 1000; // Time for rotation in milliseconds
unsigned long moveDuration = 2000;     // Time for moving forward in milliseconds
unsigned long stopDuration = 1000;     // Pause time when stopping

enum RobotState {
    IDLE,
    ROTATING_RIGHT,
    ROTATING_LEFT,
    MOVING_FORWARD,
    STOPPING
};

RobotState currentState = IDLE;

void customMessageHandler(const char* message);

void setup() {
  // ledcSetup(0, 5000, 8); // Set up PWM channel 0, 5kHz, 8-bit resolution
  // ledcAttachPin(PWM_PIN, 0); // Attach PWM to Trig/PWM pin

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

  // IrReceiver.begin(recvPin, ENABLE_LED_FEEDBACK); // Initialize IR receiver
  // Serial.println("IR Receiver is ready...");

  robotController.setBrushSpeeds(255, 255);

  Serial.println("Robot initialized!");
}

void loop() {
  // if (IrReceiver.decode()) {
  //   char receivedChar = (char)IrReceiver.decodedIRData.decodedRawData; // Convert received data to char
  //   receivedData += receivedChar; // Append character to the received string

  //   // Check for end of transmission (e.g., newline or specific marker)
  //   if (receivedChar == '}') { 
  //     Serial.print("Received JSON: ");
  //     Serial.println(receivedData);
  //     receivedData = ""; // Clear the buffer for the next message
  //   }

  //   IrReceiver.resume(); // Prepare to receive the next signal
  // }

    unsigned long currentMillis = millis();

    switch (currentState) {
        case IDLE:
            if (sensorManager.isObstacleFront()) {
                Serial.println("Obstacle detected in front!");
                robotController.stop();
                currentState = ROTATING_RIGHT;
                previousMillis = currentMillis;
            } else if (sensorManager.isObstacleLeft()) {
                Serial.println("Obstacle detected on the left!");
                robotController.stop();
                currentState = ROTATING_RIGHT;
                previousMillis = currentMillis;
            } else if (sensorManager.isObstacleRight()) {
                Serial.println("Obstacle detected on the right!");
                robotController.stop();
                currentState = ROTATING_LEFT;
                previousMillis = currentMillis;
            } else if (sensorManager.isNearStairs()) {
                Serial.println("Ledge detected! Stopping...");
                robotController.stop();
                currentState = STOPPING;
                previousMillis = currentMillis;
            } else {
                Serial.println("Moving forward...");
                robotController.moveForward(DEFAULT_SPEED);
                currentState = MOVING_FORWARD;
                previousMillis = currentMillis;
            }
            break;

        case ROTATING_RIGHT:
            if (currentMillis - previousMillis >= rotationDuration) {
                Serial.println("Finished rotating right.");
                currentState = IDLE;
            } else {
                robotController.rotateRight(ROTATION_SPEED);
            }
            break;

        case ROTATING_LEFT:
            if (currentMillis - previousMillis >= rotationDuration) {
                Serial.println("Finished rotating left.");
                currentState = IDLE;
            } else {
                robotController.rotateLeft(ROTATION_SPEED);
            }
            break;

        case MOVING_FORWARD:
            if (currentMillis - previousMillis >= moveDuration) {
                Serial.println("Finished moving forward.");
                robotController.stop();
                currentState = IDLE;
            }
            break;

        case STOPPING:
            if (currentMillis - previousMillis >= stopDuration) {
                Serial.println("Finished stopping.");
                currentState = IDLE;
            }
            break;
    }

  // // Gradually increase speed
  // for (int speed = 0; speed <= 255; speed += 5) {
  //   ledcWrite(0, speed); // Set PWM duty cycle
  //   delay(50);
  // }

  // delay(1000); // Run at full speed for 1 second

  // // Gradually decrease speed
  // for (int speed = 255; speed >= 0; speed -= 5) {
  //   ledcWrite(0, speed); // Set PWM duty cycle
  //   delay(50);
  // }
}

void customMessageHandler(const char* message) 
{
}