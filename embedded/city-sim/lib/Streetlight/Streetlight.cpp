#include "Streetlight.h"

/**
 * @brief Creates a streetlight controller.
 *
 * Stores the pins, light threshold, and update interval.
 */
StreetLight::StreetLight(int ldrPin, int relayPin, int threshold, int interval)
    : _ldrPin(ldrPin),
      _relayPin(relayPin),
      _threshold(threshold),
      _interval(interval),
      _previousMillis(0) {

      }

/**
 * @brief Initializes the streetlight hardware.
 *
 * Sets up serial communication, the LDR pin, and the relay pin.
 */
void StreetLight::begin() {

  // Start serial communication.
  Serial.begin(115200);

  // Set the LDR pin as input.
  pinMode(_ldrPin, INPUT);

  // Set the relay pin as output.
  pinMode(_relayPin, OUTPUT);
}

/**
 * @brief Updates the streetlight state.
 *
 * Reads the LDR sensor after each interval and switches the relay
 * based on the light threshold.
 */
void StreetLight::update() {

  // Get the current time in milliseconds.
  unsigned long currentMillis = millis();

  // Check if it is time to read the light sensor again.
  if (currentMillis - _previousMillis >= _interval) {

    // Store the time of this update.
    _previousMillis = currentMillis;

    // Read the current light level from the LDR sensor.
    int lightLevel = analogRead(_ldrPin);

    // Turn the relay on when the measured light level is above the threshold.
    if (lightLevel > _threshold) {
      digitalWrite(_relayPin, HIGH);
    } else {

      // Turn the relay off when the measured light level is under the threshold.
      digitalWrite(_relayPin, LOW);
    }
  }
}