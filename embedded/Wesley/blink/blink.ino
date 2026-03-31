#include <Arduino.h>
#include <FastLED.h>

#define LED_FASTLED 48
#define NUMPIXELS 1 

CRGB leds[NUMPIXELS];

void showRandomColor() {
  leds[0] = CRGB(random(0, 256), random(0, 256), random(0, 256));
  FastLED.show();
}

void turnOffFastLed() {
  leds[0] = CRGB::Black;
  FastLED.show();
}

void resetFastLed() {
  FastLED.clear();
  FastLED.show();
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    FastLED.addLeds<WS2812, LED_FASTLED, GRB>(leds, NUMPIXELS);
    resetFastLed();
}
  
void loop() {
  showRandomColor();
  delay(random(500, 3000));
  turnOffFastLed();
  delay(random(500, 1200)); 
}