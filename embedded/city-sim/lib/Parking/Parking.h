#ifndef PARKING_H
#define PARKING_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class Parking {

private:
  enum SensorState { IDLE, WAITING_FOR_ECHO_START, WAITING_FOR_ECHO_END };

  struct ParkingSpot {
    int8_t echoPin;
    float distance;
    bool occupied;
    SensorState state;
    unsigned long triggerTimeUs;
    unsigned long echoStartUs;
  };

  uint8_t _trigPin;

  uint8_t _oledSdaPin;
  uint8_t _oledSclPin;
  int _screenWidth;
  int _screenHeight;
  uint8_t _oledAddr;

  float _soundSpeed;
  float _parkedThresholdOnCm;
  float _parkedThresholdOffCm;
  float _invalidDistanceCm;
  float _echoTravelDivider;

  unsigned long _echoTimeoutMicroseconds;
  unsigned long _uiRefreshIntervalMs;
  unsigned long _sensorMeasureIntervalMs;

  unsigned long _lastUiRefreshMs;
  unsigned long _lastSensorMeasureMs;
  int8_t _currentSensorIndex;

  TwoWire _parkingWire;
  Adafruit_SSD1306 _display;

  ParkingSpot _parkingSpots[4];

  volatile bool _echoRiseDetected;
  volatile bool _echoMeasurementDone;
  volatile unsigned long _echoStartUsInterrupt;
  volatile unsigned long _echoEndUsInterrupt;
  volatile int _activeEchoPin;

  static Parking* _instance;

  static void IRAM_ATTR handleEchoChangeISR();
  void handleEchoChange();

  bool updateDistanceMeasurement(ParkingSpot& spot);
  void updateOccupiedState(float distanceCm, bool& isOccupied);
  const char* getStateText(bool isOccupied);
  int countAvailableSpots();
  void drawStatusScreen();

public:
  Parking(uint8_t trigPin, uint8_t echo1Pin, uint8_t echo2Pin, uint8_t echo3Pin, uint8_t echo4Pin,
          uint8_t oledSdaPin, uint8_t oledSclPin, int screenWidth, int screenHeight,
          uint8_t oledAddr, float soundSpeed, float parkedThresholdOnCm, float parkedThresholdOffCm,
          float invalidDistanceCm, float echoTravelDivider, unsigned long echoTimeoutMicroseconds,
          unsigned long uiRefreshIntervalMs, unsigned long sensorMeasureIntervalMs);

  void begin();
  void update();
};

#endif