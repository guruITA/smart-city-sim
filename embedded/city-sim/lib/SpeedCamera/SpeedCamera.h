#ifndef SPEEDCAMERA_H
#define SPEEDCAMERA_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class SpeedCamera {
public:
  SpeedCamera(int ir1Pin, int ir2Pin, int oledSdaPin, int oledSclPin, int screenWidth,
              int screenHeight, int oledAddr, int irActiveState, float sensorDistanceM,
              float speedLimitKmh, unsigned long passTimeoutUs, unsigned long measurementCooldownMs,
              unsigned long resultScreenHoldMs, const String& camCaptureUrl);

  void begin();
  void update();
  bool isTooFast();
  float getLastSpeedKmh();
  String getLastDirection();

private:
  enum MeasureState { IDLE, WAIT_FOR_SECOND_SENSOR };

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

  String _camCaptureUrl;

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

  bool sensorActive(int pin);
  void drawBootScreen();
  void drawStatusScreen(bool ir1, bool ir2);
  void drawMeasurementScreen(float speedKmh, bool tooFast, const String& direction,
                             unsigned long dtUs);
  void resetMeasurement();
  void triggerCameraOverWiFi();
  void processMeasurement(int fromSensor, int toSensor, unsigned long dtUs);
};

#endif