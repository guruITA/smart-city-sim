#ifndef PARKING_H
#define PARKING_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP23X17.h>

/**
 * @brief Class representing a parking system with ultrasonic sensors and an OLED display.
 */
class Parking {

private:
  // Measurement steps used while reading an ultrasonic sensor.
  enum SensorState { IDLE, WAITING_FOR_ECHO_START, WAITING_FOR_ECHO_END };

  // Stores the data for one parking spot.
  struct ParkingSpot {
    uint8_t triggerPin; // Trigger pin connected to this parking spot sensor.
    uint8_t echoPin; // Echo pin connected to this parking spot sensor.
    float distance; // Last measured distance in centimeters.
    bool occupied; // Shows whether the parking spot is occupied.
    SensorState state; // Current measurement step for this parking spot.
    unsigned long triggerTimeUs; // Time when the trigger pulse was sent.
    unsigned long echoStartUs; // Time when the echo signal started.
    uint8_t occupiedCount; // Counts how many times the spot looks occupied.
    uint8_t freeCount; // Counts how many times the spot looks free.
    bool backendSynced; // Shows whether this spot was already sent to the backend once.
  };

  // I2C address of the MCP23017.
  uint8_t _mcpAddress;

  // OLED I2C pins and display settings.
  uint8_t _oledSdaPin;
  uint8_t _oledSclPin;
  int _screenWidth;
  int _screenHeight;
  uint8_t _oledAddr;

  // Distance calculation and parking detection settings.
  float _soundSpeed;
  float _parkedThresholdOnCm;
  float _parkedThresholdOffCm;
  float _invalidDistanceCm;
  float _echoTravelDivider;

  // Timing settings for sensor timeout, display refresh, and sensor updates.
  unsigned long _echoTimeoutMicroseconds;
  unsigned long _uiRefreshIntervalMs;
  unsigned long _sensorMeasureIntervalMs;

  // Last update times and the current sensor index.
  unsigned long _lastUiRefreshMs;
  unsigned long _lastSensorMeasureMs;
  int8_t _currentSensorIndex;

  // OLED communication and display object.
  TwoWire _parkingWire;
  Adafruit_SSD1306 _display;

  // MCP23017 object used for the ultrasonic sensor pins.
  Adafruit_MCP23X17 _mcp;

  // Stores the data for the four parking spots
  ParkingSpot _parkingSpots[4];

  bool updateDistanceMeasurement(ParkingSpot& spot);
  void updateOccupiedState(ParkingSpot& spot);
  const char* getStateText(bool isOccupied);
  int countAvailableSpots();
  void drawStatusScreen();

public:
  Parking(uint8_t mcpAddress, uint8_t trig1Pin, uint8_t trig2Pin, uint8_t trig3Pin,
          uint8_t trig4Pin, uint8_t echo1Pin, uint8_t echo2Pin, uint8_t echo3Pin, uint8_t echo4Pin,
          uint8_t oledSdaPin, uint8_t oledSclPin, int screenWidth, int screenHeight,
          uint8_t oledAddr, float soundSpeed, float parkedThresholdOnCm, float parkedThresholdOffCm,
          float invalidDistanceCm, float echoTravelDivider, unsigned long echoTimeoutMicroseconds,
          unsigned long uiRefreshIntervalMs, unsigned long sensorMeasureIntervalMs);

  void begin();
  void update();
};

#endif