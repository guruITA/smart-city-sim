#include "StreetLight.h"

StreetLight lamp(4, 5, 650, 1000);

void setup() {
  lamp.begin();
}

void loop() {
  lamp.update();
}