#include "Config.h"
#include "DataSender.h"
#include "Sensors.h"
#include "UserInterface.h"
#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager wifiManager;
Sensors sensors;
UserInterface ui;
DataSender dataSender;

unsigned long lastReportTime = 0;
const unsigned long reportingInterval = 1000; // 1 second

void setup() {
  Serial.begin(9600);

  ui.begin();

  wifiManager.connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  ui.setLED(LED_WIFI, HIGH);

  sensors.begin();
  ui.setLED(LED_SENSORS, HIGH);
}

void loop() {
  if (ui.isButtonPressed()) {
    Serial.println("Button pressed, starting data collection");

    // Update LEDs
    ui.setLED(LED_SENSORS, LOW);
    ui.setLED(LED_SENDING, HIGH);

    float tempC = sensors.getTemperature();

    // Collect heart rate and SpO2 over 10 seconds
    unsigned long startTime = millis();
    float heartRateSum = 0;
    float spO2Sum = 0;
    int readings = 0;

    while (millis() - startTime < 10000) { // Collect data for 10 seconds
      sensors.updateOximeter();

      if (millis() - lastReportTime > reportingInterval) {
        float hr = sensors.getHeartRate();
        float spo2 = sensors.getSpO2();

        if (hr > 0 && spo2 > 0) {
          heartRateSum += hr;
          spO2Sum += spo2;
          readings++;
        }
        lastReportTime = millis();
      }
    }

    // Calculate averages
    float heartRate = (readings > 0) ? heartRateSum / readings : 0;
    float spO2 = (readings > 0) ? spO2Sum / readings : 0;

    Serial.printf("Average Heart Rate: %.2f bpm\n", heartRate);
    Serial.printf("Average SpO2: %.2f %%\n", spO2);
    Serial.printf("Temperature: %.2f °C\n", tempC);

    String jsonData = "{";
    jsonData += "\"temperature\":";
    jsonData += tempC;
    jsonData += ",\"heart_rate\":";
    jsonData += heartRate;
    jsonData += ",\"spo2\":";
    jsonData += spO2;
    jsonData += "}";

    if (wifiManager.isConnected()) {
      bool success = dataSender.sendData(SERVER_NAME, jsonData);
      if (success) {
        Serial.println("Data sent successfully");
      } else {
        Serial.println("Failed to send data");
      }
    } else {
      Serial.println("Wi-Fi not connected");
    }

    ui.setLED(LED_SENDING, LOW);
    ui.setLED(LED_SENSORS, HIGH);
  }
}
