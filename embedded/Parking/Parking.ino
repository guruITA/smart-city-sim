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
const float PARKED_THRESHOLD_OFF_CM = 15.0f;
const float INVALID_DISTANCE_CM = -1.0f;
const float ECHO_TRAVEL_DIVIDER = 2.0f;

const unsigned long ECHO_TIMEOUT_MICROSECONDS = 100000UL;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long lastUiRefreshMs = 0;

/**
 * 
 */
struct ParkingSpot {
  int8_t echoPin;
  float distance;
  bool occupied;
};

ParkingSpot parkingSpots[] = {{ECHO1_PIN, INVALID_DISTANCE_CM, false},
                              {ECHO2_PIN, INVALID_DISTANCE_CM, false},
                              {ECHO3_PIN, INVALID_DISTANCE_CM, false},
                              {ECHO4_PIN, INVALID_DISTANCE_CM, false}};

const int8_t TOTAL_SPOTS = sizeof(parkingSpots) / sizeof(parkingSpots[0]);

/**
 * 
 */
float readDistanceOnceCm(int echoPin) {
  // Sets the trigger pin LOW to make sure the sensor starts with a clean signal
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send a short trigger pulse of 10 microseconds to start the ultrasonic measurement
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measuring how long the echo pin stays HIGH
  // This is the time the sound wave needed to travel to the object and back
  long duration = pulseIn(echoPin, HIGH, ECHO_TIMEOUT_MICROSECONDS);

  // If no valid echo is received within the given timeout an invalid distance value is returned
  if (duration <= 0) {
    return INVALID_DISTANCE_CM;
  }

  // Converted the measured travel time into distance in centimeters
  // Divideed by 2 because the sound travels to the object and back
  return (duration * SOUND_SPEED) / ECHO_TRAVEL_DIVIDER;
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

void drawStatusScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("Parking Status");
  display.println("---------------------");

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

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  for (int i = 0; i < TOTAL_SPOTS; i++) {
    pinMode(parkingSpots[i].echoPin, INPUT);
  }

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
}

void loop() {
  for (int i = 0; i < TOTAL_SPOTS; i++) {
    parkingSpots[i].distance = readDistanceOnceCm(parkingSpots[i].echoPin);
    updateOccupiedState(parkingSpots[i].distance, parkingSpots[i].occupied);
    delay(80);
  }

  if (millis() - lastUiRefreshMs > 250) {
    drawStatusScreen();
    lastUiRefreshMs = millis();
  }

  delay(100);
}