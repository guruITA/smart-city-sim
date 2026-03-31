#include <Arduino.h>
#include <FastLED.h>

#define LED_FASTLED 48 // set wire
// #define BOOTBUTTON 0 // set wire
#define NUMPIXELS 1 // set number of leds

CRGB leds[NUMPIXELS];

// bool stateActive = true;
// bool buttonState = HIGH;

void showRandomColor() {
  leds[0] = CRGB(random(0, 256), random(0, 256), random(0, 256));
  FastLED.show();
}

void turnOffFastLed() {
  leds[0] = CRGB::Black;
  FastLED.show();
}

// void toggleActiveFastLed() {
//   bool currentButton = digitalRead(BOOTBUTTON);
//   if (buttonState == HIGH && currentButton == LOW) {
//     Serial.println("Switching FastLed");
//     stateActive = !stateActive;
//   }
//   buttonState = currentButton
// }

void resetFastLed() {
  FastLED.clear();
  FastLED.show();
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    // pinMode(BOOTBUTTON, INPUT_PULLUP);
    FastLED.addLeds<WS2812, LED_FASTLED, GRB>(leds, NUMPIXELS);
    resetFastLed();
}
  
void loop() {
  // toggleActiveFastLed();

  showRandomColor();
  delay(random(500, 3000));
  turnOffFastLed();
  delay(random(500, 1200)); 
}