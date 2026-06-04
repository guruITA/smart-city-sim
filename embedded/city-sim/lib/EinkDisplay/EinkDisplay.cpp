#include "EinkDisplay.h"
#include <SPI.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "NetworkController.h"
#include "OverrideController.h"
using Display = GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT>;

static SPIClass _spi(HSPI);
static Display* _disp = nullptr;

// Marker stored in _currentPayload while the emergency alert is on screen, so
// the slow e-ink refreshes once on the transition instead of every sync cycle.
static const char* const EINK_OVERRIDE_SENTINEL = "__override_alert__";

EinkDisplay::EinkDisplay(int clkPin, int mosiPin, int csPin, int dcPin, int rstPin, int busyPin)
    : _clkPin(clkPin), _mosiPin(mosiPin), _csPin(csPin), _dcPin(dcPin), _rstPin(rstPin), _busyPin(busyPin), _currentPayload("") {}

static uint16_t resolveColor(const String& colorName) {
    String normalized = colorName;
    normalized.toLowerCase();

    if (normalized == "red") {
        return GxEPD_RED;
    }

    if (normalized == "white") {
        return GxEPD_WHITE;
    }

    return GxEPD_BLACK;
}

static String trimCopy(String value) {
    value.trim();
    return value;
}

static bool parseDisplayPayload(const String& payload, String& text, String& textColor,
                                bool& imageEnabled, String& imageColor, int& imageWidth,
                                int& imageHeight, String& imageRows) {
    text = "";
    textColor = "black";
    imageEnabled = false;
    imageColor = "red";
    imageWidth = 0;
    imageHeight = 0;
    imageRows = "";

    int start = 0;
    while (start < payload.length()) {
        int end = payload.indexOf('\n', start);
        if (end < 0) {
            end = payload.length();
        }

        String line = payload.substring(start, end);
        line.trim();

        if (line.startsWith("TEXT=")) {
            text = line.substring(5);
        } else if (line.startsWith("TEXT_COLOR=")) {
            textColor = line.substring(11);
        } else if (line.startsWith("IMAGE_ENABLED=")) {
            imageEnabled = line.substring(14).toInt() != 0;
        } else if (line.startsWith("IMAGE_COLOR=")) {
            imageColor = line.substring(12);
        } else if (line.startsWith("IMAGE_WIDTH=")) {
            imageWidth = line.substring(12).toInt();
        } else if (line.startsWith("IMAGE_HEIGHT=")) {
            imageHeight = line.substring(13).toInt();
        } else if (line.startsWith("IMAGE_ROWS=")) {
            imageRows = line.substring(11);
        }

        start = end + 1;
    }

    text = trimCopy(text);
    textColor = trimCopy(textColor);
    imageColor = trimCopy(imageColor);
    imageRows = trimCopy(imageRows);

    return text.length() > 0;
}

static void drawBitmapRows(Display* disp, int originX, int originY, int width, int height,
                           const String& rows, uint16_t color) {
    if (!disp || width <= 0 || height <= 0 || rows.length() == 0) {
        return;
    }

    int rowStart = 0;
    int y = 0;
    while (rowStart < rows.length() && y < height) {
        int rowEnd = rows.indexOf(';', rowStart);
        if (rowEnd < 0) {
            rowEnd = rows.length();
        }

        String row = rows.substring(rowStart, rowEnd);
        row.trim();

        for (int x = 0; x < width && x < row.length(); ++x) {
            if (row.charAt(x) == '1') {
                disp->drawPixel(originX + x, originY + y, color);
            }
        }

        rowStart = rowEnd + 1;
        ++y;
    }
}

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
    showState(text, "black", false, "red", 0, 0, "");
}

void EinkDisplay::showState(const String& text, const String& textColor, bool imageEnabled,
                            const String& imageColor, int imageWidth, int imageHeight,
                            const String& imageRows) {
    if (!_disp) return;
    _disp->setRotation(3);
    _disp->setFont(&FreeMonoBold9pt7b);
    _disp->setTextColor(resolveColor(textColor));
    _disp->setFullWindow();
    _disp->firstPage();
    do {
        _disp->fillScreen(GxEPD_WHITE);
        if (imageEnabled && imageWidth > 0 && imageHeight > 0 && imageRows.length() > 0) {
            drawBitmapRows(_disp, 10, 78, imageWidth, imageHeight, imageRows, resolveColor(imageColor));
        }

        _disp->setCursor(10, imageEnabled ? 44 : 64);
        _disp->print(text);
    } while (_disp->nextPage());

    _currentText = text;
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

        // Backend override: show an emergency alert and pause the normal payload
        // sync while it is active. Rendered once on the transition; when the
        // override clears, the next fetched payload differs from the sentinel and
        // the normal screen is drawn again.
        if (OverrideController::isCommand("eink", "show_alert")) {
            if (self->_currentPayload != EINK_OVERRIDE_SENTINEL) {
                Serial.println("[EinkSync] Override active -> EMERGENCY alert");
                self->showState("EMERGENCY", "red", false, "red", 0, 0, "");
                self->_currentPayload = EINK_OVERRIDE_SENTINEL;
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            continue;
        }

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
            const String endpoint = NetworkController::buildUrl("api/v1/eink-display/payload");
            Serial.printf("[EinkSync] Fetching from: %s\n", endpoint.c_str());
            
            String payload = NetworkController::fetch(endpoint, httpCode);
            Serial.printf("[EinkSync] HTTP %d, response len=%u\n", httpCode, payload.length());

            if (httpCode != 200) {
                Serial.printf("[EinkSync] Failed: HTTP=%d\n", httpCode);
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            payload.trim();
            Serial.printf("[EinkSync] Current payload len=%u\n", self->_currentPayload.length());

            if (payload.length() == 0) {
                Serial.println("[EinkSync] Response payload is empty");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            if (payload == self->_currentPayload) {
                Serial.println("[EinkSync] Payload unchanged, skipping display update");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            String text;
            String textColor;
            bool imageEnabled;
            String imageColor;
            int imageWidth;
            int imageHeight;
            String imageRows;

            if (!parseDisplayPayload(payload, text, textColor, imageEnabled, imageColor,
                                     imageWidth, imageHeight, imageRows)) {
                Serial.println("[EinkSync] Failed to parse display payload");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }

            Serial.println("[EinkSync] Updating display...");
            self->_currentPayload = payload;
            self->showState(text, textColor, imageEnabled, imageColor, imageWidth, imageHeight, imageRows);
            Serial.printf("[EinkSync] Display updated: %s\n", self->_currentText.c_str());
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
