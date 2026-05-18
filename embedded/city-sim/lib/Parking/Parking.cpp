#include "Parking.h"

Parking::Parking(uint8_t mcpAddress, uint8_t trig1Pin, uint8_t trig2Pin, uint8_t trig3Pin,
                 uint8_t trig4Pin, uint8_t echo1Pin, uint8_t echo2Pin, uint8_t echo3Pin,
                 uint8_t echo4Pin, uint8_t oledSdaPin, uint8_t oledSclPin, int screenWidth,
                 int screenHeight, uint8_t oledAddr, float soundSpeed, float parkedThresholdOnCm,
                 float parkedThresholdOffCm, float invalidDistanceCm, float echoTravelDivider,
                 unsigned long echoTimeoutMicroseconds, unsigned long uiRefreshIntervalMs,
                 unsigned long sensorMeasureIntervalMs)
    : _mcpAddress(mcpAddress), _oledSdaPin(oledSdaPin), _oledSclPin(oledSclPin),
      _screenWidth(screenWidth), _screenHeight(screenHeight), _oledAddr(oledAddr),
      _soundSpeed(soundSpeed), _parkedThresholdOnCm(parkedThresholdOnCm),
      _parkedThresholdOffCm(parkedThresholdOffCm), _invalidDistanceCm(invalidDistanceCm),
      _echoTravelDivider(echoTravelDivider), _echoTimeoutMicroseconds(echoTimeoutMicroseconds),
      _uiRefreshIntervalMs(uiRefreshIntervalMs), _sensorMeasureIntervalMs(sensorMeasureIntervalMs),
      _lastUiRefreshMs(0), _lastSensorMeasureMs(0), _currentSensorIndex(0), _parkingWire(0),
      _display(screenWidth, screenHeight, &_parkingWire, -1) {

  _parkingSpots[0] = {trig1Pin, echo1Pin, _invalidDistanceCm, false, IDLE, 0, 0, 0, 0};
  _parkingSpots[1] = {trig2Pin, echo2Pin, _invalidDistanceCm, false, IDLE, 0, 0, 0, 0};
  _parkingSpots[2] = {trig3Pin, echo3Pin, _invalidDistanceCm, false, IDLE, 0, 0, 0, 0};
  _parkingSpots[3] = {trig4Pin, echo4Pin, _invalidDistanceCm, false, IDLE, 0, 0, 0, 0};
}

/**
 * @brief Initializes the parking system hardware.
 *
 * Sets up serial communication, sensor pins, I2C communication,
 * and the OLED display.
 */
void Parking::begin() {

  // Calculate the total number of parking spots in the array.
  const int8_t TOTAL_SPOTS = sizeof(_parkingSpots) / sizeof(_parkingSpots[0]);

  // Start serial communication.
  Serial.begin(115200);

  // Start I2C communication for the OLED display.
  _parkingWire.begin(_oledSdaPin, _oledSclPin);
  _parkingWire.setClock(50000);

  // Start MCP23017 communication.
  if (!_mcp.begin_I2C(_mcpAddress, &_parkingWire)) {
    Serial.println("MCP23017 not found");
  } else {
    Serial.println("MCP23017 started");
  }

  // Set each trigger and echo pin.
  for (int i = 0; i < TOTAL_SPOTS; i++) {

    // Set the trigger pin as output.
    _mcp.pinMode(_parkingSpots[i].triggerPin, OUTPUT);

    // Set the trigger pin to LOW at startup.
    _mcp.digitalWrite(_parkingSpots[i].triggerPin, LOW);

    // Set each echo pin as input.
    _mcp.pinMode(_parkingSpots[i].echoPin, INPUT);
  }

  // Initialize the OLED display.
  _display.begin(SSD1306_SWITCHCAPVCC, _oledAddr);
}

/**
 * @brief Updates the parking system.
 *
 * Handles sensor measurements step by step and refreshes the display.
 */
void Parking::update() {

  // Calculate the total number of parking spots in the array.
  const int8_t TOTAL_SPOTS = sizeof(_parkingSpots) / sizeof(_parkingSpots[0]);

  // Get the current time in milliseconds.
  unsigned long currentMillis = millis();

  // Check if it is time to update the sensor measurement.
  if (currentMillis - _lastSensorMeasureMs >= _sensorMeasureIntervalMs) {

    // Run one measurement step for the current parking spot.
    bool measurementFinished = updateDistanceMeasurement(_parkingSpots[_currentSensorIndex]);

    // When the measurement is finished, update the occupied state.
    if (measurementFinished) {
      updateOccupiedState(_parkingSpots[_currentSensorIndex]);

      // Move to the next parking spot.
      _currentSensorIndex++;

      // Go back to the first spot when all spots are done.
      if (_currentSensorIndex >= TOTAL_SPOTS) {
        _currentSensorIndex = 0;
      }
    }

    // Store the time of the last sensor update.
    _lastSensorMeasureMs = currentMillis;
  }

  // Check if it is time to refresh the display.
  if (currentMillis - _lastUiRefreshMs >= _uiRefreshIntervalMs) {

    // Draw the current parking status on the screen.
    drawStatusScreen();

    // Store the time of the last display update.
    _lastUiRefreshMs = currentMillis;
  }
}

/**
 * @brief Updates the distance measurement for one parking spot.
 *
 * This function measures one sensor step by step. It returns true when
 * the measurement is finished, and false when the measurement is still running.
 *
 * @param spot Parking spot that is being measured.
 * @return true when the measurement is finished, otherwise false.
 */
bool Parking::updateDistanceMeasurement(ParkingSpot& spot) {

  // Get the current time in microseconds.
  unsigned long currentMicros = micros();

  // Run the correct measurement step for this parking spot.
  switch (spot.state) {

  case IDLE:
    // Send a short trigger pulse to start the ultrasonic sensor.
    _mcp.digitalWrite(spot.triggerPin, LOW);
    delayMicroseconds(2);
    _mcp.digitalWrite(spot.triggerPin, HIGH);
    delayMicroseconds(10);
    _mcp.digitalWrite(spot.triggerPin, LOW);

    // Store the time when the measurement started.
    spot.triggerTimeUs = micros();

    // Move to the next step: wait for the echo signal to start.
    spot.state = WAITING_FOR_ECHO_START;

    // The measurement has started, but is not finished yet.
    return false;

  case WAITING_FOR_ECHO_START:
    // Check if the echo signal has started.
    if (_mcp.digitalRead(spot.echoPin) == HIGH) {

      // Store the echo start time for this parking spot.
      spot.echoStartUs = currentMicros;

      // Move to the next step: wait for the echo signal to end.
      spot.state = WAITING_FOR_ECHO_END;

    } else if (currentMicros - spot.triggerTimeUs >= _echoTimeoutMicroseconds) {

      // Mark the measurement as invalid.
      spot.distance = _invalidDistanceCm;

      // Reset the state so this spot can be measured again later.
      spot.state = IDLE;

      // The measurement is finished, but failed because of timeout.
      return true;
    }

    // The measurement is still waiting for the echo signal to start.
    return false;

  case WAITING_FOR_ECHO_END:
    // Check if the echo signal has ended.
    if (_mcp.digitalRead(spot.echoPin) == LOW) {

      // Copy the echo end time.
      unsigned long localEchoEndUs = currentMicros;

      // Calculate the distance if the timing is valid.
      if (localEchoEndUs > spot.echoStartUs) {
        unsigned long duration = localEchoEndUs - spot.echoStartUs;
        spot.distance = (duration * _soundSpeed) / _echoTravelDivider;
      } else {

        // Mark the measurement as invalid when the timing is wrong.
        spot.distance = _invalidDistanceCm;
      }

      // Reset the state so this spot can be measured again later.
      spot.state = IDLE;

      // The measurement is finished.
      return true;

    } else if (currentMicros - spot.echoStartUs >= _echoTimeoutMicroseconds) {

      // Mark the measurement as invalid.
      spot.distance = _invalidDistanceCm;

      // Reset the state so this spot can be measured again later.
      spot.state = IDLE;

      // The measurement is finished, but failed because of timeout.
      return true;
    }

    // The measurement is still waiting for the echo signal to end.
    return false;
  }

  // Fallback return. This should normally not be reached.
  return false;
}

/**
 * @brief Updates whether a parking spot is occupied or free.
 *
 * Invalid distance values are ignored. A free spot becomes occupied when the
 * distance is below PARKED_THRESHOLD_ON_CM. An occupied spot becomes free when
 * the distance is above PARKED_THRESHOLD_OFF_CM.
 *
 * @param spot Parking spot that is being updated.
 */
void Parking::updateOccupiedState(ParkingSpot& spot) {

  const uint8_t REQUIRED_COUNT = 3;

  // Mark the spot as probably occupied when the measurement is invalid.
  if (spot.distance < 0) {
    spot.occupiedCount++;
    spot.freeCount = 0;
  }
  // Mark the spot as probably occupied when the measured distance is below the occupied threshold.
  else if (spot.distance < _parkedThresholdOnCm) {
    spot.occupiedCount++;
    spot.freeCount = 0;
  }
  // Mark the spot as probably free when the measured distance is above the free threshold.
  else if (spot.distance > _parkedThresholdOffCm) {
    spot.freeCount++;
    spot.occupiedCount = 0;
  }
  // Keep the current state when the distance is between both thresholds.
  else {
    spot.occupiedCount = 0;
    spot.freeCount = 0;
  }

  // Only change to occupied after multiple matching measurements.
  if (spot.occupiedCount >= REQUIRED_COUNT) {
    spot.occupied = true;
    spot.occupiedCount = REQUIRED_COUNT;
  }

  // Only change to free after multiple matching measurements.
  if (spot.freeCount >= REQUIRED_COUNT) {
    spot.occupied = false;
    spot.freeCount = REQUIRED_COUNT;
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

    // Count the spot when the confirmed state is free.
    if (!_parkingSpots[i].occupied) {
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
      _display.println(getStateText(_parkingSpots[i].occupied));
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