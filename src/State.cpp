#include "State.h"

State::State()
    : currentState(Idle), collectingOximeter(false),
      collectingTemperature(false), oximeterStartTime(0),
      temperatureStartTime(0), heartRateSum(0.0f), spO2Sum(0.0f),
      temperatureSum(0.0f), readings(0), tempReadings(0), heartRate(0.0f),
      spO2(0.0f), temperature(0.0f) {}

State::DeviceState State::getCurrentState() const { return currentState; }

void State::setCurrentState(DeviceState state) { currentState = state; }

bool State::isCollectingOximeter() const { return collectingOximeter; }

void State::setCollectingOximeter(bool value) { collectingOximeter = value; }

bool State::isCollectingTemperature() const { return collectingTemperature; }

void State::setCollectingTemperature(bool value) {
  collectingTemperature = value;
}

unsigned long State::getOximeterStartTime() const { return oximeterStartTime; }

void State::setOximeterStartTime(unsigned long value) {
  oximeterStartTime = value;
}

unsigned long State::getTemperatureStartTime() const {
  return temperatureStartTime;
}

void State::setTemperatureStartTime(unsigned long value) {
  temperatureStartTime = value;
}

float State::getHeartRateSum() const { return heartRateSum; }

void State::addHeartRate(float value) { heartRateSum += value; }

void State::resetHeartRateSum() { heartRateSum = 0.0f; }

float State::getSpO2Sum() const { return spO2Sum; }

void State::addSpO2(float value) { spO2Sum += value; }

void State::resetSpO2Sum() { spO2Sum = 0.0f; }

float State::getTemperatureSum() const { return temperatureSum; }

void State::addTemperature(float value) { temperatureSum += value; }

void State::resetTemperatureSum() { temperatureSum = 0.0f; }

int State::getReadings() const { return readings; }

void State::incrementReadings() { readings++; }

void State::resetReadings() { readings = 0; }

int State::getTempReadings() const { return tempReadings; }

void State::incrementTempReadings() { tempReadings++; }

void State::resetTempReadings() { tempReadings = 0; }

float State::getHeartRate() const { return heartRate; }

void State::setHeartRate(float value) { heartRate = value; }

float State::getSpO2() const { return spO2; }

void State::setSpO2(float value) { spO2 = value; }

float State::getTemperature() const { return temperature; }

void State::setTemperature(float value) { temperature = value; }

void State::setError(const char *message) { errorMessage = message; }

void State::clearError() { errorMessage = nullptr; }

const char *State::getError() const { return errorMessage; }

unsigned long State::getThanksMessageStartTime() const {
  return thanksMessageStartTime;
}

void State::setThanksMessageStartTime(unsigned long value) {
  thanksMessageStartTime = value;
}

void State::clearThanksMessageStartTime() { thanksMessageStartTime = 0; }
