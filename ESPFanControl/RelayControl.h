#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

#include <Arduino.h>

class RelayControl {
public:
    // Constructor to initialize the relay pin, state, and name
    RelayControl(uint8_t relayPin, const char* name, bool activeState = LOW);

    // Initialize the relay
    void begin();

    // Set temperature thresholds and auto mode
    void setAutoMode(bool isAutoMode, float tempToRun = 0.0, float tempToStop = 0.0);

    // Update the relay state based on temperature
    void update(float currentTemp);

    // Turn the relay on (manual mode)
    void turnOn();

    // Turn the relay off (manual mode)
    void turnOff();

    // Get the current state of the relay
    bool getState();

    // Check if the relay is in auto mode
    bool isAutoMode();

    // Get the name of the relay
    const char* getName();

    float getTempToRun();

    float getTempToStop();

private:
    uint8_t _relayPin;        // Pin connected to the relay
    const char* _name;        // Name of the relay
    bool _activeState;        // Relay active state (LOW or HIGH)
    bool _currentState;       // Current state of the relay
    bool _isAutoMode;         // Whether the relay is in auto mode
    float _tempToRun;         // Temperature to activate the relay
    float _tempToStop;        // Temperature to deactivate the relay
};

#endif // RELAYCONTROL_H
