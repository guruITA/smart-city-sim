#include "Config.h"
#include "NetworkController.h"
#include "EinkDisplay.h"

#define builtin LED_BUILTIN


EinkDisplay eink(
  Config::EinkDisplay::CLK_PIN,
  Config::EinkDisplay::MOSI_PIN,
  Config::EinkDisplay::CS_PIN,
  Config::EinkDisplay::DC_PIN,
  Config::EinkDisplay::RST_PIN,
  Config::EinkDisplay::BUSY_PIN
);
void setup() {
  pinMode(builtin, OUTPUT);
  digitalWrite(builtin, LOW);

  Serial.begin(115200);
  Serial.println("Setup start");

  if (NetworkController::begin(Config::Network::WIFI_SSID, Config::Network::WIFI_PASSWORD)) {
    Serial.println("WiFi connected, network fetch availability up");
  } else {
    Serial.println("WiFi not connected, some network features will be skipped");
  }

  NetworkController::setApiBaseUrl(Config::Network::API_BASE_URL);
  
  eink.begin();
  eink.startSyncTask();
}

void loop() {
}