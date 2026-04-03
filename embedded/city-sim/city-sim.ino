#include "NetworkController.h"
#include "Streetlight.h"
#include "TrainPredictionSignal.h"
#include "SpeedCamera.h"

#define builtin LED_BUILTIN

//  WiFi details
const char* WIFI_SSID = "ESP32CAM_CAPTURE";
const char* WIFI_PASSWORD = "12345678";

// backend URL
const String API_BASE_URL = "http://127.0.0.1:8000";

StreetLight lamp(4, 5, 650, 1000);
TrainPredictionSignal trainSignal(37, 36, 42, 18, 45, 200, 1000, 5000, 1000, 8, 0, 90);
SpeedCamera speedCamera(6, 12, 17, 46, 128, 64, 0x3C, LOW, 0.10f, 1.0f, 2000000UL, 500, 2000, "http://192.168.4.1/capture");

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
  speedCamera.begin();  
}

void loop() {
  lamp.update();
  trainSignal.update();
  speedCamera.update();
}