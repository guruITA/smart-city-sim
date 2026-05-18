# Realise Document

## Table of Contents

1. [Goal of This Realisation](#1-goal-of-this-realisation)  
2. [Where the Code Was Realised](#2-where-the-code-was-realised)  
3. [City-Sim Integration Realisation](#3-city-sim-integration-realisation)  
4. [Display Behaviour Realisation](#4-display-behaviour-realisation)  
	4.1 [Payload format](#41-payload-format)  
	4.2 [Image rendering](#42-image-rendering)  
	4.3 [Update logic](#43-update-logic)  
5. [Backend Communication Realisation](#5-backend-communication-realisation)  
6. [Reusability and Mergeability Outcome](#6-reusability-and-mergeability-outcome)  
7. [Reflection and Next Improvements](#7-reflection-and-next-improvements)  

## 1. Goal of This Realisation

Turn the Sprint 3 design for an e-ink tile into working code inside the shared `embedded/city-sim` firmware. The focus is on:

- Driving a 2.9" e-ink display from an ESP32
- Retrieving display content from a backend API and updating the screen only when needed
- Providing a compact payload format that supports a text label and a small monochrome/logo image

---

## 2. Where the Code Was Realised

Main integration entry point:  
`embedded/city-sim/city-sim.ino`  

Display library files:  
`embedded/city-sim/lib/EinkDisplay/EinkDisplay.h`  
`embedded/city-sim/lib/EinkDisplay/EinkDisplay.cpp`  

Configuration:  
`embedded/city-sim/lib/Config/Config.h`  

Fritzing wiring diagram (asset):  
`docs/Thijmen/Sprint 3/assets/E-ink-screen-fritzing.png`

---

## 3. City-Sim Integration Realisation

In `city-sim.ino` the `EinkDisplay` is instantiated with the pin mappings from `Config.h` and started alongside other tiles:

```cpp
EinkDisplay eink(
	Config::EinkDisplay::CLK_PIN,
	Config::EinkDisplay::MOSI_PIN,
	Config::EinkDisplay::CS_PIN,
	Config::EinkDisplay::DC_PIN,
	Config::EinkDisplay::RST_PIN,
	Config::EinkDisplay::BUSY_PIN
);

void setup() {
	// ... network, other tiles
	eink.begin();
	eink.startSyncTask();
}

void loop() {
	lamp.update();
	trainSignal.update();
	speedCamera.update();
	parking.update();
}
```

This realisation integrates the e-ink tile as a reusable class-based component that exposes `begin()` and `startSyncTask()` and updates are handled in a background FreeRTOS task.

---

## 4. Display Behaviour Realisation

The implementation focuses on minimal, reliable updates to respect e-ink constraints: low power, slow refresh and ghosting avoidance.

### 4.1 Payload format

The backend returns a small plain-text payload with line-delimited key/value pairs. Required/recognized fields:

- `TEXT=<display text>`
- `TEXT_COLOR=<black|red|white>`
- `IMAGE_ENABLED=<0|1>`
- `IMAGE_COLOR=<black|red|white>`
- `IMAGE_WIDTH=<width in pixels>`
- `IMAGE_HEIGHT=<height in pixels>`
- `IMAGE_ROWS=<row1;row2;row3;...>`

Example minimal payload:

```
TEXT=The Embedded alliance
TEXT_COLOR=black
IMAGE_ENABLED=0
```

### 4.2 Image rendering

Images/logos are provided as a small monochrome bitmap encoded as semicolon-separated rows. Each row contains characters where `'1'` means draw pixel and any other char is transparent. The firmware parses `IMAGE_ROWS` and draws pixels with `drawPixel`.

Snippet (bitmap drawing helper):

```cpp
static void drawBitmapRows(Display* disp, int originX, int originY, int width, int height,
													 const String& rows, uint16_t color) {
		if (!disp || width <= 0 || height <= 0 || rows.length() == 0) return;
		int rowStart = 0; int y = 0;
		while (rowStart < rows.length() && y < height) {
				int rowEnd = rows.indexOf(';', rowStart);
				if (rowEnd < 0) rowEnd = rows.length();
				String row = rows.substring(rowStart, rowEnd);
				for (int x = 0; x < width && x < row.length(); ++x) {
						if (row.charAt(x) == '1') disp->drawPixel(originX + x, originY + y, color);
				}
				rowStart = rowEnd + 1;
				++y;
		}
}
```

### 4.3 Update logic

- Poll interval: 10 seconds (FreeRTOS task)  
- The sync task fetches the payload, trims it and compares with `_currentPayload`.  
- If payload unchanged -> skip refresh. If changed and parse succeeds -> call `showState(...)` to redraw.

Key snippet (constructor + begin):

```cpp
EinkDisplay::EinkDisplay(int clkPin, int mosiPin, int csPin, int dcPin, int rstPin, int busyPin)
		: _clkPin(clkPin), _mosiPin(mosiPin), _csPin(csPin), _dcPin(dcPin), _rstPin(rstPin), _busyPin(busyPin), _currentPayload("") {}

void EinkDisplay::begin() {
		_spi.begin(_clkPin, /*miso=*/-1, _mosiPin, /*ss=*/-1);
		_disp = new Display(GxEPD2_290_C90c(_csPin, _dcPin, _rstPin, _busyPin));
		_disp->epd2.selectSPI(_spi, SPISettings(1000000, MSBFIRST, SPI_MODE0));
		_disp->init(115200, true, 10, false);
		const String initText = "The Embedded alliance";
		_currentText = initText;
		showText(initText);
}
```

---

## 5. Backend Communication Realisation

The sync task builds the endpoint `api/v1/eink-display/payload` using `NetworkController::buildUrl(...)` and uses `NetworkController::fetch(endpoint, httpCode)` to obtain the payload. The task logs HTTP status and behaviour to serial.

Behavioral points:

- If network not connected or base URL missing: task delays and retries.  
- If HTTP code != 200 or payload empty: skip and retry.  
- On success: parse payload, update `_currentPayload`, and refresh display.

The payload parsing routine extracts text and optional image metadata and returns success only if `TEXT` is present.

---

## 6. Reusability and Mergeability Outcome

What was realised:

- The e-ink tile is integrated as a reusable class `EinkDisplay` with clear initialization (`begin`) and a background sync task (`startSyncTask`).
- The same firmware can be deployed to multiple tiles; content is delivered by the backend so no reflashing is required for content changes.
- The image format is compact and backend-preparable, reducing ESP32 processing needs.

Files involved:  
`embedded/city-sim/lib/EinkDisplay/EinkDisplay.h`  
`embedded/city-sim/lib/EinkDisplay/EinkDisplay.cpp`  
`embedded/city-sim/lib/Config/Config.h`  
`embedded/city-sim/city-sim.ino`  

---

## 7. Reflection and Next Improvements

Successful outcomes:

- Reliable display updates when content changes.  
- Low update frequency to respect e-ink characteristics.  
- Simple, backend-driven payload format for text and small images.

Improvements to consider:

- Add retry/backoff and exponential fallback for network errors.  
- Add unit tests for payload parsing and drawing helper logic.