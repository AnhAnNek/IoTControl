#include "SensorManager.h"
#include <IRremote.h> // Include IRremote here

// Constructor to initialize sensor pins
SensorManager::SensorManager(SensorPins pins) : _pins(pins) {}

// Method to initialize the sensors
void SensorManager::begin() {
    // Initialize IR obstacle sensor pin
    pinMode(_pins.irObstaclePin, INPUT);

    // Initialize ultrasonic sensor pins
    pinMode(_pins.frontTrigPin, OUTPUT);
    pinMode(_pins.frontEchoPin, INPUT);
    pinMode(_pins.leftTrigPin, OUTPUT);
    pinMode(_pins.leftEchoPin, INPUT);
    pinMode(_pins.rightTrigPin, OUTPUT);
    pinMode(_pins.rightEchoPin, INPUT);

    // Initialize IR receiver with appropriate feedback settings
    IrReceiver.begin(_pins.irReceiverPin, DISABLE_LED_FEEDBACK);
}

bool SensorManager::isObstacleFront() {
    long distance = getFrontObstacleDistance();
    return distance < Constants::OBSTACLE_THRESHOLD_CM;
}

bool SensorManager::isObstacleLeft() {
    long distance = getLeftObstacleDistance();
    return distance < Constants::OBSTACLE_THRESHOLD_CM;
}

bool SensorManager::isObstacleRight() {
    long distance = getRightObstacleDistance();
    return distance < Constants::OBSTACLE_THRESHOLD_CM;
}

long SensorManager::getFrontObstacleDistance()
{
    long distance = readUltrasonic(_pins.frontTrigPin, _pins.frontEchoPin);
    Serial.print("Front Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance;
}

long SensorManager::getLeftObstacleDistance()
{
    long distance = readUltrasonic(_pins.leftTrigPin, _pins.leftEchoPin);
    Serial.print("Left Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance;
}

long SensorManager::getRightObstacleDistance()
{
    long distance = readUltrasonic(_pins.rightTrigPin, _pins.rightEchoPin);
    Serial.print("Right Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    return distance;
}

bool SensorManager::isNearStairs() {
    int value = digitalRead(_pins.irObstaclePin);
    Serial.print("IR Obstacle Read: ");
    Serial.println(value);
    return value == HIGH;
}

bool SensorManager::isSignalFromBase() {
    if (IrReceiver.decode()) {
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print raw data in HEX
        IrReceiver.printIRResultShort(&Serial); // Print decoded data in a single line
        IrReceiver.printIRSendUsage(&Serial); // Print the send command
        IrReceiver.resume(); // Prepare for the next signal
        return true;
    }
    return false;
}

// Helper method to read ultrasonic sensor distance
long SensorManager::readUltrasonic(unsigned short trigPin, unsigned short echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000); // Timeout after 30ms
    if (duration == 0) return -1;

    long distance = duration * 0.034 / 2; // Convert duration to cm
    return (distance < 2 || distance > 400) ? 0 : distance; // Valid range: 2-400 cm
}