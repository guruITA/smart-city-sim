#include "TrainPredictionSignal.h"
#include "NetworkController.h"

bool TrainPredictionSignal::httpRequest(const String& method, const String& endpoint, const String& body, int& httpCode, String& responseBody) {
  String url = NetworkController::buildUrl(endpoint);
  Serial.printf("TrainPredictionSignal HTTP %s %s\n", method.c_str(), url.c_str());
  bool ok = NetworkController::request(url, method, body, httpCode, responseBody);
  if (!ok) {
    Serial.printf("TrainPredictionSignal::httpRequest failed, httpCode=%d\n", httpCode);
    return false;
  }
  return true;
}

bool TrainPredictionSignal::createTrain() {
  int httpCode;
  String response;

  if (!httpRequest("POST", "/api/v1/railroadcrossing/train/first", "{}", httpCode, response)) {
    Serial.println("createTrain failed");
    return false;
  }

  int idIndex = response.indexOf("\"id\"");
  if (idIndex < 0) {
    Serial.println("createTrain: no id returned");
    return false;
  }

  int colon = response.indexOf(':', idIndex);
  if (colon < 0) {
    Serial.println("createTrain: malformed response");
    return false;
  }

  int comma = response.indexOf(',', colon);
  if (comma < 0) comma = response.indexOf('}', colon);
  if (comma < 0) {
    Serial.println("createTrain: malformed response 2");
    return false;
  }

  String idText = response.substring(colon + 1, comma);
  idText.trim();
  int id = idText.toInt();
  if (id <= 0) {
    Serial.println("createTrain: invalid id");
    return false;
  }

  _trainId = id;
  Serial.printf("createTrain succeeded -> trainId=%d\n", _trainId);
  return true;
}

bool TrainPredictionSignal::updateTrainSecondSensor(float predictedSeconds) {
  if (_trainId <= 0) {
    Serial.println("updateTrainSecondSensor skipped: no trainId");
    return false;
  }

  int httpCode;
  String response;
  String endpoint = "/api/v1/railroadcrossing/train/" + String(_trainId) + "/second";
  String body = "{\"predicted_arrival_seconds\": " + String(predictedSeconds, 2) + "}";

  if (!httpRequest("PUT", endpoint, body, httpCode, response)) {
    Serial.println("updateTrainSecondSensor failed");
    return false;
  }

  Serial.println("updateTrainSecondSensor succeeded");
  return true;
}

bool TrainPredictionSignal::updateTrainCrossed() {
  if (_trainId <= 0) {
    Serial.println("updateTrainCrossed skipped: no trainId");
    return false;
  }

  int httpCode;
  String response;
  String endpoint = "/api/v1/railroadcrossing/train/" + String(_trainId) + "/crossed";

  if (!httpRequest("PUT", endpoint, "{}", httpCode, response)) {
    Serial.println("updateTrainCrossed failed");
    return false;
  }

  Serial.printf("updateTrainCrossed succeeded for trainId=%d\n", _trainId);
  return true;
}
