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

const unsigned long ALL_RED_TIME = 1000;
const unsigned long GREEN_TIME   = 5000;
const unsigned long YELLOW_TIME  = 2000;

// ============================================================
// Phase constants
// ============================================================

const uint8_t PHASE_ALL_RED_1     = 0;
const uint8_t PHASE_ROAD12_GREEN  = 1;
const uint8_t PHASE_ROAD12_YELLOW = 2;
const uint8_t PHASE_ALL_RED_2     = 3;
const uint8_t PHASE_ROAD34_GREEN  = 4;
const uint8_t PHASE_ROAD34_YELLOW = 5;

// ============================================================
// State variables
// ============================================================

uint8_t currentPhase = PHASE_ALL_RED_1;
unsigned long lastPhaseChange = 0;

// ============================================================
// Sensor state variables
// ============================================================

int rawSensorLevel = HIGH;
bool rawSensorActive = false;
bool lastRawSensorActive = false;
bool stableSensorActive = false;

unsigned long lastRawSensorChangeTime = 0;
unsigned long lastSensorStatusPrint = 0;

// ============================================================
// MCP23017 functions
// ============================================================

uint8_t bitMask(uint8_t bit) {
  return (1 << bit);
}

void mcpWriteRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void mcpWriteBoth(uint8_t portAValue, uint8_t portBValue) {
  mcpWriteRegister(GPIOA, portAValue);
  mcpWriteRegister(GPIOB, portBValue);
}

void mcpInit() {
  mcpWriteRegister(IODIRA, 0x00);
  mcpWriteRegister(IODIRB, 0x00);
  mcpWriteBoth(0x00, 0x00);
}

// ============================================================
// Sensor functions
// ============================================================

void initSensor() {
  pinMode(VEHICLE_SENSOR_PIN, INPUT_PULLUP);

  rawSensorLevel = digitalRead(VEHICLE_SENSOR_PIN);
  rawSensorActive = (rawSensorLevel == SENSOR_ACTIVE_LEVEL);
  lastRawSensorActive = rawSensorActive;
  stableSensorActive = rawSensorActive;
  lastRawSensorChangeTime = millis();
}

void updateSensor(unsigned long now) {
  rawSensorLevel = digitalRead(VEHICLE_SENSOR_PIN);
  rawSensorActive = (rawSensorLevel == SENSOR_ACTIVE_LEVEL);

  if (rawSensorActive != lastRawSensorActive) {
    lastRawSensorChangeTime = now;
    lastRawSensorActive = rawSensorActive;
  }

  if ((now - lastRawSensorChangeTime) >= SENSOR_DEBOUNCE_TIME) {
    if (stableSensorActive != rawSensorActive) {
      stableSensorActive = rawSensorActive;

      Serial.print("[SENSOR] Stable state changed: ");
      Serial.println(stableSensorActive ? "vehicle detected" : "no vehicle");
    }
  }
}

bool vehicleDetected() {
  return stableSensorActive;
}

void printSensorStatus(unsigned long now) {
  if ((now - lastSensorStatusPrint) < SENSOR_STATUS_PRINT_INTERVAL) {
    return;
  }

  lastSensorStatusPrint = now;

  Serial.print("[SENSOR] GPIO ");
  Serial.print(VEHICLE_SENSOR_PIN);
  Serial.print(" raw level: ");
  Serial.print(rawSensorLevel == HIGH ? "HIGH" : "LOW");
  Serial.print(" | stable state: ");
  Serial.println(stableSensorActive ? "vehicle detected" : "no vehicle");
}

// ============================================================
// Wi-Fi functions
// ============================================================

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
  Serial.println();
  Serial.println("[WIFI] Connecting to Wi-Fi...");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long startAttempt = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttempt < WIFI_CONNECT_TIMEOUT) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[WIFI] Connection PASSED.");
    printWiFiStatus();
    return true;
  }

  Serial.println("[WIFI] Connection FAILED.");
  printWiFiStatus();
  return false;
}

// ============================================================
// Traffic-light output functions
// ============================================================

void allRed() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Green_road34Red() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_GREEN_B);
  b |= bitMask(TL2_GREEN_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Yellow_road34Red() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_YELLOW_B);
  b |= bitMask(TL2_YELLOW_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Red_road34Green() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_GREEN_A);
  a |= bitMask(TL4_GREEN_A);

  mcpWriteBoth(a, b);
}

void road12Red_road34Yellow() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_YELLOW_A);
  a |= bitMask(TL4_YELLOW_A);

  mcpWriteBoth(a, b);
}

// ============================================================
// Backend message functions
// ============================================================

const char* getBackendPhase(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
    case PHASE_ALL_RED_2:
      return "ALL_RED";

    case PHASE_ROAD12_GREEN:
      return "NS_GREEN";

    case PHASE_ROAD12_YELLOW:
      return "NS_YELLOW";

    case PHASE_ROAD34_GREEN:
      return "EW_GREEN";

    case PHASE_ROAD34_YELLOW:
      return "EW_YELLOW";

    default:
      return "UNKNOWN";
  }
}

const char* getInterpretedState(uint8_t phase) {
  if (!vehicleDetected()) {
    return "no_vehicle";
  }

  switch (phase) {
    case PHASE_ROAD12_GREEN:
    case PHASE_ROAD12_YELLOW:
      return "passing_vehicle";

    case PHASE_ALL_RED_1:
    case PHASE_ALL_RED_2:
    case PHASE_ROAD34_GREEN:
    case PHASE_ROAD34_YELLOW:
    default:
      return "waiting_vehicle";
  }
}

String createBackendJsonMessage(uint8_t phase) {
  String payload = "{";
  payload += "\"sensorId\":\"north_1\",";
  payload += "\"direction\":\"north\",";
  payload += "\"phase\":\"";
  payload += getBackendPhase(phase);
  payload += "\",";
  payload += "\"interpretedState\":\"";
  payload += getInterpretedState(phase);
  payload += "\",";
  payload += "\"timestampMs\":";
  payload += millis();
  payload += ",";
  payload += "\"valid\":true";
  payload += "}";

  return payload;
}

void sendBackendMessage(uint8_t phase) {
  updateSensor(millis());

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[BACKEND] Wi-Fi not connected. Trying reconnect...");

    if (!connectToWiFi()) {
      Serial.println("[BACKEND] POST skipped because Wi-Fi is unavailable.");
      return;
    }
  }

  String payload = createBackendJsonMessage(phase);

  Serial.println();
  Serial.println("[BACKEND] Sending JSON payload:");
  Serial.println(payload);

  HTTPClient http;
  http.setTimeout(HTTP_TIMEOUT);

  if (!http.begin(BACKEND_URL)) {
    Serial.println("[BACKEND] HTTP begin failed.");
    return;
  }

  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");

  unsigned long startRequest = millis();
  int httpResponseCode = http.POST(payload);
  unsigned long requestDuration = millis() - startRequest;

  Serial.print("[BACKEND] HTTP response code: ");
  Serial.println(httpResponseCode);
  Serial.print("[BACKEND] Request duration ms: ");
  Serial.println(requestDuration);

  if (httpResponseCode >= 200 && httpResponseCode < 300) {
    Serial.println("[BACKEND] POST successful.");
  } else if (httpResponseCode > 0) {
    Serial.println("[BACKEND] Backend returned an error response.");
    String response = http.getString();
    Serial.print("[BACKEND] Response body: ");
    Serial.println(response);
  } else {
    Serial.print("[BACKEND] HTTP request failed: ");
    Serial.println(http.errorToString(httpResponseCode));
  }

  http.end();
}

// ============================================================
// State machine functions
// ============================================================

void applyPhase(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
      allRed();
      Serial.println("Phase: ALL_RED");
      break;

    case PHASE_ROAD12_GREEN:
      road12Green_road34Red();
      Serial.println("Phase: NS_GREEN");
      break;

    case PHASE_ROAD12_YELLOW:
      road12Yellow_road34Red();
      Serial.println("Phase: NS_YELLOW");
      break;

    case PHASE_ALL_RED_2:
      allRed();
      Serial.println("Phase: ALL_RED");
      break;

    case PHASE_ROAD34_GREEN:
      road12Red_road34Green();
      Serial.println("Phase: EW_GREEN");
      break;

    case PHASE_ROAD34_YELLOW:
      road12Red_road34Yellow();
      Serial.println("Phase: EW_YELLOW");
      break;

    default:
      allRed();
      Serial.println("Phase: UNKNOWN -> fallback ALL_RED");
      break;
  }

  sendBackendMessage(phase);
}

unsigned long getPhaseDuration(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
    case PHASE_ALL_RED_2:
      return ALL_RED_TIME;

    case PHASE_ROAD12_GREEN:
    case PHASE_ROAD34_GREEN:
      return GREEN_TIME;

    case PHASE_ROAD12_YELLOW:
    case PHASE_ROAD34_YELLOW:
      return YELLOW_TIME;

    default:
      return ALL_RED_TIME;
  }
}

uint8_t getNextPhase(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
      return PHASE_ROAD12_GREEN;

    case PHASE_ROAD12_GREEN:
      return PHASE_ROAD12_YELLOW;

    case PHASE_ROAD12_YELLOW:
      return PHASE_ALL_RED_2;

    case PHASE_ALL_RED_2:
      return PHASE_ROAD34_GREEN;

    case PHASE_ROAD34_GREEN:
      return PHASE_ROAD34_YELLOW;

    case PHASE_ROAD34_YELLOW:
      return PHASE_ALL_RED_1;

    default:
      return PHASE_ALL_RED_1;
  }
}

// ============================================================
// Setup
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("============================================================");
  Serial.println("SMART TRAFFIC LIGHT START");
  Serial.println("============================================================");

  initSensor();

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  mcpInit();

  connectToWiFi();

  lastPhaseChange = millis();
  applyPhase(currentPhase);

  Serial.println("Breadboard traffic light controller started");
  Serial.println("Non-blocking timing with millis()");
  Serial.println("Vehicle sensor is read from GPIO 6.");
  Serial.println("Backend JSON is sent after every phase change.");
  Serial.println("============================================================");
}

// ============================================================
// Main loop
// ============================================================

void loop() {
  unsigned long now = millis();

  updateSensor(now);
  printSensorStatus(now);

  if (now - lastPhaseChange >= getPhaseDuration(currentPhase)) {
    currentPhase = getNextPhase(currentPhase);
    lastPhaseChange = now;
    applyPhase(currentPhase);
  }
}