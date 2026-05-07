#include "NetworkController.h"

String NetworkController::_apiBaseUrl = "";

void NetworkController::setApiBaseUrl(const String& apiBaseUrl) {
  _apiBaseUrl = apiBaseUrl;
  if (!_apiBaseUrl.endsWith("/")) {
    _apiBaseUrl += "/";
  }
}

String NetworkController::getApiBaseUrl() {
  return _apiBaseUrl;
}

String NetworkController::buildUrl(const String& endpoint) {
  if (_apiBaseUrl.length() == 0 || endpoint.length() == 0) {
    return endpoint;
  }

  if (endpoint.startsWith("http://") || endpoint.startsWith("https://")) {
    return endpoint;
  }

  String normalized = _apiBaseUrl;
  if (endpoint.startsWith("/")) {
    normalized += endpoint.substring(1);
  } else {
    normalized += endpoint;
  }
  return normalized;
}

bool NetworkController::begin(const char* ssid, const char* password, unsigned long timeoutMs) {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Network already connected");
    return true;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long start = millis();
  Serial.print("Connecting to WiFi");
  while (millis() - start < timeoutMs) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi connected");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      return true;
    }
    delay(500);
    Serial.print('.');
  }

  Serial.println("\nWiFi connection failed");
  return false;
}

bool NetworkController::connected() {
  return (WiFi.status() == WL_CONNECTED);
}

void NetworkController::disconnect() {
  WiFi.disconnect();
}

String NetworkController::fetch(const String& url, int& httpCode) {
  httpCode = -1;
  if (!connected()) {
    Serial.println("NetworkController::fetch failed: not connected");
    return String();
  }

  HTTPClient http;
  http.begin(url);
  httpCode = http.GET();

  if (httpCode <= 0) {
    Serial.printf("HTTP request failed, error=%d\n", httpCode);
    http.end();
    return String();
  }

  String payload = http.getString();
  Serial.printf("HTTP %d response len=%u\n", httpCode, payload.length());
  http.end();
  return payload;
}

bool NetworkController::request(const String& url, const String& method, const String& body, int& httpCode, String& responseBody) {
  httpCode = -1;
  if (!connected()) {
    Serial.println("NetworkController::request failed: not connected");
    responseBody = String();
    return false;
  }

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  if (method == "GET") {
    httpCode = http.GET();
  } else if (method == "POST") {
    httpCode = http.POST(body);
  } else if (method == "PUT") {
    httpCode = http.PUT(body);
  } else {
    Serial.printf("NetworkController::request unsupported method %s\n", method.c_str());
    http.end();
    return false;
  }

  if (httpCode <= 0) {
    Serial.printf("NetworkController::request failed, error=%d\n", httpCode);
    responseBody = String();
    http.end();
    return false;
  }

  responseBody = http.getString();
  Serial.printf("NetworkController::request HTTP %d response len=%u\n", httpCode, responseBody.length());
  http.end();
  return true;
}
