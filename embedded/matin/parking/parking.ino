/**
 * Smart Parking Sensor
 * ESP32-S3 + HC-SR04 Ultrasonic Sensor
 *
 * Measures distance to detect if a parking spot is occupied.
 * Sends data via WiFi HTTP POST to the City Sim backend API.
 *
 * Author: Matin Khajehfard
 * Project: City Sim - The Embedded Alliance
 * Tile: Parking
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

// --- WiFi config ---
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// --- API config ---
// Change this to the IP of the machine running docker compose
const char* API_BASE_URL = "http://192.168.1.100:8000";
const int SPOT_NUMBER = 1;

// --- Ultrasonic sensor pins ---
#define TRIG_PIN 5
#define ECHO_PIN 6

// --- LED indicators ---
#define LED_GREEN 7
#define LED_RED 8

// --- Settings ---
const float OCCUPIED_THRESHOLD_CM = 10.0;
const unsigned long SEND_INTERVAL_MS = 2000;  // Send every 2 seconds

unsigned long lastSendTime = 0;

/**
 * Measure distance with HC-SR04 ultrasonic sensor.
 * Returns distance in centimeters.
 */
float measureDistance() {
  // Send trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms timeout

  // No echo received
  if (duration == 0) {
    return -1.0;
  }

  // Convert to cm: speed of sound = 343 m/s = 0.0343 cm/us
  // Divide by 2 because sound travels to object and back
  float distance = (duration * 0.0343) / 2.0;
  return distance;
}

/**
 * Update LED indicators based on spot status.
 */
void updateLeds(bool occupied) {
  if (occupied) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
}

/**
 * Send distance reading to the backend API.
 */
void sendToApi(float distance) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping API call");
    return;
  }

  HTTPClient http;
  String url = String(API_BASE_URL) + "/api/v1/parking/update/" + String(SPOT_NUMBER) + "?distance_cm=" + String(distance, 2);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int responseCode = http.POST("");

  if (responseCode > 0) {
    Serial.print("API response: ");
    Serial.println(responseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("API error: ");
    Serial.println(responseCode);
  }

  http.end();
}

/**
 * Connect to WiFi network.
 */
void connectWifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("WiFi connection failed. Running in offline mode.");
  }
}

void setup() {
  Serial.begin(115200);

  // Sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // LED pins
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Connect to WiFi
  connectWifi();

  Serial.println("Smart Parking Sensor started");
  Serial.print("Spot number: ");
  Serial.println(SPOT_NUMBER);
}

void loop() {
  float distance = measureDistance();

  if (distance < 0) {
    Serial.println("Sensor read error");
    return;
  }

  bool occupied = distance < OCCUPIED_THRESHOLD_CM;

  // Update LEDs immediately (local feedback)
  updateLeds(occupied);

  // Print to serial for debugging
  Serial.print("Distance: ");
  Serial.print(distance, 2);
  Serial.print(" cm | Status: ");
  Serial.println(occupied ? "OCCUPIED" : "FREE");

  // Send to API at interval
  if (millis() - lastSendTime >= SEND_INTERVAL_MS) {
    lastSendTime = millis();
    sendToApi(distance);
  }

  delay(100);
}
