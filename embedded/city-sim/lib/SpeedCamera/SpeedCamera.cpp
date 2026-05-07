#include "SpeedCamera.h"

#include <Wire.h>
#include <WiFi.h>

#include "NetworkController.h"

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
      _bootScreenShowing(false) {}

void SpeedCamera::begin() {
  pinMode(_ir1Pin, INPUT);
  pinMode(_ir2Pin, INPUT);

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

  Serial.println("Starting ESP32-S3.");
  Serial.println("Waiting on IR-measurements...");
}

void SpeedCamera::update() {
  bool ir1 = sensorActive(_ir1Pin);
  bool ir2 = sensorActive(_ir2Pin);

  if (_bootScreenShowing && (millis() - _bootScreenStartMs >= _bootScreenHoldMs)) {
    drawStatusScreen(ir1, ir2);
    _bootScreenShowing = false;
  }

  bool edge1 = ir1 && !_lastIr1Active;
  bool edge2 = ir2 && !_lastIr2Active;

  bool inCooldown = (millis() - _lastMeasurementDoneMs) < _measurementCooldownMs;

  switch (_measureState) {
  case IDLE:
    if (!inCooldown) {
      if (edge1 && !ir2) {
        _firstSensor = 1;
        _tStartUs = micros();
        _measureState = WAIT_FOR_SECOND_SENSOR;
        Serial.println("Start on IR1");
      } else if (edge2 && !ir1) {
        _firstSensor = 2;
        _tStartUs = micros();
        _measureState = WAIT_FOR_SECOND_SENSOR;
        Serial.println("Start on IR2");
      }
    }
    break;

  case WAIT_FOR_SECOND_SENSOR: {
    unsigned long elapsedUs = micros() - _tStartUs;

    if (elapsedUs > _passTimeoutUs) {
      Serial.println("Timeout, measurement reset.");
      resetMeasurement();
    } else if (_firstSensor == 1 && edge2) {
      processMeasurement(1, 2, elapsedUs);
    } else if (_firstSensor == 2 && edge1) {
      processMeasurement(2, 1, elapsedUs);
    }
    break;
  }
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
  _display.println("Speed Camera S3 start...");
  _display.println("IR1 = GPIO6");
  _display.println("IR2 = GPIO12");
  _display.println("OLED SDA/SCL = 17y/46");
  _display.println("Camera via WiFi");
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
  _display.println("------------");

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

void SpeedCamera::triggerCameraOverWiFi() {
  if (!NetworkController::connected()) {
    Serial.println("Kan camera niet triggeren, geen wifi.");
    return;
  }

  int httpCode = -1;
  String response = NetworkController::fetch(_camCaptureUrl, httpCode);

  Serial.print("Camera trigger HTTP code: ");
  Serial.println(httpCode);

  if (httpCode <= 0) {
    Serial.println("Camera trigger mislukt.");
    return;
  }

  Serial.print("Camera response: ");
  Serial.println(response);
}

void SpeedCamera::processMeasurement(int fromSensor, int toSensor, unsigned long dtUs) {
  if (dtUs == 0) {
    resetMeasurement();
    return;
  }

  float dtSeconds = dtUs / 1000000.0f;
  float speedMs = _sensorDistanceM / dtSeconds;
  float speedKmh = speedMs * 3.6f;
  bool tooFast = speedKmh > _speedLimitKmh;

  _lastSpeedKmh = speedKmh;
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
  Serial.print("Distance (m): ");
  Serial.println(_sensorDistanceM, 3);
  Serial.print("Speed (km/u): ");
  Serial.println(speedKmh, 2);
  Serial.print("Too Fast: ");
  Serial.println(tooFast ? "YES" : "NO");

  drawMeasurementScreen(speedKmh, tooFast, _lastDirection, dtUs);

  if (tooFast) {
    triggerCameraOverWiFi();
  }

  resetMeasurement();
}