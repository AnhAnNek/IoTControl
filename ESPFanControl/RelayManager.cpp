#include "RelayManager.h"

// Constructor
RelayManager::RelayManager(int pin, float activationTemp)
    : pin(pin), activationTemp(activationTemp), currentState(false) {}

// Initialize relay
void RelayManager::begin() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); // Ensure relay is off initially
}

// Update relay state based on current temperature
void RelayManager::update(float currentTemp) {
    bool newState = currentTemp > activationTemp;
    if (newState != currentState) {
        setState(newState);
    }
}

// Manually set relay state
void RelayManager::setState(bool state) {
    currentState = state;
    digitalWrite(pin, state ? HIGH : LOW);
}
