#include "BatteryChargingStation.h"

BatteryChargingStation::BatteryChargingStation(int irPin, int relayPin, int trigPin, int echoPin) 
  : _irPin(irPin), _relayPin(relayPin), _trigPin(trigPin), _echoPin(echoPin) {}

void BatteryChargingStation::init() {
    pinMode(_irPin, OUTPUT);
    pinMode(_relayPin, OUTPUT);
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    stopCharging();
}

void BatteryChargingStation::signalRobot() {
    digitalWrite(_irPin, HIGH); // Turn on IR signal
    delay(500);
    digitalWrite(_irPin, LOW);  // Turn off IR signal
    delay(500);
}

bool BatteryChargingStation::isRobotDocked() {
    return readUltrasonicDistance() < DETECTION_DISTANCE;
}

void BatteryChargingStation::startCharging() {
    digitalWrite(_relayPin, HIGH); // Activate relay to start charging
}

void BatteryChargingStation::stopCharging() {
    digitalWrite(_relayPin, LOW); // Deactivate relay to stop charging
}

long BatteryChargingStation::readUltrasonicDistance() {
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);
    long duration = pulseIn(_echoPin, HIGH);
    return duration * 0.034 / 2; // Convert to cm
}
