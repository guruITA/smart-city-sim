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

const char* Parking::getStateText(bool isOccupied) {
  return isOccupied ? "OCCUPIED" : "FREE";
}

/**
 * Counts the number of available parking spots based on the distance measurements and occupied states of all parking spots.
 */
int Parking::countAvailableSpots() {
  const int8_t TOTAL_SPOTS = sizeof(_parkingSpots) / sizeof(_parkingSpots[0]);
  int availableSpots = 0;

  for (int i = 0; i < TOTAL_SPOTS; i++) {
    if (_parkingSpots[i].distance >= 0 && !_parkingSpots[i].occupied) {
      availableSpots++;
    }
  }

  return availableSpots;
}

/**
 * Draws the status screen on the OLED display, showing the state of each parking spot and the total number of available spots.
 * 
 */
void Parking::drawStatusScreen() {
  const int8_t TOTAL_SPOTS = sizeof(_parkingSpots) / sizeof(_parkingSpots[0]);
  int availableSpots = countAvailableSpots();

  _display.clearDisplay();
  _display.setTextSize(1);
  _display.setTextColor(SSD1306_WHITE);
  _display.setCursor(0, 0);
  _display.println("Parking");
  _display.println();

  for (int i = 0; i < TOTAL_SPOTS; i++) {
    _display.print("P");
    _display.print(i + 1);
    _display.print(": ");

    if (_parkingSpots[i].distance < 0) {
      _display.println("NO DATA");
    } else {
      _display.println(getStateText(_parkingSpots[i].occupied));
    }
  }

  _display.drawLine(78, 0, 78, 63, SSD1306_WHITE);

  _display.setTextSize(1);
  _display.setCursor(88, 4);
  _display.println("Free");

  _display.setTextSize(3);
  _display.setCursor(95, 24);
  _display.print(availableSpots);

  _display.display();
}