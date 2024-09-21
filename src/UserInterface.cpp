#include "UserInterface.h"
#include "Config.h"
#include <Arduino.h>

void UserInterface::begin() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_SENSORS, OUTPUT);
  pinMode(LED_SENDING, OUTPUT);
}

bool UserInterface::isButtonPressed() { return digitalRead(BUTTON_PIN) == LOW; }

void UserInterface::setLED(int ledPin, bool state) {
  digitalWrite(ledPin, state ? HIGH : LOW);
}
