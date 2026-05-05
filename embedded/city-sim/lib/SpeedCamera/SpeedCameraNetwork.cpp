#include "SpeedCameraNetwork.h"

#include "NetworkController.h"

bool SpeedCameraNetwork::httpRequest(const String& method, const String& endpoint,
                                     const String& body, int& httpCode, String& responseBody) {
  String url = NetworkController::buildUrl(endpoint);

  Serial.printf("SpeedCameraNetwork HTTP %s %s\n", method.c_str(), url.c_str());

  bool ok = NetworkController::request(url, method, body, httpCode, responseBody);

  if (!ok) {
    Serial.printf("SpeedCameraNetwork::httpRequest failed, httpCode=%d\n", httpCode);
    return false;
  }

  if (httpCode < 200 || httpCode >= 300) {
    Serial.printf("SpeedCameraNetwork backend returned HTTP %d\n", httpCode);
    Serial.println(responseBody);
    return false;
  }

  return true;
}

bool SpeedCameraNetwork::sendMeasurement(float speedKmh, const String& direction, bool isViolation,
                                         float speedLimitKmh) {
  if (!NetworkController::connected()) {
    Serial.println("SpeedCameraNetwork skipped: WiFi offline.");
    return false;
  }

  int httpCode = -1;
  String responseBody;

  String body = "{";
  body += "\"speed_kmh\":";
  body += String(speedKmh, 2);
  body += ",";
  body += "\"direction\":\"";
  body += direction;
  body += "\",";
  body += "\"is_violation\":";
  body += isViolation ? "true" : "false";
  body += ",";
  body += "\"speed_limit_kmh\":";
  body += String(speedLimitKmh, 2);
  body += "}";

  bool ok = httpRequest("POST", "/api/v1/speedcamera/", body, httpCode, responseBody);

  if (!ok) {
    Serial.println("SpeedCameraNetwork sendMeasurement failed.");
    return false;
  }

  Serial.println("SpeedCameraNetwork sendMeasurement succeeded.");
  Serial.print("Backend response: ");
  Serial.println(responseBody);

  return true;
}