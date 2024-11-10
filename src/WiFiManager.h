#pragma once

#include <WiFi.h>

struct WifiInfo {
  String ssid;
  String localIP;
  int rssi;
};

class WiFiManager {
public:
  void connectToWiFi(const char *ssid, const char *password);
  bool isConnected();
  WifiInfo getWifiInfos();
};
