#include <Arduino.h>

#define MOTOR_PIN 25          // First motor pin
#define LED_PIN 27            // Pin to control LED
#define PWM_FREQUENCY 30000   // PWM frequency for motor and LED
#define PWM_RESOLUTION 8      // PWM resolution (8-bit: 0-255)
#define MOTOR_CHANNEL 0       // PWM channel for first motor
#define LED_CHANNEL 1         // PWM channel for LED

void setDutyCycle(int dutyCycle);

void setup() {
  // Set up the PWM channel for the first motor
  ledcSetup(MOTOR_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(MOTOR_PIN, MOTOR_CHANNEL);

  // Set up the PWM channel for the LED
  ledcSetup(LED_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(LED_PIN, LED_CHANNEL);
}

void loop() {
  // Smoothly increase and decrease motor and LED brightness
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    setDutyCycle(dutyCycle);
    delay(20);
  }
  delay(5000);
  
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    setDutyCycle(dutyCycle);
    delay(20);
  }
  delay(5000);
}

void setDutyCycle(int dutyCycle) {
  ledcWrite(MOTOR_CHANNEL, dutyCycle);   // Update first motor PWM
  ledcWrite(LED_CHANNEL, dutyCycle);     // Update LED PWM
}
