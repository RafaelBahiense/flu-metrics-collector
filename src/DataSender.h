#pragma once

#include <HTTPClient.h>

class DataSender {
public:
  bool sendData(const char *serverName, const String &jsonData);
};
