#include "Streetlight.h"
#include "OverrideController.h"

StreetLight::StreetLight(int ldrPin, int relayPin, int threshold, int interval) {
  _ldrPin = ldrPin;
  _relayPin = relayPin;
  _threshold = threshold;
  _interval = interval; 
  _previousMillis = 0;
}

void StreetLight::begin() {
  Serial.begin(115200);
  pinMode(_ldrPin, INPUT);
  pinMode(_relayPin, OUTPUT);
}

void StreetLight::update() {
  // Backend override: force full lighting for visibility during an emergency,
  // ignoring the LDR until the override is cleared.
  if (OverrideController::isCommand("streetlight", "force_on")) {
    digitalWrite(_relayPin, HIGH);
    return;
  }

  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _interval) {
    _previousMillis = currentMillis;

    int lightLevel = analogRead(_ldrPin);

    if (lightLevel > _threshold) {
      digitalWrite(_relayPin, HIGH);
    } else {
      digitalWrite(_relayPin, LOW);
    }
  }
}