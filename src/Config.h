#pragma once

// Wi-Fi settings
constexpr char WIFI_SSID[] = "MyWifi";
constexpr char WIFI_PASSWORD[] = "MyPassword";

// API settings
constexpr char SERVER_NAME[] = "https://myapi.com/api/v1/data";

// Pin definitions
constexpr int ONE_WIRE_BUS = 2; // DS18B20 data pin
constexpr int BUTTON_PIN = 4;   // Button pin

constexpr int COLLECTION_INTERVAL =
    10000; // Data collection interval in milliseconds
