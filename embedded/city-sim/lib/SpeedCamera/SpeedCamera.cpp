#include "SpeedCamera.h"

#include <Wire.h>
#include <WiFi.h>

#include "Config.h"
#include "NetworkController.h"
#include "SpeedCameraNetwork.h"

SpeedCamera* SpeedCamera::_instance = NULL;

SpeedCamera::SpeedCamera(int ir1Pin, int ir2Pin, int oledSdaPin, int oledSclPin, int screenWidth,
                         int screenHeight, int oledAddr, int irActiveState, float sensorDistanceM,
                         float speedLimitKmh, unsigned long passTimeoutUs,
                         unsigned long measurementCooldownMs, unsigned long resultScreenHoldMs,
                         unsigned long bootScreenHoldMs, unsigned long uiRefreshIntervalMs,
                         const String& camCaptureUrl)
    : _ir1Pin(ir1Pin), _ir2Pin(ir2Pin), _oledSdaPin(oledSdaPin), _oledSclPin(oledSclPin),
      _screenWidth(screenWidth), _screenHeight(screenHeight), _oledAddr(oledAddr),
      _irActiveState(irActiveState), _sensorDistanceM(sensorDistanceM),
      _speedLimitKmh(speedLimitKmh), _passTimeoutUs(passTimeoutUs),
      _measurementCooldownMs(measurementCooldownMs), _resultScreenHoldMs(resultScreenHoldMs),
      _bootScreenHoldMs(bootScreenHoldMs), _uiRefreshIntervalMs(uiRefreshIntervalMs),
      _camCaptureUrl(camCaptureUrl), _displayWire(1),
      _display(screenWidth, screenHeight, &_displayWire, -1), _displayReady(false),
      _measureState(IDLE), _firstSensor(0), _tStartUs(0), _lastIr1Active(false),
      _lastIr2Active(false), _lastSpeedKmh(0.0f), _lastTooFast(false), _lastDirection("-"),
      _lastEventMs(0), _lastMeasurementDoneMs(0), _lastUiRefresh(0), _bootScreenStartMs(0),
      _bootScreenShowing(false), _ir1EdgeDetected(false), _ir2EdgeDetected(false),
      _ir1EdgeTimeUs(0), _ir2EdgeTimeUs(0), _cameraTriggerState(CAMERA_TRIGGER_IDLE),
      _cameraTriggerStateStartedMs(0), _pendingBackendUpdate(false), _pendingBackendSpeedKmh(0.0f),
      _pendingBackendTooFast(false), _pendingBackendDirection("-"),
      _pendingBackendSpeedLimitKmh(0.0f) {}

void SpeedCamera::begin() {
  pinMode(_ir1Pin, INPUT);
  pinMode(_ir2Pin, INPUT);

  _instance = this;

  attachInterrupt(digitalPinToInterrupt(_ir1Pin), handleIr1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_ir2Pin), handleIr2ISR, CHANGE);

  _displayWire.begin(_oledSdaPin, _oledSclPin);

  if (_display.begin(SSD1306_SWITCHCAPVCC, _oledAddr)) {
    _displayReady = true;
    drawBootScreen();
    _bootScreenStartMs = millis();
    _bootScreenShowing = true;
  } else {
    Serial.println("OLED not found.");
  }

  _lastIr1Active = sensorActive(_ir1Pin);
  _lastIr2Active = sensorActive(_ir2Pin);

  Serial.println("Starting ESP32-S3 SpeedCamera.");
  Serial.println("Waiting on IR measurements...");
}

void IRAM_ATTR SpeedCamera::handleIr1ISR() {
  if (_instance != NULL) {
    _instance->handleIrEdge(1);
  }
}

void IRAM_ATTR SpeedCamera::handleIr2ISR() {
  if (_instance != NULL) {
    _instance->handleIrEdge(2);
  }
}

void IRAM_ATTR SpeedCamera::handleIrEdge(int sensorNumber) {
  if (sensorNumber == 1) {
    if (digitalRead(_ir1Pin) == _irActiveState) {
      _ir1EdgeTimeUs = micros();
      _ir1EdgeDetected = true;
    }
  } else if (sensorNumber == 2) {
    if (digitalRead(_ir2Pin) == _irActiveState) {
      _ir2EdgeTimeUs = micros();
      _ir2EdgeDetected = true;
    }
  }
}

void SpeedCamera::update() {
  updateCameraTriggerOverWiFi();

  bool ir1 = sensorActive(_ir1Pin);
  bool ir2 = sensorActive(_ir2Pin);

  bool edge1 = false;
  bool edge2 = false;
  unsigned long edge1TimeUs = 0;
  unsigned long edge2TimeUs = 0;

  edge1 = _ir1EdgeDetected;
  edge2 = _ir2EdgeDetected;
  edge1TimeUs = _ir1EdgeTimeUs;
  edge2TimeUs = _ir2EdgeTimeUs;
  _ir1EdgeDetected = false;
  _ir2EdgeDetected = false;

  if (_bootScreenShowing && (millis() - _bootScreenStartMs >= _bootScreenHoldMs)) {
    drawStatusScreen(ir1, ir2);
    _bootScreenShowing = false;
  }

  bool inCooldown = (millis() - _lastMeasurementDoneMs) < _measurementCooldownMs;

  switch (_measureState) {
  case IDLE:
    if (!inCooldown) {

      if (edge1 && edge2) {
        if (edge1TimeUs < edge2TimeUs) {
          processMeasurement(1, 2, edge2TimeUs - edge1TimeUs);
        } else if (edge2TimeUs < edge1TimeUs) {
          processMeasurement(2, 1, edge1TimeUs - edge2TimeUs);
        } else {
          Serial.println("Both sensors triggered at the same time, waiting for clear.");
          _firstSensor = 0;
          _tStartUs = 0;
          _measureState = WAIT_FOR_CLEAR;
        }
      } else if (edge1 && !ir2) {
        _firstSensor = 1;
        _tStartUs = edge1TimeUs;
        _measureState = WAIT_FOR_SECOND_SENSOR;
        Serial.println("Start on IR1");
      } else if (edge2 && !ir1) {
        _firstSensor = 2;
        _tStartUs = edge2TimeUs;
        _measureState = WAIT_FOR_SECOND_SENSOR;
        Serial.println("Start on IR2");
      }
    }
    break;

  case WAIT_FOR_SECOND_SENSOR: {
    unsigned long elapsedUs = micros() - _tStartUs;

    if (elapsedUs > _passTimeoutUs) {
      Serial.println("Timeout, waiting until both sensors are clear.");
      _firstSensor = 0;
      _tStartUs = 0;
      _measureState = WAIT_FOR_CLEAR;
    } else if (_firstSensor == 1 && edge2) {
      processMeasurement(1, 2, edge2TimeUs - _tStartUs);
    } else if (_firstSensor == 2 && edge1) {
      processMeasurement(2, 1, edge1TimeUs - _tStartUs);
    }
    break;
  }

  case WAIT_FOR_CLEAR:
    if (!ir1 && !ir2) {
      Serial.println("Both sensors clear, ready for new measurement.");
      resetMeasurement();
    }
    break;
  }

  if (!_bootScreenShowing && millis() - _lastUiRefresh > _uiRefreshIntervalMs) {
    if ((millis() - _lastEventMs) >= _resultScreenHoldMs) {
      drawStatusScreen(ir1, ir2);
    }
    _lastUiRefresh = millis();
  }

  _lastIr1Active = ir1;
  _lastIr2Active = ir2;
}

bool SpeedCamera::isTooFast() {
  return _lastTooFast;
}

float SpeedCamera::getLastSpeedKmh() {
  return _lastSpeedKmh;
}

String SpeedCamera::getLastDirection() {
  return _lastDirection;
}

bool SpeedCamera::sensorActive(int pin) {
  return digitalRead(pin) == _irActiveState;
}

void SpeedCamera::drawBootScreen() {
  if (!_displayReady) {
    return;
  }

  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);

  _display.println("Speed Camera S3");
  _display.println("Starting...");
  _display.println("IR1 = GPIO6");
  _display.println("IR2 = GPIO12");
  _display.println("OLED SDA/SCL = 17/46");
  _display.println("Backend + Camera");

  _display.display();
}

void SpeedCamera::drawStatusScreen(bool ir1, bool ir2) {
  if (!_displayReady) {
    return;
  }

  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);

  _display.println("Speed Camera S3");
  _display.println("----------------");

  _display.print("IR1: ");
  _display.println(ir1 ? "ACTIVE" : "REST");

  _display.print("IR2: ");
  _display.println(ir2 ? "ACTIVE" : "REST");

  _display.print("Last: ");
  _display.print(_lastSpeedKmh, 1);
  _display.println(" km/u");

  _display.print("Limit: ");
  _display.print(_speedLimitKmh, 1);
  _display.println(" km/u");

  _display.print("Direction: ");
  _display.println(_lastDirection);

  if (NetworkController::connected()) {
    _display.println("WiFi: connected");
  } else {
    _display.println("WiFi: offline");
  }

  _display.display();
}

void SpeedCamera::drawMeasurementScreen(float speedKmh, bool tooFast, const String& direction,
                                        unsigned long dtUs) {
  if (!_displayReady) {
    return;
  }

  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);

  _display.println("MEASUREMENT READY");
  _display.println("-----------------");

  _display.print("Direction: ");
  _display.println(direction);

  _display.print("Time: ");
  _display.print(dtUs / 1000.0f, 1);
  _display.println(" ms");

  _display.print("Speed: ");
  _display.print(speedKmh, 1);
  _display.println(" km/u");

  _display.print("Limit: ");
  _display.print(_speedLimitKmh, 1);
  _display.println(" km/u");

  if (tooFast) {
    _display.println("Result: TOO FAST!");
  } else {
    _display.println("Result: OK");
  }

  _display.display();
}

void SpeedCamera::resetMeasurement() {
  _measureState = IDLE;
  _firstSensor = 0;
  _tStartUs = 0;
}

bool SpeedCamera::cameraTriggerBusy() {
  return _cameraTriggerState != CAMERA_TRIGGER_IDLE;
}

void SpeedCamera::startCameraTriggerOverWiFi() {
  if (cameraTriggerBusy()) {
    Serial.println("Camera trigger skipped: already running.");
    return;
  }

  Serial.println("Starting camera trigger first. Backend update will be sent after reconnect.");
  _cameraTriggerState = CAMERA_DISCONNECT_BACKEND_WIFI;
  _cameraTriggerStateStartedMs = millis();
}

void SpeedCamera::sendPendingBackendUpdate() {
  if (!_pendingBackendUpdate) {
    return;
  }

  SpeedCameraNetwork::sendMeasurement(_pendingBackendSpeedKmh, _pendingBackendDirection,
                                      _pendingBackendTooFast, _pendingBackendSpeedLimitKmh);

  _pendingBackendUpdate = false;
}

void SpeedCamera::updateCameraTriggerOverWiFi() {
  switch (_cameraTriggerState) {

  case CAMERA_TRIGGER_IDLE:
    return;

  case CAMERA_DISCONNECT_BACKEND_WIFI:
    Serial.println("Disconnecting backend WiFi...");
    WiFi.disconnect(false, false);

    _cameraTriggerState = CAMERA_CONNECT_TO_CAMERA_WIFI;
    _cameraTriggerStateStartedMs = millis();
    break;

  case CAMERA_CONNECT_TO_CAMERA_WIFI:
    Serial.println("Connecting to ESP32-CAM WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(Config::SpeedCamera::CAMERA_WIFI_SSID, Config::SpeedCamera::CAMERA_WIFI_PASSWORD);

    _cameraTriggerState = CAMERA_WAIT_FOR_CAMERA_WIFI;
    _cameraTriggerStateStartedMs = millis();
    break;

  case CAMERA_WAIT_FOR_CAMERA_WIFI:
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to ESP32-CAM WiFi.");
      _cameraTriggerState = CAMERA_SEND_CAPTURE_REQUEST;
      _cameraTriggerStateStartedMs = millis();
    } else if (millis() - _cameraTriggerStateStartedMs >=
               Config::SpeedCamera::CAMERA_WIFI_CONNECT_TIMEOUT_MS) {
      Serial.println("Could not connect to ESP32-CAM WiFi.");
      _cameraTriggerState = CAMERA_RECONNECT_BACKEND_WIFI;
      _cameraTriggerStateStartedMs = millis();
    }
    break;

  case CAMERA_SEND_CAPTURE_REQUEST: {
    int httpCode = -1;
    String response = NetworkController::fetch(_camCaptureUrl, httpCode);

    Serial.print("Camera trigger HTTP code: ");
    Serial.println(httpCode);

    if (httpCode <= 0) {
      Serial.println("Camera trigger failed.");
    } else {
      Serial.print("Camera response: ");
      Serial.println(response);
    }

    _cameraTriggerState = CAMERA_DISCONNECT_CAMERA_WIFI;
    _cameraTriggerStateStartedMs = millis();
    break;
  }

  case CAMERA_DISCONNECT_CAMERA_WIFI:
    Serial.println("Disconnecting ESP32-CAM WiFi...");
    WiFi.disconnect(false, false);

    _cameraTriggerState = CAMERA_RECONNECT_BACKEND_WIFI;
    _cameraTriggerStateStartedMs = millis();
    break;

  case CAMERA_RECONNECT_BACKEND_WIFI:
    Serial.println("Reconnecting to backend WiFi...");
    WiFi.mode(WIFI_STA);
    WiFi.begin(Config::Network::WIFI_SSID, Config::Network::WIFI_PASSWORD);

    NetworkController::setApiBaseUrl(Config::Network::API_BASE_URL);

    _cameraTriggerState = CAMERA_WAIT_FOR_BACKEND_WIFI;
    _cameraTriggerStateStartedMs = millis();
    break;

  case CAMERA_WAIT_FOR_BACKEND_WIFI:
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected to backend WiFi.");

      sendPendingBackendUpdate();

      _cameraTriggerState = CAMERA_TRIGGER_IDLE;
    } else if (millis() - _cameraTriggerStateStartedMs >=
               Config::SpeedCamera::CAMERA_WIFI_CONNECT_TIMEOUT_MS) {
      Serial.println("Failed to reconnect to backend WiFi.");

      _cameraTriggerState = CAMERA_TRIGGER_IDLE;
    }
    break;
  }
}

void SpeedCamera::processMeasurement(int fromSensor, int toSensor, unsigned long dtUs) {
  if (dtUs == 0) {
    _measureState = WAIT_FOR_CLEAR;
    _firstSensor = 0;
    _tStartUs = 0;
    return;
  }

  float dtSeconds = dtUs / 1000000.0f;

  float modelSpeedMs = _sensorDistanceM / dtSeconds;
  float modelSpeedKmh = modelSpeedMs * 3.6f;

  float realSpeedKmh = modelSpeedKmh * Config::SpeedCamera::SCALE_FACTOR;

  bool tooFast = realSpeedKmh > _speedLimitKmh;

  _lastSpeedKmh = realSpeedKmh;
  _lastTooFast = tooFast;
  _lastDirection = (fromSensor == 1 && toSensor == 2) ? "1->2" : "2->1";
  _lastEventMs = millis();
  _lastMeasurementDoneMs = millis();

  Serial.println();
  Serial.println("=== MEASUREMENT ===");
  Serial.print("Direction: ");
  Serial.println(_lastDirection);
  Serial.print("Time (us): ");
  Serial.println(dtUs);
  Serial.print("Sensor distance model (m): ");
  Serial.println(_sensorDistanceM, 3);
  Serial.print("Model speed (km/u): ");
  Serial.println(modelSpeedKmh, 2);
  Serial.print("Real speed scale 1:64 (km/u): ");
  Serial.println(realSpeedKmh, 2);
  Serial.print("Speed limit real (km/u): ");
  Serial.println(_speedLimitKmh, 2);
  Serial.print("Too Fast: ");
  Serial.println(tooFast ? "YES" : "NO");

  drawMeasurementScreen(realSpeedKmh, tooFast, _lastDirection, dtUs);

  if (tooFast) {
    _pendingBackendUpdate = true;
    _pendingBackendSpeedKmh = realSpeedKmh;
    _pendingBackendTooFast = tooFast;
    _pendingBackendDirection = _lastDirection;
    _pendingBackendSpeedLimitKmh = _speedLimitKmh;

    startCameraTriggerOverWiFi();
  } else {
    SpeedCameraNetwork::sendMeasurement(realSpeedKmh, _lastDirection, tooFast, _speedLimitKmh);
  }

  _measureState = WAIT_FOR_CLEAR;
  _firstSensor = 0;
  _tStartUs = 0;
}