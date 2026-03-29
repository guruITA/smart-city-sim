#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include <Arduino.h>
#include <esp_event.h>
#include <Network.h>
#include <WiFi.h>
#include <HTTPClient.h>

class NetworkController {
public:
  static bool begin(const char* ssid, const char* password, unsigned long timeoutMs = 15000);
  static bool connected();
  static void disconnect();

  static void setApiBaseUrl(const String& apiBaseUrl);
  static String getApiBaseUrl();
  static String buildUrl(const String& endpoint);

  static String fetch(const String& url, int& httpCode);

  static bool request(const String& url, const String& method, const String& body, int& httpCode, String& responseBody);

private:
  NetworkController() = delete;
  static String _apiBaseUrl;
};

#endif
