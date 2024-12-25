#include "RelayControl.h"

// Constructor
RelayControl::RelayControl(uint8_t relayPin, const char* name, bool activeState)
    : _relayPin(relayPin), _name(name), _activeState(activeState), 
      _currentState(!activeState), _isAutoMode(false), 
      _tempToRun(0.0), _tempToStop(0.0) {}

// Initialize the relay pin
void RelayControl::begin() {
    pinMode(_relayPin, OUTPUT);
    digitalWrite(_relayPin, !_activeState); // Set relay to off state
}

// Set auto mode and thresholds
void RelayControl::setAutoMode(bool isAutoMode, float tempToRun, float tempToStop) {
    _isAutoMode = isAutoMode;
    if (_isAutoMode) {
        _tempToRun = tempToRun;
        _tempToStop = tempToStop;
    }
}

// Update the relay state based on temperature
void RelayControl::update(float currentTemp) {
    if (_isAutoMode) {
        if (currentTemp >= _tempToRun) {
            digitalWrite(_relayPin, _activeState); // Turn on relay
            _currentState = true;
        } else if (currentTemp < _tempToStop) {
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

// Get the current state of the relay
bool RelayControl::getState() {
    return _currentState;
}

// Check if the relay is in auto mode
bool RelayControl::isAutoMode() {
    return _isAutoMode;
}

// Get the name of the relay
const char* RelayControl::getName() {
    return _name;
}

float RelayControl::getTempToRun() {
    return _tempToRun;
}

float RelayControl::getTempToStop() {
    return _tempToStop;
}
