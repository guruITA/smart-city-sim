#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA_PIN 8
#define OLED_SCL_PIN 9

#define TRIG_PIN 13
#define ECHO1_PIN 14
#define ECHO2_PIN 15

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

bool displayReady = false;

float distance1 = INVALID_DISTANCE_CM;
float distance2 = INVALID_DISTANCE_CM;

bool occupied1 = false;
bool occupied2 = false;

unsigned long lastUiRefreshMs = 0;

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

void updateOccupiedState(float distanceCm, bool& occupied) {
  if (distanceCm < 0) {
    return;
  }

  if (!occupied && distanceCm < PARKED_THRESHOLD_ON_CM) {
    occupied = true;
  } else if (occupied && distanceCm > PARKED_THRESHOLD_OFF_CM) {
    occupied = false;
  }
}

const char* getStateText(bool occupied) {
  return occupied ? "OCCUPPIED" : "FREE";
}

void drawStatusScreen() {
  if (!displayReady) {
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("Parking Status");
  display.println("---------------------");

  display.print("P1: ");
  if (distance1 < 0) {
    display.println("NO DATA");
  } else {
    display.println(getStateText(occupied1));
  }

  display.print("P2: ");
  if (distance2 < 0) {
    display.println("NO DATA");
  } else {
    display.println(getStateText(occupied2));
  }

  display.print("P3: ");
  if (distance2 < 0) {
    display.println("NOT DATA");
  } else {
    display.println(getStateText(occupied2));
  }

  display.print("P4: ");
  if (distance2 < 0) {
    display.println("NOT DATA");
  } else {
    display.println(getStateText(occupied2));
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO1_PIN, INPUT);
  pinMode(ECHO2_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    displayReady = true;
    delay(1500);
  } else {
    Serial.println("OLED not found.");
  }

  Serial.println("ESP32-S3 2x HC-SR04 + OLED started.");
}

void loop() {
  distance1 = readDistanceOnceCm(ECHO1_PIN);
  updateOccupiedState(distance1, occupied1);

  Serial.print("P1 -> Afstand: ");
  if (distance1 < 0) {
    Serial.print("NO DATA");
  }
  
  Serial.print(" -> Status: ");
  Serial.println(getStateText(occupied1));

  delay(80);

  distance2 = readDistanceOnceCm(ECHO2_PIN);
  updateOccupiedState(distance2, occupied2);

  Serial.print("P2 -> Afstand: ");
  if (distance2 < 0) {
    Serial.print("NO DATA");
  }

  Serial.print(" -> Status: ");
  Serial.println(getStateText(occupied2));

  Serial.println("--------------------");

  if (millis() - lastUiRefreshMs > 250) {
    drawStatusScreen();
    lastUiRefreshMs = millis();
  }

  delay(100);
}