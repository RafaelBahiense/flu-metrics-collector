#pragma once

class UserInterface {
public:
  void begin();
  bool isButtonPressed();
  void setLED(int ledPin, bool state);
};
