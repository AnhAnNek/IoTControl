#include <Arduino.h>
#include <IRremote.h>

const int irPin = 23; // Pin connected to KY-005 (Signal)

void setup() {
  IrSender.begin(irPin); // Initialize IR sender
}

void loop() {
  String jsonData = "{\"name\":\"robot\",\"command\":\"move\",\"speed\":5}";
  for (unsigned int i = 0; i < jsonData.length(); i++) {
    IrSender.sendNECMSB(jsonData[i], 8); // Send each character as an 8-bit value
    delay(50); // Small delay to ensure smooth transmission
  }
  delay(1000); // Wait for 1 second before sending again
}
