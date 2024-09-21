#pragma once

#include <WiFi.h>

class WiFiManager {
public:
  void connectToWiFi(const char *ssid, const char *password);
  bool isConnected();
};
