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