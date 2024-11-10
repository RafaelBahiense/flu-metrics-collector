#include "WiFiManager.h"
#include <Arduino.h>

void WiFiManager::connectToWiFi(const char *ssid, const char *password) {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

bool WiFiManager::isConnected() { return WiFi.status() == WL_CONNECTED; }

WifiInfo WiFiManager::getWifiInfos() {
  WifiInfo wifiInfo;
  wifiInfo.ssid = WiFi.SSID();
  wifiInfo.localIP = WiFi.localIP().toString();
  wifiInfo.rssi = WiFi.RSSI();
  return wifiInfo;
}
