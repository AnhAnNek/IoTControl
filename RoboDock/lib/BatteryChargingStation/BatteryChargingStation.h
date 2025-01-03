#ifndef BATTERYCHARGINGSTATION_H
#define BATTERYCHARGINGSTATION_H

#include <Arduino.h>

class BatteryChargingStation {
public:
    BatteryChargingStation(int irPin, int relayPin, int trigPin, int echoPin);
    void init();
    void signalRobot();
    bool isRobotDocked();
    void startCharging();
    void stopCharging();

private:
    int _irPin, _relayPin, _trigPin, _echoPin;
    long readUltrasonicDistance();
};

#endif
