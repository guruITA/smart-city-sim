#include "CommunicationHandler.h"
#include "NetworkController.h"

bool CommunicationHandler::request(const String& method, const String& endpoint,
                                   const String& body, String& response) {
  int httpCode;
  String url = NetworkController::buildUrl(endpoint);
  return NetworkController::request(url, method, body, httpCode, response);
}

bool CommunicationHandler::createTrain(int& trainId) {
  String response;
  if (!request("POST", "/api/v1/railroadcrossing/train/first", "{}", response))
    return false;

  int idIndex = response.indexOf("\"id\"");
  if (idIndex < 0) return false;

  int colon = response.indexOf(':', idIndex);
  int comma = response.indexOf(',', colon);
  if (comma < 0) comma = response.indexOf('}', colon);

  trainId = response.substring(colon + 1, comma).toInt();
  return trainId > 0;
}

bool CommunicationHandler::sendPrediction(int trainId, float seconds) {
  String body = "{\"predicted_arrival_seconds\": " + String(seconds, 2) + "}";
  String endpoint = "/api/v1/railroadcrossing/train/" + String(trainId) + "/second";

  String response;
  return request("PUT", endpoint, body, response);
}

bool CommunicationHandler::sendCrossed(int trainId) {
  String endpoint = "/api/v1/railroadcrossing/train/" + String(trainId) + "/crossed";
  String response;
  return request("PUT", endpoint, "{}", response);
}