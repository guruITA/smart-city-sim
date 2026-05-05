#include "EinkDisplay.h"
#include <SPI.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "NetworkController.h"
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
    const String initText = "The Embedded alliance";
    _currentText = initText;
    showText(initText);

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

void EinkDisplay::startSyncTask() {
    xTaskCreatePinnedToCore(
        syncTaskLoop,
        "EinkSync",
        4096,
        (void*)this,
        1,
        nullptr,
        1
    );
    Serial.println("EinkDisplay sync task started on core 1");
}

void EinkDisplay::syncTaskLoop(void* param) {
    EinkDisplay* self = (EinkDisplay*)param;
    constexpr unsigned long SYNC_INTERVAL_MS = 10000;
    unsigned long lastSync = 0;
    int syncCount = 0;

    vTaskDelay(pdMS_TO_TICKS(2000));  

    while (true) {
        unsigned long now = millis();
        unsigned long elapsed = now - lastSync;

        if (elapsed >= SYNC_INTERVAL_MS) {
            lastSync = now;
            syncCount++;
            Serial.printf("[EinkSync #%d] Starting at %lu ms\n", syncCount, now);

            if (!NetworkController::connected()) {
                Serial.println("[EinkSync] Network not connected");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }
            Serial.println("[EinkSync] Network connected");

            String baseUrl = NetworkController::getApiBaseUrl();
            if (baseUrl.length() == 0) {
                Serial.println("[EinkSync] No API base URL");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }
            Serial.printf("[EinkSync] Base URL: %s\n", baseUrl.c_str());

            int httpCode = -1;
            const String endpoint = NetworkController::buildUrl("api/v1/eink-display/text");
            Serial.printf("[EinkSync] Fetching from: %s\n", endpoint.c_str());
            
            String text = NetworkController::fetch(endpoint, httpCode);
            Serial.printf("[EinkSync] HTTP %d, response len=%u\n", httpCode, text.length());

            if (httpCode != 200) {
                Serial.printf("[EinkSync] Failed: HTTP=%d\n", httpCode);
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            text.trim();
            Serial.printf("[EinkSync] Trimmed text: '%s'\n", text.c_str());
            Serial.printf("[EinkSync] Current: '%s'\n", self->_currentText.c_str());

            if (text.length() == 0) {
                Serial.println("[EinkSync] Response text is empty");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            if (text == self->_currentText) {
                Serial.println("[EinkSync] Text unchanged, skipping display update");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            Serial.println("[EinkSync] Updating display...");
            self->_currentText = text;
            self->showText(self->_currentText);
            Serial.printf("[EinkSync] Display updated: %s\n", self->_currentText.c_str());
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
