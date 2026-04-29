#include "Parking.h"

// https://www.instructables.com/Non-blocking-Ultrasonic-Sensor-for-Arduino/
Parking* Parking::_instance = NULL;

Parking::Parking(uint8_t trigPin, uint8_t echo1Pin, uint8_t echo2Pin, uint8_t echo3Pin, uint8_t echo4Pin,
                 uint8_t oledSdaPin, uint8_t oledSclPin, int screenWidth, int screenHeight,
                 uint8_t oledAddr, float soundSpeed, float parkedThresholdOnCm, float parkedThresholdOffCm,
                 float invalidDistanceCm, float echoTravelDivider, unsigned long echoTimeoutMicroseconds,
                 unsigned long uiRefreshIntervalMs, unsigned long sensorMeasureIntervalMs)
    : _trigPin(trigPin),
      _oledSdaPin(oledSdaPin),
      _oledSclPin(oledSclPin),
      _screenWidth(screenWidth),
      _screenHeight(screenHeight),
      _oledAddr(oledAddr),
      _soundSpeed(soundSpeed),
      _parkedThresholdOnCm(parkedThresholdOnCm),
      _parkedThresholdOffCm(parkedThresholdOffCm),
      _invalidDistanceCm(invalidDistanceCm),
      _echoTravelDivider(echoTravelDivider),
      _echoTimeoutMicroseconds(echoTimeoutMicroseconds),
      _uiRefreshIntervalMs(uiRefreshIntervalMs),
      _sensorMeasureIntervalMs(sensorMeasureIntervalMs),
      _lastUiRefreshMs(0),
      _lastSensorMeasureMs(0),
      _currentSensorIndex(0),
      _parkingWire(0),
      _display(screenWidth, screenHeight, &_parkingWire, -1),
      _echoRiseDetected(false),
      _echoMeasurementDone(false),
      _echoStartUsInterrupt(0),
      _echoEndUsInterrupt(0),
      _activeEchoPin(-1) {

  _parkingSpots[0] = {int8_t(echo1Pin), _invalidDistanceCm, false, IDLE, 0, 0};
  _parkingSpots[1] = {int8_t(echo2Pin), _invalidDistanceCm, false, IDLE, 0, 0};
  _parkingSpots[2] = {int8_t(echo3Pin), _invalidDistanceCm, false, IDLE, 0, 0};
  _parkingSpots[3] = {int8_t(echo4Pin), _invalidDistanceCm, false, IDLE, 0, 0};
}

void Parking::begin() {
  const int8_t TOTAL_SPOTS = sizeof(_parkingSpots) / sizeof(_parkingSpots[0]);

  Serial.begin(115200);

  _instance = this;

  pinMode(_trigPin, OUTPUT);
  digitalWrite(_trigPin, LOW);

  for (int i = 0; i < TOTAL_SPOTS; i++) {
    pinMode(_parkingSpots[i].echoPin, INPUT);
  }

  _parkingWire.begin(_oledSdaPin, _oledSclPin);
  _display.begin(SSD1306_SWITCHCAPVCC, _oledAddr);
}

void Parking::update() {
  const int8_t TOTAL_SPOTS = sizeof(_parkingSpots) / sizeof(_parkingSpots[0]);
  unsigned long currentMillis = millis();

  if (currentMillis - _lastSensorMeasureMs >= _sensorMeasureIntervalMs) {
    bool measurementFinished = updateDistanceMeasurement(_parkingSpots[_currentSensorIndex]);

    if (measurementFinished) {
      updateOccupiedState(_parkingSpots[_currentSensorIndex].distance,
                          _parkingSpots[_currentSensorIndex].occupied);

      _currentSensorIndex++;
      if (_currentSensorIndex >= TOTAL_SPOTS) {
        _currentSensorIndex = 0;
      }
    }

    _lastSensorMeasureMs = currentMillis;
  }

  if (currentMillis - _lastUiRefreshMs >= _uiRefreshIntervalMs) {
    drawStatusScreen();
    _lastUiRefreshMs = currentMillis;
  }
}

void IRAM_ATTR Parking::handleEchoChangeISR() {
  if (_instance != NULL) {
    _instance->handleEchoChange();
  }
}

void IRAM_ATTR Parking::handleEchoChange() {
  if (_activeEchoPin < 0) {
    return;
  }

  int pinState = digitalRead(_activeEchoPin);
  unsigned long nowUs = micros();

  if (!_echoRiseDetected && pinState == HIGH) {
    _echoStartUsInterrupt = nowUs;
    _echoRiseDetected = true;
  } else if (_echoRiseDetected && pinState == LOW) {
    _echoEndUsInterrupt = nowUs;
    _echoMeasurementDone = true;
  }
}

/**
 * Updates one sensor step by step without pulseIn().
 * Returns true when a full measurement is finished.
 */
bool Parking::updateDistanceMeasurement(ParkingSpot& spot) {
  unsigned long currentMicros = micros();

  switch (spot.state) {
    case IDLE:
      _activeEchoPin = spot.echoPin;
      _echoRiseDetected = false;
      _echoMeasurementDone = false;
      _echoStartUsInterrupt = 0;
      _echoEndUsInterrupt = 0;

      // https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/
      // https://docs.arduino.cc/language-reference/en/functions/external-interrupts/digitalPinToInterrupt/
      attachInterrupt(digitalPinToInterrupt(_activeEchoPin), handleEchoChangeISR, CHANGE);

      digitalWrite(_trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(_trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(_trigPin, LOW);

      spot.triggerTimeUs = micros();
      spot.state = WAITING_FOR_ECHO_START;
      return false;

    case WAITING_FOR_ECHO_START:
      if (_echoRiseDetected) {
        spot.echoStartUs = _echoStartUsInterrupt;

        spot.state = WAITING_FOR_ECHO_END;
      } else if (currentMicros - spot.triggerTimeUs >= _echoTimeoutMicroseconds) {

        // https://docs.arduino.cc/language-reference/en/functions/external-interrupts/detachInterrupt/
        // https://docs.arduino.cc/language-reference/en/functions/external-interrupts/digitalPinToInterrupt/
        detachInterrupt(digitalPinToInterrupt(_activeEchoPin));
        _activeEchoPin = -1;

        spot.distance = _invalidDistanceCm;
        spot.state = IDLE;
        return true;
      }
      return false;

    case WAITING_FOR_ECHO_END:
      if (_echoMeasurementDone) {
        unsigned long localEchoEndUs = 0;

        localEchoEndUs = _echoEndUsInterrupt;
        _echoMeasurementDone = false;

        detachInterrupt(digitalPinToInterrupt(_activeEchoPin));
        _activeEchoPin = -1;

        if (localEchoEndUs > spot.echoStartUs) {
          unsigned long duration = localEchoEndUs - spot.echoStartUs;
          spot.distance = (duration * _soundSpeed) / _echoTravelDivider;
        } else {
          spot.distance = _invalidDistanceCm;
        }

        spot.state = IDLE;
        return true;
      } else if (currentMicros - spot.echoStartUs >= _echoTimeoutMicroseconds) {
        detachInterrupt(digitalPinToInterrupt(_activeEchoPin));
        _activeEchoPin = -1;

        spot.distance = _invalidDistanceCm;
        spot.state = IDLE;
        return true;
      }
      return false;
  }

  return false;
}

/**
 * @brief Updates whether a parking spot is occupied or free.
 *
 * Invalid distance values are ignored. A free spot becomes occupied when the
 * distance is below PARKED_THRESHOLD_ON_CM. An occupied spot becomes free when
 * the distance is above PARKED_THRESHOLD_OFF_CM.
 *
 * @param distanceCm Measured distance in centimeters. Negative values are invalid.
 * @param isOccupied Current occupied state of the parking spot. Updated when needed.
 */
void Parking::updateOccupiedState(float distanceCm, bool& isOccupied) {

  // Ignore invalid distance values.
  if (distanceCm < 0) {
    return;
  }

  // Mark the spot as occupied when the measured distance is below the occupied threshold.
  if (!isOccupied && distanceCm < _parkedThresholdOnCm) {
    isOccupied = true;
  }
  // Mark the spot as free when the measured distance is above the free threshold.
  else if (isOccupied && distanceCm > _parkedThresholdOffCm) {
    isOccupied = false;
  }
}

/**
 * @brief Returns the text for the current parking spot state.
 *
 * @param isOccupied Current occupied state of the parking spot.
 * @return "OCCUPIED" if the spot is occupied, otherwise "FREE".
 */
const char* Parking::getStateText(bool isOccupied) {

  // Return the correct text for the current parking spot state.
  return isOccupied ? "OCCUPIED" : "FREE";
}

/**
 * @brief Counts how many parking spots are free.
 *
 * A parking spot is only counted when the distance value is valid
 * and the spot is not occupied.
 *
 * @return Number of free parking spots.
 */
int Parking::countAvailableSpots() {

  // Calculate the total number of parking spots in the array.
  const int8_t TOTAL_SPOTS = sizeof(_parkingSpots) / sizeof(_parkingSpots[0]);

  // Start the counter for free parking spots at zero.
  int availableSpots = 0;

  // Go through all parking spots one by one.
  for (int i = 0; i < TOTAL_SPOTS; i++) {

    // Count the spot when the distance is valid and the spot is free.
    if (_parkingSpots[i].distance >= 0 && !_parkingSpots[i].occupied) {
      availableSpots++;
    }
  }

  return availableSpots;
}

/**
 * @brief Draws the parking status screen on the OLED display.
 *
 * The screen shows the state of each parking spot and the total number
 * of free parking spots.
 */
void Parking::drawStatusScreen() {

  // Calculate the total number of parking spots in the array.
  const int8_t TOTAL_SPOTS = sizeof(_parkingSpots) / sizeof(_parkingSpots[0]);

  // Get the current number of free parking spots.
  int availableSpots = countAvailableSpots();

  // Clear the old screen content.
  _display.clearDisplay();

  // Set the text size to normal 
  _display.setTextSize(1);

  // Set the text color to white.
  _display.setTextColor(SSD1306_WHITE);

  // Set the text position to the top-left corner of the screen.
  _display.setCursor(0, 0);

  // Show the screen title.
  _display.println("Parking");

  // Move to the next empty line.
  _display.println();

  // Show the status of each parking spot.
  for (int i = 0; i < TOTAL_SPOTS; i++) {
    _display.print("P");
    _display.print(i + 1);
    _display.print(": ");

    // Show NO DATA when the distance measurement is invalid.
    if (_parkingSpots[i].distance < 0) {
      _display.println("NO DATA");
    } else {

      // Show OCCUPIED or FREE when the measurement is valid.
      _display.println(getStateText(_parkingSpots[i].occupied));
    }
  }

  // Draw a vertical line between the spot list and the free counter.
  _display.drawLine(78, 0, 78, 63, SSD1306_WHITE);

  // Set the text size for the label.
  _display.setTextSize(1);

  // Set the text position for the label.
  _display.setCursor(88, 4);

  // Show the label for free parking spots.
  _display.println("Free");

  // Set the text size for the number display.
  _display.setTextSize(3);

  // Set the text position for the number.
  _display.setCursor(95, 24);

  // Show the number of free parking spots.
  _display.print(availableSpots);

  // Send everything to the OLED display.
  _display.display();
}