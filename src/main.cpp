#include "Config.h"
#include "DataSender.h"
#include "Sensors.h"
#include "State.h"
#include "UserInterface.h"
#include "WiFiManager.h"
#include <Arduino.h>

WiFiManager wifiManager;
Sensors sensors;
UserInterface ui;
DataSender dataSender;
State deviceState;

void setup() {
  Serial.begin(115200);

  ui.begin();
  ui.displayMessage("Conectando ao Wi-Fi...");

  wifiManager.connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  ui.displayMessage("Wi-Fi Conectado");

  sensors.begin();
  ui.displayMessage("Sensores Iniciados");

  deviceState.setCurrentState(State::Debug);
  ui.displayMessage("Pressione o Botão para Iniciar");
}

void loop() {
  static unsigned long lastOxiUpdateTime = 0;
  const unsigned long oximeterInterval = 10000;
  static unsigned long lastTempUpdateTime = 0;
  const unsigned long tempInterval = 5000;

  switch (deviceState.getCurrentState()) {
  case State::Idle: {
    if (ui.isButtonPressed()) {

      deviceState.setCollectingOximeter(true);
      deviceState.setOximeterStartTime(millis());
      deviceState.resetHeartRateSum();
      deviceState.resetSpO2Sum();
      deviceState.resetReadings();

      deviceState.setCurrentState(State::CollectingOximeter);

      while (ui.isButtonPressed()) {
        delay(10);
      }
    }
    break;
  }

  case State::CollectingOximeter: {
    sensors.updateOximeter();
    if (millis() - lastOxiUpdateTime > oximeterInterval) {
      float hr = sensors.getHeartRate();
      float spo2 = sensors.getSpO2();
      Serial.printf("HR: %.2f bpm\n", hr);
      Serial.printf("SpO2: %.2f %%\n", spo2);

      ui.displayOximeterReadings(hr, spo2);
      if (hr > 0 && spo2 > 0) {
        deviceState.addHeartRate(hr);
        deviceState.addSpO2(spo2);
        deviceState.incrementReadings();

        ui.displayOximeterReadings(hr, spo2);
      } else {
        ui.displayOximeterReadings(deviceState.getHeartRate(),
                                   deviceState.getSpO2());
      }

      lastOxiUpdateTime = millis();
    }

    if (millis() - deviceState.getOximeterStartTime() >= 10000) {
      deviceState.setCollectingOximeter(false);

      int readings = deviceState.getReadings();
      float heartRateAvg =
          (readings > 0) ? deviceState.getHeartRateSum() / readings : 0;
      float spO2Avg = (readings > 0) ? deviceState.getSpO2Sum() / readings : 0;

      deviceState.setHeartRate(heartRateAvg);
      deviceState.setSpO2(spO2Avg);

      Serial.printf("Média HR: %.2f bpm\n", deviceState.getHeartRate());
      Serial.printf("Média SpO2: %.2f %%\n", deviceState.getSpO2());

      ui.displayOximeterReadings(deviceState.getHeartRate(),
                                 deviceState.getSpO2());

      // ui.displayMessage("Colete a Temperatura...");
      // deviceState.setCurrentState(State::CollectingTemperature);
      // deviceState.setTemperatureStartTime(millis());
    }
    break;
  }

  case State::CollectingTemperature: {
    if (millis() - lastTempUpdateTime >= tempInterval) {
      float tempC = sensors.getTemperature();
      deviceState.addTemperature(tempC);
      deviceState.incrementTempReadings();

      Serial.printf("Temperature Reading: %.2f °C\n", tempC);
      ui.displayTemperature(tempC);

      lastTempUpdateTime = millis();
    }

    if (millis() - deviceState.getTemperatureStartTime() >= 10000) {
      deviceState.setCollectingTemperature(false);

      int tempReadings = deviceState.getTempReadings();
      float tempAvg = (tempReadings > 0)
                          ? deviceState.getTemperatureSum() / tempReadings
                          : 0;

      deviceState.setTemperature(tempAvg);

      Serial.printf("Average Temperature: %.2f °C\n",
                    deviceState.getTemperature());
      ui.displayTemperature(deviceState.getTemperature());

      deviceState.setCurrentState(State::SendingData);
    }
    break;
  }

  case State::SendingData: {
    String jsonData = "{";
    jsonData += "\"temperature\":";
    jsonData += deviceState.getTemperature();
    jsonData += ",\"heart_rate\":";
    jsonData += deviceState.getHeartRate();
    jsonData += ",\"spo2\":";
    jsonData += deviceState.getSpO2();
    jsonData += "}";

    if (wifiManager.isConnected()) {
      ui.displayMessage("Enviando Dados...");
      bool success = dataSender.sendData(SERVER_NAME, jsonData);
      if (success) {
        Serial.println("Dados enviados com sucesso");
        ui.displayMessage("Dados Enviados com Sucesso");
        deviceState.setCurrentState(State::DisplayResult);
      } else {
        Serial.println("Falha ao enviar dados");
        ui.displayMessage("Falha ao Enviar Dados");
        deviceState.setCurrentState(State::Error);
      }
    } else {
      Serial.println("Wi-Fi não conectado");
      ui.displayMessage("Wi-Fi não Conectado");
      deviceState.setCurrentState(State::Error);
    }
    break;
  }

  case State::DisplayResult: {
    ui.displayMessage("Pressione Botão para Iniciar");
    deviceState.setCurrentState(State::Idle);
    break;
  }

  case State::Error: {
    ui.displayMessage("Pressione Botão para Iniciar");
    deviceState.setCurrentState(State::Idle);
    break;
  }

  case State::Debug: {
    sensors.updateOximeter();
    float hr = sensors.getHeartRate();
    float spo2 = sensors.getSpO2();
    float temp = sensors.getTemperature();
    auto wifiInfo = wifiManager.getWifiInfos();

    ui.displayAllInfos(hr, spo2, temp, wifiInfo.ssid, wifiInfo.localIP,
                       wifiInfo.rssi);
    break;
  }

  default: {
    deviceState.setCurrentState(State::Error);
    break;
  }
  }
}
