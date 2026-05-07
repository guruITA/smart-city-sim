#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

/*
  Smart traffic-light controller

  Hardware:
  - ESP32-S3
  - MCP23017 I2C output expander
  - 2x ULN2803 driver IC
  - KY-021 vehicle sensor on GPIO 6

  Backend message format:
  {
    "sensorId": "north_1",
    "direction": "north",
    "phase": "EW_GREEN",
    "interpretedState": "waiting_vehicle",
    "timestampMs": 123456,
    "valid": true
  }
*/

// ============================================================
// Wi-Fi and backend settings
// ============================================================

const char* BACKEND_URL = "http://httpbin.org/post";

const unsigned long WIFI_CONNECT_TIMEOUT = 20000;
const unsigned long HTTP_TIMEOUT = 5000;

