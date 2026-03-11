#include <Arduino.h>
#include <ESP32Servo.h>

// Output LED pins (warning lights)
#define LED_1 37
#define LED_2 36

// Servo pin
#define SERVO_PIN 18

// Input button pin
#define BTN 45

// distances
#define A_B_DISTANCE 200
#define B_C_DISTANCE 1000

// Oled screen width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

enum State {
  IDLE,
  MEASURING,
  WAITING
};

State currentState = IDLE;

Servo servo;

const unsigned int safetyMargin = 5000;

unsigned long startMillis;
unsigned long predictedTime;

bool buttonState = HIGH;
bool lastReading = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// unsigned long lastDisplayUpdate = 0;
// const unsigned long displayInterval = 100;

unsigned long blinkTimer = 0;
const unsigned long blinkInterval = 400;
bool ledToggle = false;

const int BARRIER_OPEN = 0;
const int BARRIER_CLOSED = 90;
int currentBarrierPos = BARRIER_OPEN;

void handleServo() {

  if (currentState != WAITING) {
    if (currentBarrierPos != BARRIER_OPEN) {
      servo.write(BARRIER_OPEN);
      currentBarrierPos = BARRIER_OPEN;
    }
    return;
  }

  unsigned long elapsed = millis() - startMillis;

  if (elapsed >= predictedTime && currentBarrierPos != BARRIER_CLOSED) {
    servo.write(BARRIER_CLOSED);
    currentBarrierPos = BARRIER_CLOSED;
  }

}

void handleWarningLEDS() {

  if (currentState != WAITING) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    return;
  }

  unsigned long elapsed = millis() - startMillis;

  if (elapsed >= predictedTime - safetyMargin) {

    if (millis() - blinkTimer >= blinkInterval) {
      blinkTimer = millis();

      ledToggle = !ledToggle;

      digitalWrite(LED_1, ledToggle);
      digitalWrite(LED_2, !ledToggle);
    }

  } else {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
  }
}

void handleButton() {
  bool reading = digitalRead(BTN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        switch (currentState) {

          case IDLE:
            startMillis = millis();
            currentState = MEASURING;
            break;

          case MEASURING:
            predictedTime = (millis() - startMillis) * (B_C_DISTANCE / A_B_DISTANCE);
            startMillis = millis();
            currentState = WAITING;
            break;

          case WAITING:
            currentState = IDLE;
            digitalWrite(LED_1, LOW);
            digitalWrite(LED_2, LOW);
            break;
        }
      }
    }
  }

  lastReading = reading;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
	servo.attach(SERVO_PIN, 500, 2400);
  servo.write(BARRIER_OPEN);
  Serial.println("Setup complete");
}

void loop() {
  handleButton();
  handleWarningLEDS();
  handleServo();
}