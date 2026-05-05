#ifndef EINK_DISPLAY_H
#define EINK_DISPLAY_H

#include <Arduino.h>

class EinkDisplay {
public:
    EinkDisplay(int clkPin, int mosiPin, int csPin, int dcPin, int rstPin, int busyPin);
    void begin();
    void showText(const String& text);
    void update();

private:
    int _clkPin, _mosiPin, _csPin, _dcPin, _rstPin, _busyPin;
};

#endif
