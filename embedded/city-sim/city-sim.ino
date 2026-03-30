#include "NetworkController.h"
#include "Streetlight.h"
#include "TrainPredictionSignal.h"

#define builtin LED_BUILTIN

//  WiFi details
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

// backend URL
const String API_BASE_URL = "http://127.0.0.1:8000";

StreetLight lamp(4, 5, 650, 1000);
TrainPredictionSignal trainSignal(37, 36, 42, 18, 45, 200, 1000, 5000, 1000, 8, 0, 90);

void setup() {
  pinMode(builtin, OUTPUT);
  digitalWrite(builtin, LOW);

  Serial.begin(115200);
  Serial.println("Setup start");

  if (NetworkController::begin(WIFI_SSID, WIFI_PASSWORD)) {
    Serial.println("WiFi connected, network fetch availability up");
  } else {
    Serial.println("WiFi not connected, some network features will be skipped");
  }

  NetworkController::setApiBaseUrl(API_BASE_URL);

  lamp.begin();
  trainSignal.begin();
}

void loop() {
  lamp.update();
  trainSignal.update();
}