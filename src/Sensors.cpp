#include "Sensors.h"
#include "Config.h"
#include <Arduino.h>

Sensors::Sensors() {
  oneWire = OneWire(ONE_WIRE_BUS);
  tempSensor = DallasTemperature(&oneWire);
}

int Sensors::begin() {
  tempSensor.begin();
  Serial.print("Initializing pulse oximeter...");
  if (!pox.begin()) {
    Serial.println("FAILED");
    return 1;
  } else {
    Serial.println("SUCCESS");
    return 0;
  }
  pox.setOnBeatDetectedCallback(Sensors::onBeatDetected);
}

void Sensors::onBeatDetected() { Serial.println("Beat!"); }

float Sensors::getTemperature() {
  tempSensor.requestTemperatures();
  float tempC = tempSensor.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C) {
    return tempC;
  } else {
    Serial.println("Error: Could not read temperature data");
    return NAN;
  }
}

void Sensors::updateOximeter() { pox.update(); }

float Sensors::getHeartRate() { return pox.getHeartRate(); }

float Sensors::getSpO2() { return pox.getSpO2(); }
