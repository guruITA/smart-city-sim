#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA_PIN 8
#define OLED_SCL_PIN 9

#define TRIG_PIN 13
#define ECHO1_PIN 14
#define ECHO2_PIN 15
#define ECHO3_PIN 3
#define ECHO4_PIN 10

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

const float SOUND_SPEED = 0.0343f;
const float PARKED_THRESHOLD_ON_CM = 5.0f;
const float PARKED_THRESHOLD_OFF_CM = 10.0f;
const float INVALID_DISTANCE_CM = -1.0f;
const float ECHO_TRAVEL_DIVIDER = 2.0f;
const unsigned long ECHO_TIMEOUT_MICROSECONDS = 100000UL;
const unsigned long UI_REFRESH_INTERVAL_MS = 250UL;
const unsigned long SENSOR_MEASURE_INTERVAL_MS = 80UL;

unsigned long lastUiRefreshMs = 0;
unsigned long lastSensorMeasureMs = 0;
int8_t currentSensorIndex = 0;

TwoWire parkingWire = TwoWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &parkingWire, -1);

/**
 * 
 */
enum SensorState { IDLE, WAITING_FOR_ECHO_START, WAITING_FOR_ECHO_END };

/**
 * 
 */
struct ParkingSpot {
  int8_t echoPin;
  float distance;
  bool occupied;
  SensorState state;
  unsigned long triggerTimeUs;
  unsigned long echoStartUs;
};

ParkingSpot parkingSpots[] = {{ECHO1_PIN, INVALID_DISTANCE_CM, false, IDLE, 0, 0},
                              {ECHO2_PIN, INVALID_DISTANCE_CM, false, IDLE, 0, 0},
                              {ECHO3_PIN, INVALID_DISTANCE_CM, false, IDLE, 0, 0},
                              {ECHO4_PIN, INVALID_DISTANCE_CM, false, IDLE, 0, 0}};

const int8_t TOTAL_SPOTS = sizeof(parkingSpots) / sizeof(parkingSpots[0]);

volatile bool echoRiseDetected = false;
volatile bool echoMeasurementDone = false;
volatile unsigned long echoStartUsInterrupt = 0;
volatile unsigned long echoEndUsInterrupt = 0;
volatile int activeEchoPin = -1;

/**
 * Interrupt handler for the currently active echo pin.
 */
void IRAM_ATTR handleEchoChange() {
  if (activeEchoPin < 0) {
    return;
  }

  int pinState = digitalRead(activeEchoPin);
  unsigned long nowUs = micros();

  if (!echoRiseDetected && pinState == HIGH) {
    echoStartUsInterrupt = nowUs;
    echoRiseDetected = true;
  } else if (echoRiseDetected && pinState == LOW) {
    echoEndUsInterrupt = nowUs;
    echoMeasurementDone = true;
  }
}

/**
 * Updates one sensor step by step without pulseIn().
 * Returns true when a full measurement is finished.
 */
bool updateDistanceMeasurement(ParkingSpot& spot) {
  unsigned long currentMicros = micros();

  switch (spot.state) {
  case IDLE:
    activeEchoPin = spot.echoPin;
    echoRiseDetected = false;
    echoMeasurementDone = false;
    echoStartUsInterrupt = 0;
    echoEndUsInterrupt = 0;

    attachInterrupt(digitalPinToInterrupt(activeEchoPin), handleEchoChange, CHANGE);

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    spot.triggerTimeUs = micros();
    spot.state = WAITING_FOR_ECHO_START;
    return false;

  case WAITING_FOR_ECHO_START:
    if (echoRiseDetected) {
      noInterrupts();
      spot.echoStartUs = echoStartUsInterrupt;
      interrupts();

      spot.state = WAITING_FOR_ECHO_END;
    } else if (currentMicros - spot.triggerTimeUs >= ECHO_TIMEOUT_MICROSECONDS) {
      detachInterrupt(digitalPinToInterrupt(activeEchoPin));
      activeEchoPin = -1;

      spot.distance = INVALID_DISTANCE_CM;
      spot.state = IDLE;
      return true;
    }
    return false;

  case WAITING_FOR_ECHO_END:
    if (echoMeasurementDone) {
      unsigned long localEchoEndUs = 0;

      noInterrupts();
      localEchoEndUs = echoEndUsInterrupt;
      echoMeasurementDone = false;
      interrupts();

      detachInterrupt(digitalPinToInterrupt(activeEchoPin));
      activeEchoPin = -1;

      if (localEchoEndUs > spot.echoStartUs) {
        unsigned long duration = localEchoEndUs - spot.echoStartUs;
        spot.distance = (duration * SOUND_SPEED) / ECHO_TRAVEL_DIVIDER;
      } else {
        spot.distance = INVALID_DISTANCE_CM;
      }

      spot.state = IDLE;
      return true;
    } else if (currentMicros - spot.echoStartUs >= ECHO_TIMEOUT_MICROSECONDS) {
      detachInterrupt(digitalPinToInterrupt(activeEchoPin));
      activeEchoPin = -1;

      spot.distance = INVALID_DISTANCE_CM;
      spot.state = IDLE;
      return true;
    }
    return false;
  }

  return false;
}

void updateOccupiedState(float distanceCm, bool& isOccupied) {
  if (distanceCm < 0) {
    return;
  }

  if (!isOccupied && distanceCm < PARKED_THRESHOLD_ON_CM) {
    isOccupied = true;
  } else if (isOccupied && distanceCm > PARKED_THRESHOLD_OFF_CM) {
    isOccupied = false;
  }
}

const char* getStateText(bool isOccupied) {
  return isOccupied ? "OCCUPIED" : "FREE";
}

/**
 * 
 */
int countAvailableSpots() {
  int availableSpots = 0;

  for (int i = 0; i < TOTAL_SPOTS; i++) {
    if (parkingSpots[i].distance >= 0 && !parkingSpots[i].occupied) {
      availableSpots++;
    }
  }

  return availableSpots;
}

void drawStatusScreen() {
  int availableSpots = countAvailableSpots();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Parking");
  display.println();

  for (int i = 0; i < TOTAL_SPOTS; i++) {
    display.print("P");
    display.print(i + 1);
    display.print(": ");

    if (parkingSpots[i].distance < 0) {
      display.println("NO DATA");
    } else {
      display.println(getStateText(parkingSpots[i].occupied));
    }
  }

  display.drawLine(78, 0, 78, 63, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(88, 4);
  display.println("Free");


  display.setTextSize(3);
  display.setCursor(95, 24);
  display.print(availableSpots);

  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  for (int i = 0; i < TOTAL_SPOTS; i++) {
    pinMode(parkingSpots[i].echoPin, INPUT);
  }

  parkingWire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastSensorMeasureMs >= SENSOR_MEASURE_INTERVAL_MS) {
    bool measurementFinished = updateDistanceMeasurement(parkingSpots[currentSensorIndex]);

    if (measurementFinished) {
      updateOccupiedState(parkingSpots[currentSensorIndex].distance, parkingSpots[currentSensorIndex].occupied);

      currentSensorIndex++;
      if (currentSensorIndex >= TOTAL_SPOTS) {
        currentSensorIndex = 0;
      }
    }

    lastSensorMeasureMs = currentMillis;
  }

  if (currentMillis - lastUiRefreshMs >= UI_REFRESH_INTERVAL_MS) {
    drawStatusScreen();
    lastUiRefreshMs = currentMillis;
  }
}