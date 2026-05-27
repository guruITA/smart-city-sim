#include "SpeedCamera.h"

#include <Wire.h>

#include "Config.h"
#include "NetworkController.h"
#include "SpeedCameraNetwork.h"

SpeedCamera* SpeedCamera::_instance = NULL;

SpeedCamera::SpeedCamera(int ir1Pin, int ir2Pin, int oledSdaPin, int oledSclPin, int screenWidth,
                         int screenHeight, int oledAddr, int irActiveState, float sensorDistanceM,
                         float speedLimitKmh, unsigned long passTimeoutUs,
                         unsigned long measurementCooldownMs, unsigned long resultScreenHoldMs,
                         unsigned long bootScreenHoldMs, unsigned long uiRefreshIntervalMs)
    : _ir1Pin(ir1Pin), _ir2Pin(ir2Pin), _oledSdaPin(oledSdaPin), _oledSclPin(oledSclPin),
      _oledAddr(oledAddr), _irActiveState(irActiveState), _sensorDistanceM(sensorDistanceM),
      _speedLimitKmh(speedLimitKmh), _passTimeoutUs(passTimeoutUs),
      _measurementCooldownMs(measurementCooldownMs), _resultScreenHoldMs(resultScreenHoldMs),
      _bootScreenHoldMs(bootScreenHoldMs), _uiRefreshIntervalMs(uiRefreshIntervalMs),
      _displayWire(1), _display(screenWidth, screenHeight, &_displayWire, -1), _displayReady(false),
      _measureState(IDLE), _firstSensor(0), _tStartUs(0), _lastSpeedKmh(0.0f), _lastDirection("-"),
      _lastEventMs(0), _lastMeasurementDoneMs(0), _lastUiRefresh(0),
      _okMeasurementWaitingForSpeed(false), _okMeasurementSpeedKmh(0.0f), _okMeasurementStartMs(0),
      _tooFastMeasurementWaitingForSpeed(false), _tooFastMeasurementSpeedKmh(0.0f),
      _tooFastMeasurementStartMs(0), _bootScreenStartMs(0), _bootScreenShowing(false),
      _ir1EdgeDetected(false), _ir2EdgeDetected(false), _ir1EdgeTimeUs(0), _ir2EdgeTimeUs(0),
      _cameraCaptureState(CAMERA_CAPTURE_IDLE), _pendingBackendUpdate(false),
      _pendingBackendSpeedKmh(0.0f), _pendingBackendTooFast(false), _pendingBackendDirection("-"),
      _pendingBackendSpeedLimitKmh(0.0f) {}

void SpeedCamera::begin() {

  Serial.begin(115200);

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
  updateCameraCapture();

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

  if (_okMeasurementWaitingForSpeed && millis() - _okMeasurementStartMs >= 1500UL) {
    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);

    _display.setTextSize(2);
    _display.setCursor(20, 22);
    _display.print(_okMeasurementSpeedKmh, 1);

    _display.setTextSize(1);
    _display.setCursor(88, 30);
    _display.println("km/u");

    _display.display();

    _okMeasurementWaitingForSpeed = false;

    _lastEventMs = millis();
  }

  if (_tooFastMeasurementWaitingForSpeed && millis() - _tooFastMeasurementStartMs >= 1500UL) {
    _display.clearDisplay();
    _display.setTextColor(SSD1306_WHITE);

    _display.setTextSize(1);
    _display.setCursor(0, 0);
    _display.println("TOO FAST!");
    _display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

    _display.setTextSize(2);
    _display.setCursor(0, 22);
    _display.print(_tooFastMeasurementSpeedKmh, 1);

    _display.setTextSize(1);
    _display.setCursor(88, 30);
    _display.println("km/u");

    _display.setCursor(0, 52);
    _display.print("Allowed: ");
    _display.print(_speedLimitKmh, 0);
    _display.println(" km/u");

    _display.display();

    _tooFastMeasurementWaitingForSpeed = false;

    _lastEventMs = millis();
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
}

bool SpeedCamera::sensorActive(int pin) {
  return digitalRead(pin) == _irActiveState;
}

void SpeedCamera::drawBootScreen() {
  if (!_displayReady) {
    return;
  }

  _display.clearDisplay();
  _display.setTextColor(SSD1306_WHITE);

  _display.setTextSize(2);
  _display.setCursor(20, 22);
  _display.println("Loading");

  _display.display();
}

void SpeedCamera::drawStatusScreen(bool ir1, bool ir2) {
  if (!_displayReady) {
    return;
  }

  _display.clearDisplay();
  _display.setTextColor(SSD1306_WHITE);

  _display.setTextSize(1);
  _display.setCursor(0, 0);
  _display.println("Allowed speed");

  _display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  _display.setTextSize(3);
  _display.setCursor(18, 24);
  _display.print(_speedLimitKmh, 0);

  _display.setTextSize(1);
  _display.setCursor(88, 38);
  _display.println("km/u");

  _display.display();
}

void SpeedCamera::drawMeasurementScreen(float speedKmh, bool tooFast, const String& direction,
                                        unsigned long dtUs) {
  if (!_displayReady) {
    return;
  }

  _display.clearDisplay();
  _display.setTextColor(SSD1306_WHITE);

  if (!tooFast) {
    _okMeasurementWaitingForSpeed = true;
    _okMeasurementSpeedKmh = speedKmh;
    _okMeasurementStartMs = millis();

    _tooFastMeasurementWaitingForSpeed = false;

    _display.drawCircle(64, 30, 18, SSD1306_WHITE);
    _display.fillCircle(57, 25, 2, SSD1306_WHITE);
    _display.fillCircle(71, 25, 2, SSD1306_WHITE);
    _display.drawLine(55, 37, 59, 41, SSD1306_WHITE);
    _display.drawLine(59, 41, 64, 43, SSD1306_WHITE);
    _display.drawLine(64, 43, 69, 41, SSD1306_WHITE);
    _display.drawLine(69, 41, 73, 37, SSD1306_WHITE);

  } else {
    _tooFastMeasurementWaitingForSpeed = true;
    _tooFastMeasurementSpeedKmh = speedKmh;
    _tooFastMeasurementStartMs = millis();

    _okMeasurementWaitingForSpeed = false;

    _display.drawCircle(64, 30, 18, SSD1306_WHITE);
    _display.fillCircle(57, 25, 2, SSD1306_WHITE);
    _display.fillCircle(71, 25, 2, SSD1306_WHITE);
    _display.drawLine(55, 42, 59, 38, SSD1306_WHITE);
    _display.drawLine(59, 38, 64, 36, SSD1306_WHITE);
    _display.drawLine(64, 36, 69, 38, SSD1306_WHITE);
    _display.drawLine(69, 38, 73, 42, SSD1306_WHITE);
  }

  _display.display();
}

void SpeedCamera::resetMeasurement() {
  _measureState = IDLE;
  _firstSensor = 0;
  _tStartUs = 0;
}

bool SpeedCamera::cameraCaptureBusy() {
  return _cameraCaptureState != CAMERA_CAPTURE_IDLE;
}

void SpeedCamera::startCameraCapture() {
  if (cameraCaptureBusy()) {
    Serial.println("Camera capture skipped: already running.");
    return;
  }

  Serial.println("Starting camera capture through backend registered camera URL.");
  _cameraCaptureState = CAMERA_SEND_CAPTURE_REQUEST;
}

void SpeedCamera::sendPendingBackendUpdate() {
  if (!_pendingBackendUpdate) {
    return;
  }

  SpeedCameraNetwork::sendMeasurement(_pendingBackendSpeedKmh, _pendingBackendDirection,
                                      _pendingBackendTooFast, _pendingBackendSpeedLimitKmh);

  _pendingBackendUpdate = false;
}

void SpeedCamera::updateCameraCapture() {
  switch (_cameraCaptureState) {

  case CAMERA_CAPTURE_IDLE:
    return;

  case CAMERA_SEND_CAPTURE_REQUEST: {
    String captureUrl;

    if (SpeedCameraNetwork::getCameraCaptureUrl(captureUrl)) {
      int httpCode = -1;
      String response = NetworkController::fetch(captureUrl, httpCode);

      Serial.print("Camera capture HTTP code: ");
      Serial.println(httpCode);

      if (httpCode <= 0) {
        Serial.println("Camera capture failed.");
      } else {
        Serial.print("Camera response: ");
        Serial.println(response);
      }
    } else {
      Serial.println("Could not get camera capture URL from backend.");
    }

    sendPendingBackendUpdate();

    _cameraCaptureState = CAMERA_CAPTURE_IDLE;
    break;
  }
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

    startCameraCapture();
  } else {
    SpeedCameraNetwork::sendMeasurement(realSpeedKmh, _lastDirection, tooFast, _speedLimitKmh);
  }

  _measureState = WAIT_FOR_CLEAR;
  _firstSensor = 0;
  _tStartUs = 0;
}