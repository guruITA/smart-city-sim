/*
  File: wifi_backend_httpbin_test.ino

  Purpose:
  Standalone Wi-Fi + internet + backend-message test for the smart traffic-light project.

  What this test proves:
  - The ESP32-S3 can connect to Wi-Fi.
  - The ESP32-S3 can reach the internet.
  - The ESP32-S3 can send a backend-ready JSON message with HTTP POST.
  - The ESP32-S3 can receive and inspect the response from the server.
  - This works before you have your own backend.

  Test backend used:
  - http://httpbin.org/post
  - httpbin echoes the HTTP POST request back in its response.
  - Later, replace BACKEND_URL with your real backend endpoint.

  Serial monitor:
  - 115200 baud

  Required:
  - ESP32 board package installed in Arduino IDE
  - WiFi.h and HTTPClient.h are included with the ESP32 Arduino core

  IMPORTANT:
  - Most ESP32 boards connect to 2.4 GHz Wi-Fi, not 5 GHz-only networks.
  - Do not commit real Wi-Fi passwords to Git.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

// ============================================================
// Wi-Fi settings
// ============================================================

// Replace these with your Wi-Fi name and password.
//const char* WIFI_SSID = "Galaxy S25+ D04F";
//const char* WIFI_PASSWORD = "p9muu22rbytfe28";

// Public echo endpoint used while you do not have your own backend yet.
// Later, replace this with something like:
// const char* BACKEND_URL = "https://your-backend-domain.com/api/traffic-state";
const char* BACKEND_URL = "http://httpbin.org/post";

// ============================================================
// Test timing
// ============================================================

const unsigned long WIFI_CONNECT_TIMEOUT = 20000;
const unsigned long POST_INTERVAL = 15000;

// ============================================================
// Simulated traffic states for backend-message testing
// ============================================================

struct SimulatedTrafficMessage {
  const char* phase;
  const char* interpretedState;
  bool valid;
};

SimulatedTrafficMessage testMessages[] = {
  { "ROAD12_GREEN",  "no_vehicle",      true  },
  { "ROAD34_GREEN",  "waiting_vehicle", true  },
  { "ROAD12_GREEN",  "passing_vehicle", true  },
  { "ROAD12_YELLOW", "unclear_input",   false }
};

const int NUMBER_OF_TEST_MESSAGES = sizeof(testMessages) / sizeof(testMessages[0]);

int currentMessageIndex = 0;
unsigned long lastPostTime = 0;
unsigned long messageCounter = 0;

// ============================================================
// Helper functions
// ============================================================

void printHeader() {
  Serial.println();
  Serial.println("============================================================");
  Serial.println("WIFI + INTERNET + BACKEND MESSAGE TEST START");
  Serial.println("============================================================");
  Serial.println("Goal:");
  Serial.println("- Connect the ESP32-S3 to Wi-Fi.");
  Serial.println("- Send a backend-ready JSON message to a public test endpoint.");
  Serial.println("- Receive the server response and check whether the message came back.");
  Serial.println();
  Serial.println("Test backend:");
  Serial.println(BACKEND_URL);
  Serial.println();
  Serial.println("Testing steps shown in Serial Monitor:");
  Serial.println("1. Wi-Fi connection");
  Serial.println("2. Local IP address");
  Serial.println("3. JSON payload creation");
  Serial.println("4. HTTP POST request");
  Serial.println("5. HTTP response code");
  Serial.println("6. Response body check");
  Serial.println("============================================================");
  Serial.println();
}

void printWiFiStatus() {
  Serial.println();
  Serial.println("---------------- WIFI STATUS ----------------");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);

  Serial.print("WiFi.status(): ");
  Serial.println(WiFi.status());

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connection result: CONNECTED");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS IP: ");
    Serial.println(WiFi.dnsIP());
    Serial.print("Signal strength RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("Connection result: NOT CONNECTED");
  }

  Serial.println("---------------------------------------------");
  Serial.println();
}

bool connectToWiFi() {
  Serial.println("[STEP 1] Connecting to Wi-Fi...");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED && (millis() - startAttempt) < WIFI_CONNECT_TIMEOUT) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[RESULT] Wi-Fi connection PASSED.");
    printWiFiStatus();
    return true;
  }

  Serial.println("[RESULT] Wi-Fi connection FAILED.");
  Serial.println("Check:");
  Serial.println("- Wi-Fi name and password");
  Serial.println("- 2.4 GHz Wi-Fi availability");
  Serial.println("- board is close enough to the router");
  Serial.println("- phone hotspot is allowed to accept new devices");
  printWiFiStatus();
  return false;
}