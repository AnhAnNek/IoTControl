#include "main.h"

void setup() {
  try {
    Serial.println("Inside try block");
    initializeSystem();
  } catch (const std::exception& e) {
    Serial.print("Caught exception: ");
    Serial.println(e.what());
  } catch (...) {
    Serial.println("Caught unknown exception");
  }
}

void loop() {
  runSystem();
}
