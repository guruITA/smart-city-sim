#include "StreetLight.h"
#include "TrainPredictionSignal.h"
#define bulshit LED_BUILTIN
StreetLight lamp(4, 5, 650, 1000);
TrainPredictionSignal trainSignal(37, 36, 42, 18, 45);

void setup() {
  pinMode(bulshit, OUTPUT);
  digitalWrite(bulshit, LOW);
  lamp.begin();
  trainSignal.begin();
}

void loop() {
  lamp.update();
  trainSignal.update();
}