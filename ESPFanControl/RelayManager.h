#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include <Arduino.h>

class RelayManager {
public:
    // Constructor
    RelayManager(int pin, float activationTemp);

    // Initialize relay
    void begin();

    // Update relay state based on current temperature
    void update(float currentTemp);

    // Manually set relay state
    void setState(bool state);

private:
    int pin;
    float activationTemp;
    bool currentState;
};

#endif
