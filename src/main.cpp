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

  deviceState.setCurrentState(State::Idle);
}

void loop() {
  sensors.updateOximeter();
  const unsigned long oximeterInterval = 20000;
  const unsigned long tempInterval = 20000;

  switch (deviceState.getCurrentState()) {
  case State::Idle: {
    ui.displayMessage("Pressione o Botao para Iniciar");
    if (ui.isButtonPressed()) {
      deviceState.setCollectingOximeter(true);
      deviceState.setOximeterStartTime(millis());
      deviceState.resetHeartRateSum();
      deviceState.resetSpO2Sum();
      deviceState.resetReadings();

      deviceState.setCurrentState(State::CollectingOximeter);
    }
    break;
  }

  case State::CollectingOximeter: {
    if (millis() - deviceState.getOximeterStartTime() < oximeterInterval) {
      float hr = sensors.getHeartRate();
      float spo2 = sensors.getSpO2();
      Serial.printf("HR: %.2f bpm\n", hr);
      Serial.printf("SpO2: %.2f %%\n", spo2);

      if (hr > 0 && spo2 > 0) {
        deviceState.addHeartRate(hr);
        deviceState.addSpO2(spo2);
        deviceState.incrementReadings();
      }

      ui.displayOximeterReadings(hr, spo2);
    }

    if (millis() - deviceState.getOximeterStartTime() > oximeterInterval) {

      if (deviceState.isCollectingOximeter()) {

        int readings = deviceState.getReadings();
        float heartRateAvg =
            (readings > 0) ? deviceState.getHeartRateSum() / readings : 0;
        float spO2Avg =
            (readings > 0) ? deviceState.getSpO2Sum() / readings : 0;

        deviceState.setHeartRate(heartRateAvg);
        deviceState.setSpO2(spO2Avg);

        Serial.printf("Average HR: %.2f bpm\n", deviceState.getHeartRate());
        Serial.printf("Average SpO2: %.2f %%\n", deviceState.getSpO2());

        deviceState.setCollectingOximeter(false);
      }

      if (deviceState.getReadings() == 0) {
        ui.displayMessage(
            "Erro ao Coletar Dados.\nPressione o botao para tentar "
            "novamente");
        if (ui.isButtonPressed()) {
          deviceState.setCurrentState(State::CollectingOximeter);
          deviceState.resetHeartRateSum();
          deviceState.resetSpO2Sum();
          deviceState.resetReadings();
          deviceState.setCollectingOximeter(true);
          deviceState.setOximeterStartTime(millis());
        }
      } else {
        ui.displayOximeterReadingsMedium(deviceState.getHeartRate(),
                                         deviceState.getSpO2());
        if (ui.isButtonPressed()) {
          deviceState.setCurrentState(State::CollectingTemperature);
          deviceState.setCollectingTemperature(true);
          deviceState.resetTemperatureSum();
          deviceState.resetTempReadings();
          deviceState.setTemperatureStartTime(millis());
        }
      }
    }
    break;
  }

  case State::CollectingTemperature: {
    if (millis() - deviceState.getTemperatureStartTime() < tempInterval) {
      float tempC = sensors.getTemperature();

      if (tempC > 0) {
        deviceState.addTemperature(tempC);
        deviceState.incrementTempReadings();
      }

      Serial.printf("Temperature Reading: %.2f °C\n", tempC);
      ui.displayTemperature(tempC);
    }

    if (millis() - deviceState.getTemperatureStartTime() > tempInterval) {
      if (deviceState.isCollectingTemperature()) {
        int tempReadings = deviceState.getTempReadings();
        float tempAvg = (tempReadings > 0)
                            ? deviceState.getTemperatureSum() / tempReadings
                            : 0;

        deviceState.setTemperature(tempAvg);

        Serial.printf("Average Temperature: %.2f °C\n",
                      deviceState.getTemperature());
        deviceState.setCollectingTemperature(false);
      }

      if (deviceState.getTempReadings() == 0) {
        ui.displayMessage(
            "Erro ao Coletar Dados.\nPressione o botao para tentar "
            "novamente");
        if (ui.isButtonPressed()) {
          deviceState.setCurrentState(State::CollectingTemperature);
          deviceState.resetTemperatureSum();
          deviceState.resetTempReadings();
          deviceState.setCollectingTemperature(true);
          deviceState.setTemperatureStartTime(millis());
        }
      } else {
        ui.displayTemperatureMedium(deviceState.getTemperature());

        if (ui.isButtonPressed()) {
          deviceState.setCurrentState(State::SendingData);
          deviceState.resetTempReadings();
          deviceState.resetTemperatureSum();
        }
      }
    }
    break;
  }

  case State::SendingData: {
    String jsonData = "{";
    jsonData += "\"device_id\":\"";
    jsonData += "apresentacao\"";
    jsonData += ",\"temperature\":";
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
        deviceState.resetReadings();
        deviceState.resetHeartRateSum();
        deviceState.resetSpO2Sum();
        deviceState.resetTemperatureSum();
        deviceState.resetTempReadings();
        deviceState.setThanksMessageStartTime(millis());
      } else {
        deviceState.setError("Erro ao Enviar Dados\nPressione o botao para "
                             "tentar novamente");
        deviceState.setCurrentState(State::Error);
      }
    } else {
      deviceState.setError("Wi-Fi nao conectado\nPressione o botao para "
                           "tentar novamente");
      deviceState.setCurrentState(State::Error);
    }
    break;
  }

  case State::DisplayResult: {
    ui.displayMessage("Dados Enviados com\n Sucesso! Obrigado!");
    if (millis() - deviceState.getThanksMessageStartTime() > 5000) {
      deviceState.setCurrentState(State::Idle);
      deviceState.clearThanksMessageStartTime();
    }
  }

  case State::Error: {
    ui.displayMessage(deviceState.getError());
    Serial.println(deviceState.getError());
    if (ui.isButtonPressed()) {
      deviceState.setCurrentState(State::Idle);
      deviceState.clearError();
    }
    break;
  }

  case State::Debug: {
    float hr = sensors.getHeartRate();
    float spo2 = sensors.getSpO2();
    float temp = sensors.getTemperature();
    auto wifiInfo = wifiManager.getWifiInfos();
    int buttonState = ui.isButtonPressed();

    ui.displayAllInfos(hr, spo2, temp, wifiInfo.ssid, wifiInfo.localIP,
                       wifiInfo.rssi, buttonState);
    break;
  }

  default: {
    deviceState.setCurrentState(State::Error);
    break;
  }
  }
}
