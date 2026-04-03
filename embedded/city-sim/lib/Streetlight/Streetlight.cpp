#include "StreetLight.h"

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
  Serial.println("Automatic Street Light System");
}

void StreetLight::update() {
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _interval) {
    _previousMillis = currentMillis;

    int lightLevel = analogRead(_ldrPin);

    Serial.print("Light Level: ");
    Serial.println(lightLevel);

    if (lightLevel > _threshold) {
      digitalWrite(_relayPin, HIGH);
      Serial.println("It's dark! Turning light on...");
    } else {
      digitalWrite(_relayPin, LOW);
      Serial.println("It's bright! Turning light off...");
    }
  }
}