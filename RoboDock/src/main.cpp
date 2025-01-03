#include <Arduino.h>
#include "BatteryChargingStation.h"
#include "Constants.h"

BatteryChargingStation station(IR_LED_PIN, RELAY_PIN, ULTRASONIC_TRIG, ULTRASONIC_ECHO);

void setup() {
    Serial.begin(115200);
    station.init();
    Serial.println("Battery Charging Station Initialized");
}

void loop() {
    station.signalRobot(); // Continuously signal for the robot

    if (station.isRobotDocked()) {
        Serial.println("Robot Docked. Starting Charging...");
        station.startCharging();
        delay(5000); // Simulate charging for 5 seconds
        station.stopCharging();
        Serial.println("Charging Completed");
    } else {
        station.stopCharging();
        Serial.println("Awaiting Robot...");
    }

    delay(1000); // Loop every second
}
