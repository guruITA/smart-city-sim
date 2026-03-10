#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Output LED pins (warning lights)
#define LED_1 37
#define LED_2 36

// Input button pin
#define BTN 45

// distances
#define A_B_DISTANCE 200
#define B_C_DISTANCE 1000

// Oled screen pins
#define SCL_PIN 13
#define SDA_PIN 14

// Oled screen width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

enum State {
  IDLE,
  MEASURING,
  WAITING
};

State currentState = IDLE;

const unsigned int safetyMargin = 5000;

unsigned long startMillis;
unsigned long predictedTime;

bool buttonState = HIGH;
bool lastReading = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long lastDisplayUpdate = 0;
const unsigned long displayInterval = 100;

// Warning light variables
unsigned long blinkTimer = 0;
const unsigned long blinkInterval = 400;
bool ledToggle = false;

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

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  switch (currentState) {

    case IDLE:
      display.println("IDLE");
      break;

    case MEASURING:
      display.println("MEASURING");
      display.setTextSize(1);
      display.print("Elapsed: ");
      display.print((millis() - startMillis) / 1000.0, 2);
      display.println(" s");
      break;

    case WAITING:
      display.println("WAITING");
      display.setTextSize(1);
      display.print("Predicted: ");
      display.print(predictedTime / 1000.0, 2);
      display.println(" s");

      unsigned long remaining =
        (millis() - startMillis >= predictedTime)
        ? 0
        : predictedTime - (millis() - startMillis);

      display.print("Remaining: ");
      display.print(remaining / 1000.0, 2);
      display.println(" s");
      break;
  }

  display.display();
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  handleButton();
  handleWarningLEDS();

  if (millis() - lastDisplayUpdate > displayInterval) {
    lastDisplayUpdate = millis();
    updateDisplay();
  }
}