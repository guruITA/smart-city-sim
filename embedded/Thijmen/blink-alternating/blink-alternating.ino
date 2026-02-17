#include <Arduino.h>
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 750;

int LED_LEFT = 19;
int LED_RIGHT = 18;

bool leftOn = true;

void setup()
{
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  startMillis = millis();
}

void alternateBlink(){
  currentMillis = millis();

  if (currentMillis - startMillis >= period)
  {
    startMillis = currentMillis;

    if (leftOn)
    {
      digitalWrite(LED_LEFT, HIGH);
      digitalWrite(LED_RIGHT, LOW);
    }
    else
    {
      digitalWrite(LED_LEFT, LOW);
      digitalWrite(LED_RIGHT, HIGH);
    }

    leftOn = !leftOn;
  }
}

void loop()
{
  alternateBlink();
}
