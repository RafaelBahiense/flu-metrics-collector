#pragma once

class State {
public:
  enum DeviceState {
    Idle,
    CollectingOximeter,
    CollectingTemperature,
    SendingData,
    DisplayResult,
    Error
  };

  State();

  DeviceState getCurrentState() const;
  void setCurrentState(DeviceState state);

  bool isCollectingOximeter() const;
  void setCollectingOximeter(bool value);

  bool isCollectingTemperature() const;
  void setCollectingTemperature(bool value);

  unsigned long getOximeterStartTime() const;
  void setOximeterStartTime(unsigned long value);

  unsigned long getTemperatureStartTime() const;
  void setTemperatureStartTime(unsigned long value);

  float getHeartRateSum() const;
  void addHeartRate(float value);
  void resetHeartRateSum();

  float getSpO2Sum() const;
  void addSpO2(float value);
  void resetSpO2Sum();

  float getTemperatureSum() const;
  void addTemperature(float value);
  void resetTemperatureSum();

  int getReadings() const;
  void incrementReadings();
  void resetReadings();

  int getTempReadings() const;
  void incrementTempReadings();
  void resetTempReadings();

  float getHeartRate() const;
  void setHeartRate(float value);

  float getSpO2() const;
  void setSpO2(float value);

  float getTemperature() const;
  void setTemperature(float value);

private:
  DeviceState currentState;

  bool collectingOximeter;
  bool collectingTemperature;
  unsigned long oximeterStartTime;
  unsigned long temperatureStartTime;

  float heartRateSum;
  float spO2Sum;
  float temperatureSum;

  int readings;
  int tempReadings;

  float heartRate;
  float spO2;
  float temperature;
};
