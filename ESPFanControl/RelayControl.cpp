#include "RelayControl.h"

// Constructor
RelayControl::RelayControl(uint8_t relayPin, bool activeState)
    : _relayPin(relayPin), _activeState(activeState), _currentState(!activeState), _isAutoMode(false), _settingTemp(0.0), _delayTemp(0.0) {}

// Initialize the relay pin
void RelayControl::begin() {
    pinMode(_relayPin, OUTPUT);
    digitalWrite(_relayPin, !_activeState); // Set relay to off state
}

// Set auto mode and thresholds
void RelayControl::setAutoMode(bool isAutoMode, float settingTemp, float delayTemp) {
    _isAutoMode = isAutoMode;
    if (_isAutoMode) {
        _settingTemp = settingTemp;
        _delayTemp = delayTemp;
    }
}

// Update the relay state based on temperature
void RelayControl::update(float currentTemp) {
    if (_isAutoMode) {
        if (currentTemp >= _settingTemp + _delayTemp) {
            digitalWrite(_relayPin, _activeState); // Turn on relay
            _currentState = true;
        } else if (currentTemp < _settingTemp) {
            digitalWrite(_relayPin, !_activeState); // Turn off relay
            _currentState = false;
        }
    }
}

// Turn the relay on (manual mode or temporary override)
void RelayControl::turnOn() {
    if (_isAutoMode) {
        _isAutoMode = false; // Temporarily disable auto mode
    }
    digitalWrite(_relayPin, _activeState);
    _currentState = true;
}

// Turn the relay off (manual mode or temporary override)
void RelayControl::turnOff() {
    if (_isAutoMode) {
        _isAutoMode = false; // Temporarily disable auto mode
    }
    digitalWrite(_relayPin, !_activeState);
    _currentState = false;
}

// Toggle the relay state (manual mode or temporary override)
void RelayControl::toggle() {
    if (_isAutoMode) {
        _isAutoMode = false; // Temporarily disable auto mode
    }
    if (_currentState) {
        turnOff();
    } else {
        turnOn();
    }
}

// Get the current state of the relay
bool RelayControl::getState() {
    return _currentState;
}
