#include "ParkingNetwork.h"

#include "NetworkController.h"

bool ParkingNetwork::httpRequest(const String& method, const String& endpoint, const String& body,
                                 int& httpCode, String& responseBody) {
  String url = NetworkController::buildUrl(endpoint);

  Serial.printf("ParkingNetwork HTTP %s %s\n", method.c_str(), url.c_str());

  bool ok = NetworkController::request(url, method, body, httpCode, responseBody);

  if (!ok) {
    Serial.printf("ParkingNetwork::httpRequest failed, httpCode=%d\n", httpCode);
    return false;
  }

  if (httpCode < 200 || httpCode >= 300) {
    Serial.printf("ParkingNetwork backend returned HTTP %d\n", httpCode);
    Serial.println(responseBody);
    return false;
  }

  return true;
}

bool ParkingNetwork::sendSpotUpdate(uint8_t spotNumber, bool isOccupied) {
  if (!NetworkController::connected()) {
    Serial.println("ParkingNetwork skipped: WiFi offline.");
    return false;
  }

  int httpCode = -1;
  String responseBody;

  String endpoint = "/api/v1/parking/update/";
  endpoint += String(spotNumber);
  endpoint += "?is_occupied=";
  endpoint += isOccupied ? "true" : "false";

  bool ok = httpRequest("POST", endpoint, "", httpCode, responseBody);

  if (!ok) {
    Serial.println("ParkingNetwork sendSpotUpdate failed.");
    return false;
  }

  Serial.println("ParkingNetwork sendSpotUpdate succeeded.");
  Serial.print("Backend response: ");
  Serial.println(responseBody);

  return true;
}