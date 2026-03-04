#include <Arduino.h> // Include the arduino library for basic functions like pinMode, digitalWrite, and analogRead

#define LDR_PIN 4 // Pin that reads the LDR module
#define RELAY_PIN 5 // Pin that controls the relay module

int lightLevel = 0; // lightlevel to store the light level read from the LDR module
int threshold = 650; // threshold value to determine when to turn the light on or off

// setup() is a function that runs once when the system starts/resets. It is used to initialize the system.
void setup() {
  Serial.begin(115200); // Enable Serial (USB to ESP32S3) communication for debugging.
  pinMode(LDR_PIN, INPUT); // Set the LDR module pin as an input
  pinMode(RELAY_PIN, OUTPUT); // Set the relay module pin as an output
  Serial.println("Automatic Street Light System"); // Print to the Serial Monitor when the system starts
}

// loop() is an infinite loop that runs everything inside this function in order then starts again from the top.
void loop() {
  updateStreetLight(); // Update the street light status based on the current light level
  delay(1000); // Wait for 1 second before the next reading
}


/**
 * This function reads the light level from the LDR module and updates the relay status.
 */
void updateStreetLight() {

  // lightLevel reads the analog value from the LDR module
  lightLevel = analogRead(LDR_PIN);

  // Print to the Serial Monitor for debugging
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // Compare the lightLevel with the threshold to decide whether to turn the light on or off
  if (lightLevel > threshold) {
    digitalWrite(RELAY_PIN, HIGH); // Turn the relay on (light on)
    Serial.println("It's dark! Turning light on...");
  } else {
    digitalWrite(RELAY_PIN, LOW); // Turn the relay off (light off)
    Serial.println("It's bright! Turning light off...");
  }
}