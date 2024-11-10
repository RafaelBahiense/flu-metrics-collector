#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class UserInterface {
public:
  void begin();
  bool isButtonPressed();
  void displayOximeterReadings(float heartRate, float spO2);
  void displayOximeterReadingsMedium(float heartRate, float spO2);
  void displayTemperature(float temperature);
  void displayMessage(const char *message);
  void displayAllInfos(float heartRate, float spO2, float temperature,
                       String ssid, String localIp, int rssi,
                       bool isButtonPressed);

private:
  Adafruit_SSD1306 OLED = Adafruit_SSD1306(128, 64, &Wire);
  int buttonState = HIGH;
  int lastButtonState = HIGH;
  unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;
};
