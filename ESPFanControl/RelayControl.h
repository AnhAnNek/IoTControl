#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

#include <Arduino.h>

class RelayControl {
public:
    // Constructor to initialize the relay pin and state
    RelayControl(uint8_t relayPin, bool activeState = LOW);

    // Initialize the relay
    void begin();

    // Set temperature thresholds and auto mode
    void setAutoMode(bool isAutoMode, float settingTemp = 0.0, float delayTemp = 0.0);

    // Update the relay state based on temperature
    void update(float currentTemp);

    // Turn the relay on (manual mode)
    void turnOn();

    // Turn the relay off (manual mode)
    void turnOff();

    // Toggle the relay state (manual mode)
    void toggle();

    // Get the current state of the relay
    bool getState();

private:
    uint8_t _relayPin;        // Pin connected to the relay
    bool _activeState;        // Relay active state (LOW or HIGH)
    bool _currentState;       // Current state of the relay
    bool _isAutoMode;         // Whether the relay is in auto mode
    float _settingTemp;       // Temperature threshold
    float _delayTemp;         // Temperature delay
};

#endif // RELAYCONTROL_H
