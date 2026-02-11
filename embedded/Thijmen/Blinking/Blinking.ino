#include <Arduino.h>
#include <FastLED.h>

#define LED_FASTLED 48
#define LED_RIGHT 12
#define BOOTBTN 0

#define NUMPIXELS 1
CRGB leds[NUMPIXELS];

const unsigned long period = 750;
unsigned long startMillis;

bool leftOn = true;
bool blinkOn = true;
bool lastButtonState = HIGH;

void setRandomColor() {
  leds[0] = CRGB(random(0, 256), random(0, 256), random(0, 256));
  FastLED.show();
}

void turnOffFastLED() {
  leds[0] = CRGB::Black;
  FastLED.show();
}

void handleBtn() {
  bool currentState = digitalRead(BOOTBTN);
  if (lastButtonState == HIGH && currentState == LOW) {
    Serial.println("Toggle!");
    blinkOn = !blinkOn;
  }
  lastButtonState = currentState;
}

void alternateBlink() {
  if (millis() - startMillis >= period) {
    startMillis = millis();

    if (leftOn) {
      setRandomColor();
      digitalWrite(LED_RIGHT, LOW);
    } else {
      turnOffFastLED();
      digitalWrite(LED_RIGHT, HIGH);
    }

    leftOn = !leftOn;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(BOOTBTN, INPUT_PULLUP);

  FastLED.addLeds<WS2812, LED_FASTLED, GRB>(leds, NUMPIXELS);
  FastLED.clear();
  FastLED.show();

  randomSeed(analogRead(34));
  startMillis = millis();
}

void loop() {
  handleBtn();

  if (blinkOn) {
    alternateBlink();
  } else {
    turnOffFastLED();
    digitalWrite(LED_RIGHT, LOW);
  }
}
