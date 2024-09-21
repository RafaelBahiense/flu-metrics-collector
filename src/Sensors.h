#pragma once

#include "MAX30100_PulseOximeter.h"
#include <DallasTemperature.h>
#include <OneWire.h>

class Sensors {
public:
  Sensors();
  void begin();
  float getTemperature();
  void updateOximeter();
  float getHeartRate();
  float getSpO2();

private:
  OneWire oneWire;
  DallasTemperature tempSensor;
  PulseOximeter pox;
};
