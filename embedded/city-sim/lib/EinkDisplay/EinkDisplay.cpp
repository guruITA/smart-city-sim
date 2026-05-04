#include "EinkDisplay.h"
#include <SPI.h>
#include <GxEPD2_3C.h>
using Display = GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT>;

static SPIClass _spi(HSPI);
static Display* _disp = nullptr;

EinkDisplay::EinkDisplay(int clkPin, int mosiPin, int csPin, int dcPin, int rstPin, int busyPin)
    : _clkPin(clkPin), _mosiPin(mosiPin), _csPin(csPin), _dcPin(dcPin), _rstPin(rstPin), _busyPin(busyPin) {}

void EinkDisplay::begin() {
    _spi.begin(_clkPin, /*miso=*/-1, _mosiPin, /*ss=*/-1);
    _disp = new Display(GxEPD2_290_C90c(_csPin, _dcPin, _rstPin, _busyPin));
    _disp->epd2.selectSPI(_spi, SPISettings(1000000, MSBFIRST, SPI_MODE0));
    _disp->init(115200, true, 10, false);
    showText("The Embedded alliance");

}

void EinkDisplay::showText(const String& text) {
    if (!_disp) return;
    _disp->setRotation(3);
    _disp->setFont(&FreeMonoBold9pt7b);
    _disp->setTextColor(GxEPD_BLACK);
    _disp->setFullWindow();
    _disp->firstPage();
    do {
        _disp->fillScreen(GxEPD_WHITE);
        _disp->setCursor(10, 40);
        _disp->print(text);
    } while (_disp->nextPage());
}

void EinkDisplay::update() {}
