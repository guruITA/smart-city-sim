#ifndef EINK_DISPLAY_H
#define EINK_DISPLAY_H

#include <Arduino.h>

class EinkDisplay {
public:
    EinkDisplay(int clkPin, int mosiPin, int csPin, int dcPin, int rstPin, int busyPin);
    void begin();
    void showText(const String& text);
    void showState(const String& text, const String& textColor, bool imageEnabled,
                   const String& imageColor, int imageWidth, int imageHeight,
                   const String& imageRows);
    void startSyncTask();
    void update();

private:
    int _clkPin, _mosiPin, _csPin, _dcPin, _rstPin, _busyPin;
    String _currentText;
    String _currentPayload;
    static void syncTaskLoop(void* param);
};

#endif
