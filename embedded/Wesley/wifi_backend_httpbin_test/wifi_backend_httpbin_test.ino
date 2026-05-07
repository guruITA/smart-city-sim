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

String createBackendJsonMessage(const SimulatedTrafficMessage& message) {
  messageCounter++;

  String payload = "{";
  payload += "\"sensorId\":\"north_1\",";
  payload += "\"direction\":\"north\",";
  payload += "\"phase\":\"";
  payload += message.phase;
  payload += "\",";
  payload += "\"interpretedState\":\"";
  payload += message.interpretedState;
  payload += "\",";
  payload += "\"timestampMs\":";
  payload += millis();
  payload += ",";
  payload += "\"valid\":";
  payload += message.valid ? "true" : "false";
  payload += ",";
  payload += "\"messageCounter\":";
  payload += messageCounter;
  payload += "}";

  return payload;
}

void printPayloadCheck(const String& payload) {
  Serial.println();
  Serial.println("[STEP 2] Backend-ready JSON payload created:");
  Serial.println(payload);

  Serial.println();
  Serial.println("[PAYLOAD FIELD CHECK]");
  Serial.print("contains sensorId: ");
  Serial.println(payload.indexOf("\"sensorId\"") >= 0 ? "yes" : "no");

  Serial.print("contains direction: ");
  Serial.println(payload.indexOf("\"direction\"") >= 0 ? "yes" : "no");

  Serial.print("contains phase: ");
  Serial.println(payload.indexOf("\"phase\"") >= 0 ? "yes" : "no");

  Serial.print("contains interpretedState: ");
  Serial.println(payload.indexOf("\"interpretedState\"") >= 0 ? "yes" : "no");

  Serial.print("contains timestampMs: ");
  Serial.println(payload.indexOf("\"timestampMs\"") >= 0 ? "yes" : "no");

  Serial.print("contains valid: ");
  Serial.println(payload.indexOf("\"valid\"") >= 0 ? "yes" : "no");
  Serial.println();
}

void printShortResponsePreview(const String& response) {
  const int MAX_PREVIEW_LENGTH = 1200;

  Serial.println();
  Serial.println("[SERVER RESPONSE PREVIEW]");
  if (response.length() <= MAX_PREVIEW_LENGTH) {
    Serial.println(response);
  } else {
    Serial.println(response.substring(0, MAX_PREVIEW_LENGTH));
    Serial.println();
    Serial.println("...response shortened in Serial Monitor...");
  }
  Serial.println();
}

bool responseContainsExpectedData(const String& response, const SimulatedTrafficMessage& message) {
  bool containsSensor = response.indexOf("north_1") >= 0;
  bool containsDirection = response.indexOf("north") >= 0;
  bool containsPhase = response.indexOf(message.phase) >= 0;
  bool containsState = response.indexOf(message.interpretedState) >= 0;

  Serial.println("[STEP 5] Response content check:");
  Serial.print("response contains sensorId north_1: ");
  Serial.println(containsSensor ? "yes" : "no");

  Serial.print("response contains direction north: ");
  Serial.println(containsDirection ? "yes" : "no");

  Serial.print("response contains phase: ");
  Serial.println(containsPhase ? "yes" : "no");

  Serial.print("response contains interpretedState: ");
  Serial.println(containsState ? "yes" : "no");

  return containsSensor && containsDirection && containsPhase && containsState;
}

void sendBackendMessageTest(const SimulatedTrafficMessage& message) {
  Serial.println();
  Serial.println("============================================================");
  Serial.println("NEW BACKEND POST TEST");
  Serial.println("============================================================");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WARNING] Wi-Fi is not connected.");
    Serial.println("Trying to reconnect before sending...");
    if (!connectToWiFi()) {
      Serial.println("[RESULT] Backend POST test FAILED because Wi-Fi is unavailable.");
      return;
    }
  }

  String payload = createBackendJsonMessage(message);
  printPayloadCheck(payload);

  Serial.println("[STEP 3] Starting HTTP POST request...");
  Serial.print("POST URL: ");
  Serial.println(BACKEND_URL);

  HTTPClient http;

  if (!http.begin(BACKEND_URL)) {
    Serial.println("[RESULT] HTTP begin FAILED.");
    Serial.println("Check the BACKEND_URL.");
    return;
  }

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");

  unsigned long startRequest = millis();
  int httpResponseCode = http.POST(payload);
  unsigned long requestDuration = millis() - startRequest;

  Serial.println();
  Serial.println("[STEP 4] HTTP POST finished.");
  Serial.print("HTTP response code: ");
  Serial.println(httpResponseCode);
  Serial.print("Request duration ms: ");
  Serial.println(requestDuration);

  if (httpResponseCode > 0) {
    String response = http.getString();

    Serial.println("[RESULT] Server response was received.");
    Serial.print("Response length: ");
    Serial.println(response.length());

    printShortResponsePreview(response);

    bool contentOk = responseContainsExpectedData(response, message);

    Serial.println();
    Serial.println("[FINAL TEST RESULT]");
    if (httpResponseCode == 200 && contentOk) {
      Serial.println("PASSED");
      Serial.println("The ESP32-S3 connected to the internet, sent the backend-ready message, and received the echoed response.");
    } else if (httpResponseCode == 200) {
      Serial.println("PARTLY PASSED");
      Serial.println("The ESP32-S3 received HTTP 200, but the response did not contain all expected fields.");
      Serial.println("Check the response preview above.");
    } else {
      Serial.println("PARTLY PASSED");
      Serial.println("The ESP32-S3 received a response, but the HTTP status was not 200.");
      Serial.println("Check the response code and response preview.");
    }
  } else {
    Serial.println("[RESULT] HTTP request FAILED before a useful server response was received.");
    Serial.print("HTTPClient error text: ");
    Serial.println(http.errorToString(httpResponseCode));
    Serial.println();
    Serial.println("Check:");
    Serial.println("- Wi-Fi connection");
    Serial.println("- internet access");
    Serial.println("- DNS access");
    Serial.println("- whether the network blocks HTTP requests");
    Serial.println("- whether the test endpoint is reachable");
  }

  http.end();

  Serial.println("============================================================");
  Serial.println();
}

// ============================================================
// Setup
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  printHeader();

  if (!connectToWiFi()) {
    Serial.println("The sketch will keep running and will retry before each POST test.");
  }

  lastPostTime = millis() - POST_INTERVAL;
}

// ============================================================
// Main loop
// ============================================================

void loop() {
  unsigned long now = millis();

  if ((now - lastPostTime) >= POST_INTERVAL) {
    lastPostTime = now;

    SimulatedTrafficMessage message = testMessages[currentMessageIndex];
    sendBackendMessageTest(message);

    currentMessageIndex++;
    if (currentMessageIndex >= NUMBER_OF_TEST_MESSAGES) {
      currentMessageIndex = 0;
    }
  }
}
