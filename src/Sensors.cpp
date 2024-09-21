#include "Sensors.h"
#include "Config.h"
#include <Arduino.h>

#define REPORTING_PERIOD_MS 1000

Sensors::Sensors() : oneWire(ONE_WIRE_BUS), tempSensor(&oneWire) {}

void Sensors::begin() {
  tempSensor.begin();
  Serial.print("Initializing pulse oximeter...");
  if (!pox.begin()) {
    Serial.println("FAILED");
  } else {
    Serial.println("SUCCESS");
  }
}

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
