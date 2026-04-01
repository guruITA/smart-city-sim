#include "NetworkController.h"
#include "Streetlight.h"
#include "TrainDetector.h"
#include "SignalController.h"
#include "CommunicationHandler.h"
#include "CrossingController.h"

#define builtin LED_BUILTIN

// WiFi details
const char* WIFI_SSID = "";
const char* WIFI_PASSWORD = "";

// backend URL
const String API_BASE_URL = "";

StreetLight lamp(4, 5, 650, 1000);

// Railroad crossing tile
TrainDetector detector(45, 200, 1000);
SignalController signalController(37, 36, 42, 18);
CommunicationHandler comm;
CrossingController crossing(detector, signalController, comm);

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
  crossing.begin();
}

void loop() {
  lamp.update();
  crossing.update();
}