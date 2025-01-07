#include <Arduino.h>
#include <ESP32Servo.h> // Library for controlling Servo on ESP32

// Declare Servo
Servo myServo;

// Servo signal pin
const int servoPin = 18;

// Pins for HC-SR04
const int trigPin = 17;
const int echoPin = 16;

// Pin for the push button
const int buttonPin = 12; // Button pin to reset the servo

// Time interval between distance measurements (ms)
const unsigned long interval = 100; 
unsigned long previousMillis = 0; // Variable to store the last measurement time

bool hasMovedTo90 = false; // Flag to track if the Servo has moved to 90 degrees
bool lastButtonState = LOW; // Last button state (to detect state change)
bool buttonPressed = false; // Flag to track button press

void setup() {
    Serial.begin(115200); // Initialize serial communication

    // Attach the Servo to the specified pin
    myServo.attach(servoPin);
    myServo.write(90); 

    // Set up HC-SR04 pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Set up button pin
    pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor for button pin

    Serial.println("Servo + HC-SR04 test with button starting...");
}

// Function to measure distance (cm) using HC-SR04
long getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2); // Ensure a clean LOW pulse
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Send a 10µs HIGH pulse
    digitalWrite(trigPin, LOW);

    // Measure the duration of the echo pulse
    long duration = pulseIn(echoPin, HIGH, 38000); // Timeout at ~400cm (38ms)

    // Check for out-of-range conditions
    if (duration == 0) {
        Serial.println("No echo detected (object too close or out of range).");
        return -1; // Indicates out of range
    }

    // Calculate the distance in cm
    long distance = duration * 0.034 / 2;

    if (distance < 2) {
        Serial.println("Object too close (< 2 cm).");
        return -1; // Indicates out of range
    } else if (distance > 400) {
        Serial.println("Object too far (> 400 cm).");
        return -1; // Indicates out of range
    }

    return distance; // Return valid distance
}

void loop() {
    // Get the current time
    unsigned long currentMillis = millis();

    // Check if it's time for the next measurement
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis; // Update the last measurement time

        // Measure distance
        long distance = getDistance();
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        if (distance != -1 && distance < 4 && !hasMovedTo90) {
            myServo.write(0);
            hasMovedTo90 = true;
            Serial.println("Servo moved to 90 degrees and locked.");
        }
    }

    // Read the button state
    bool buttonState = digitalRead(buttonPin);

    // Check if button is pressed (low level due to pull-up)
    if (buttonState == LOW && lastButtonState == HIGH) {
        buttonPressed = true; // Button press detected
        Serial.println("Button pressed! Resetting Servo to 0 degrees.");
    }

    // Update the last button state
    lastButtonState = buttonState;

    if (buttonPressed) {
        myServo.write(90);
        hasMovedTo90 = false; // Allow the servo to move again if needed
        buttonPressed = false; // Reset button pressed flag
    }

    // Additional tasks can be added here without interruption
}
