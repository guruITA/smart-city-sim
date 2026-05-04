#include "Streetlight.h"

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