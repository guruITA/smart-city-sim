#include <Arduino.h>
#include <FastLED.h>

#define LED_FASTLED 48
#define BOOTBTN 0

#define NUMPIXELS 1
CRGB leds[NUMPIXELS];

const unsigned long period = 750;
unsigned long startMillis;

bool ledOn = true;
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

void blink() {
  if (millis() - startMillis >= period) {
    startMillis = millis();

    if (ledOn) {
      setRandomColor();
    } else {
      turnOffFastLED();
    }

    ledOn = !ledOn;
  }
}

void setup() {
  Serial.begin(115200);
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
    blink();
  } else {
    turnOffFastLED();
  }
}
