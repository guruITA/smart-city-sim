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

// ============================================================
// Vehicle sensor settings
// ============================================================

const int VEHICLE_SENSOR_PIN = 6;

// KY-021 reed-switch modules commonly work as active LOW with INPUT_PULLUP.
const int SENSOR_ACTIVE_LEVEL = LOW;
const unsigned long SENSOR_DEBOUNCE_TIME = 50;
const unsigned long SENSOR_STATUS_PRINT_INTERVAL = 4000;

// ============================================================
// I2C settings
// ============================================================

const int I2C_SDA_PIN = 5;
const int I2C_SCL_PIN = 4;

const uint8_t MCP_ADDR = 0x20;

// ============================================================
// MCP23017 registers
// ============================================================

const uint8_t IODIRA = 0x00;
const uint8_t IODIRB = 0x01;
const uint8_t GPIOA  = 0x12;
const uint8_t GPIOB  = 0x13;

// ============================================================
// Output mapping
// ============================================================

const uint8_t TL1_RED_B    = 0;
const uint8_t TL1_YELLOW_B = 1;
const uint8_t TL1_GREEN_B  = 2;
const uint8_t TL2_RED_B    = 3;
const uint8_t TL2_YELLOW_B = 4;
const uint8_t TL2_GREEN_B  = 5;

const uint8_t TL3_YELLOW_A = 6;
const uint8_t TL3_RED_A    = 5;
const uint8_t TL3_GREEN_A  = 4;
const uint8_t TL4_YELLOW_A = 3;
const uint8_t TL4_RED_A    = 2;
const uint8_t TL4_GREEN_A  = 1;

// ============================================================
// Timing values
// ============================================================