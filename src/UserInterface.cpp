#include "UserInterface.h"
#include "Config.h"
#include <Arduino.h>

void UserInterface::begin() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed");
    for (;;)
      ;
  }
  OLED.clearDisplay();
  OLED.setTextSize(1);
  OLED.setTextColor(SSD1306_WHITE);
  OLED.setCursor(0, 0);
  OLED.display();
}

bool UserInterface::isButtonPressed() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        lastButtonState = reading;
        return true;
      }
    }
  }

  lastButtonState = reading;
  return false;
}

void UserInterface::displayOximeterReadings(float heartRate, float spO2) {
  OLED.clearDisplay();
  OLED.setCursor(0, 0);
  OLED.printf("HR: %.0f bpm\n", heartRate);
  OLED.printf("SpO2: %.0f%%", spO2);
  OLED.display();
}

void UserInterface::displayOximeterReadingsMedium(float heartRate, float spO2) {
  OLED.clearDisplay();
  OLED.setCursor(0, 0);
  OLED.printf("Media HR: %.0f bpm\n", heartRate);
  OLED.printf("Media SpO2: %.0f%%\n", spO2);
  OLED.printf("Pressione Botao para continuar");
  OLED.display();
}

void UserInterface::displayTemperature(float temperature) {
  OLED.clearDisplay();
  OLED.setCursor(0, 0);
  OLED.printf("Temp: %.2f C", temperature);
  OLED.display();
}

void UserInterface::displayTemperatureMedium(float temperature) {
  OLED.clearDisplay();
  OLED.setCursor(0, 0);
  OLED.printf("Media Temp: %.2f C\n", temperature);
  OLED.printf("Pressione Botao para continuar");
  OLED.display();
}

void UserInterface::displayMessage(const char *message) {
  OLED.clearDisplay();
  OLED.setCursor(0, 0);
  OLED.print(message);
  OLED.display();
}

void UserInterface::displayAllInfos(float heartRate, float spO2,
                                    float temperature, String ssid,
                                    String localIp, int rssi,
                                    bool isButtonPressed) {
  OLED.clearDisplay();
  OLED.setCursor(0, 0);
  OLED.printf("HR: %.0f bpm\n", heartRate);
  OLED.printf("SpO2: %.0f%%", spO2);
  OLED.printf("Temp: %.2f C", temperature);
  OLED.printf("SSID: %s, RSSI: %d", ssid.c_str(), rssi);
  OLED.printf("IP: %s", localIp.c_str());
  OLED.printf("Button: %s", isButtonPressed ? "Pressed" : "Not Pressed");
  OLED.display();
}
