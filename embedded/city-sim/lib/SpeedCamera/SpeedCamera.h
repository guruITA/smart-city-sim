#ifndef SPEEDCAMERA_H
#define SPEEDCAMERA_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class SpeedCamera {
public:
  SpeedCamera(int ir1Pin, int ir2Pin, int oledSdaPin, int oledSclPin, int screenWidth,
              int screenHeight, int oledAddr, int irActiveState, float sensorDistanceM,
              float speedLimitKmh, unsigned long passTimeoutUs, unsigned long measurementCooldownMs,
              unsigned long resultScreenHoldMs, unsigned long bootScreenHoldMs,
              unsigned long uiRefreshIntervalMs, const String& camCaptureUrl);

  void begin();
  void update();
  bool isTooFast();
  float getLastSpeedKmh();
  String getLastDirection();

private:
  enum MeasureState { IDLE, WAIT_FOR_SECOND_SENSOR, WAIT_FOR_CLEAR };

  enum CameraTriggerState {
    CAMERA_TRIGGER_IDLE,
    CAMERA_DISCONNECT_BACKEND_WIFI,
    CAMERA_CONNECT_TO_CAMERA_WIFI,
    CAMERA_WAIT_FOR_CAMERA_WIFI,
    CAMERA_SEND_CAPTURE_REQUEST,
    CAMERA_DISCONNECT_CAMERA_WIFI,
    CAMERA_RECONNECT_BACKEND_WIFI,
    CAMERA_WAIT_FOR_BACKEND_WIFI
  };

  int _ir1Pin;
  int _ir2Pin;
  int _oledSdaPin;
  int _oledSclPin;
  int _screenWidth;
  int _screenHeight;
  int _oledAddr;
  int _irActiveState;

  float _sensorDistanceM;
  float _speedLimitKmh;

  unsigned long _passTimeoutUs;
  unsigned long _measurementCooldownMs;
  unsigned long _resultScreenHoldMs;
  unsigned long _bootScreenHoldMs;
  unsigned long _uiRefreshIntervalMs;

  unsigned long _bootScreenStartMs;
  bool _bootScreenShowing;

  String _camCaptureUrl;

  TwoWire _displayWire;
  Adafruit_SSD1306 _display;
  bool _displayReady;

  MeasureState _measureState;
  int _firstSensor;
  unsigned long _tStartUs;

  bool _lastIr1Active;
  bool _lastIr2Active;

  float _lastSpeedKmh;
  bool _lastTooFast;
  String _lastDirection;
  unsigned long _lastEventMs;
  unsigned long _lastMeasurementDoneMs;
  unsigned long _lastUiRefresh;

  volatile bool _ir1EdgeDetected;
  volatile bool _ir2EdgeDetected;
  volatile unsigned long _ir1EdgeTimeUs;
  volatile unsigned long _ir2EdgeTimeUs;

  static SpeedCamera* _instance;

  CameraTriggerState _cameraTriggerState;
  unsigned long _cameraTriggerStateStartedMs;

  bool _pendingBackendUpdate;
  float _pendingBackendSpeedKmh;
  bool _pendingBackendTooFast;
  String _pendingBackendDirection;
  float _pendingBackendSpeedLimitKmh;

  static void IRAM_ATTR handleIr1ISR();
  static void IRAM_ATTR handleIr2ISR();
  void IRAM_ATTR handleIrEdge(int sensorNumber);

  bool sensorActive(int pin);
  void drawBootScreen();
  void drawStatusScreen(bool ir1, bool ir2);
  void drawMeasurementScreen(float speedKmh, bool tooFast, const String& direction,
                             unsigned long dtUs);

  void resetMeasurement();
  void processMeasurement(int fromSensor, int toSensor, unsigned long dtUs);

  bool cameraTriggerBusy();
  void startCameraTriggerOverWiFi();
  void updateCameraTriggerOverWiFi();
  void sendPendingBackendUpdate();
};

#endif