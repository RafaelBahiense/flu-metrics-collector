#pragma once

#include "MAX30100_PulseOximeter.h"
#include <DallasTemperature.h>
#include <OneWire.h>

#define REPORTING_PERIOD_MS 100

class Sensors {
public:
  Sensors();
  int begin();
  float getTemperature();
  void updateOximeter();
  float getHeartRate();
  float getSpO2();

private:
  OneWire oneWire;
  DallasTemperature tempSensor;
  PulseOximeter pox;

  static void onBeatDetected();
};
