#include "esp32cam.h"

#include "esp_camera.h"
#include <WebServer.h>
#include <WiFi.h>

static WebServer server(80);
static camera_fb_t* lastFrame = nullptr;

enum ControllerState { CONTROLLER_STARTING, CONTROLLER_READY, CONTROLLER_CAMERA_ERROR };

enum CaptureState {
  CAPTURE_IDLE,
  CAPTURE_DISCARD_OLD_FRAME,
  CAPTURE_WAIT_PRE_FLASH,
  CAPTURE_FLASH_ON,
  CAPTURE_STORE_NEW_FRAME
};

enum BlinkState { BLINK_OFF, BLINK_ON };

static ControllerState controllerState = CONTROLLER_STARTING;
static CaptureState captureState = CAPTURE_IDLE;
static BlinkState blinkState = BLINK_OFF;

static unsigned long captureStateStartedMs = 0;
static unsigned long blinkStateStartedMs = 0;

static bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA; // 1600x1200
    config.jpeg_quality = 8; // lager = beter, 8 is stabieler dan 6
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size = FRAMESIZE_SVGA; // 800x600 zonder PSRAM
    config.jpeg_quality = 10;
    config.fb_location = CAMERA_FB_IN_DRAM;
    config.fb_count = 1;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  }

  if (esp_camera_init(&config) != ESP_OK) {
    return false;
  }

  sensor_t* s = esp_camera_sensor_get();
  if (s) {
    s->set_hmirror(s, 1);
    s->set_vflip(s, 0);

    s->set_brightness(s, 1);
    s->set_contrast(s, 2);
    s->set_saturation(s, 1);

    s->set_whitebal(s, 1);
    s->set_awb_gain(s, 1);
    s->set_exposure_ctrl(s, 1);
    s->set_gain_ctrl(s, 1);
    s->set_gainceiling(s, GAINCEILING_4X);
  }

  return true;
}

static bool isCaptureBusy() {
  return captureState != CAPTURE_IDLE;
}

static void startPhotoCapture() {
  if (isCaptureBusy()) {
    return;
  }

  captureState = CAPTURE_DISCARD_OLD_FRAME;
  captureStateStartedMs = millis();
}

static void updatePhotoCapture() {
  switch (captureState) {
  case CAPTURE_IDLE:
    break;

  case CAPTURE_DISCARD_OLD_FRAME: {
    camera_fb_t* fb = esp_camera_fb_get();
    if (fb) {
      esp_camera_fb_return(fb);
      fb = nullptr;
    }

    captureState = CAPTURE_WAIT_PRE_FLASH;
    captureStateStartedMs = millis();
    break;
  }

  case CAPTURE_WAIT_PRE_FLASH:
    if (millis() - captureStateStartedMs >= PRE_FLASH_WAIT_MS) {
      digitalWrite(FLASH_LED_PIN, HIGH);
      captureState = CAPTURE_FLASH_ON;
      captureStateStartedMs = millis();
    }
    break;

  case CAPTURE_FLASH_ON:
    if (millis() - captureStateStartedMs >= FLASH_PULSE_MS) {
      captureState = CAPTURE_STORE_NEW_FRAME;
      captureStateStartedMs = millis();
    }
    break;

  case CAPTURE_STORE_NEW_FRAME: {
    camera_fb_t* fb = esp_camera_fb_get();

    digitalWrite(FLASH_LED_PIN, LOW);

    if (!fb) {
      captureState = CAPTURE_IDLE;
      return;
    }

    if (lastFrame != nullptr) {
      esp_camera_fb_return(lastFrame);
      lastFrame = nullptr;
    }

    lastFrame = fb;
    captureState = CAPTURE_IDLE;
    break;
  }
  }
}

static bool capturePhotoBlockingUntilDone() {
  if (isCaptureBusy()) {
    return false;
  }

  startPhotoCapture();

  unsigned long startedMs = millis();
  while (isCaptureBusy()) {
    updatePhotoCapture();

    if (millis() - startedMs >= CAPTURE_TIMEOUT_MS) {
      digitalWrite(FLASH_LED_PIN, LOW);
      captureState = CAPTURE_IDLE;
      return false;
    }

    yield();
  }

  return lastFrame != nullptr;
}

static void handleRoot() {
  String html;
  html += "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32-CAM Capture Test</title></head><body>";
  html += "<h2>ESP32-CAM Capture Test</h2>";
  html += "<button onclick=\"fetch('/capture').then(() => { ";
  html += "document.getElementById('foto').src='/photo?t=' + Date.now();";
  html += "});\">Maak foto</button>";
  html += "<br><br>";
  html += "<img id='foto' src='/photo?t=" + String(millis()) +
          "' style='max-width:100%;height:auto;border:1px solid #ccc;'>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

static void handlePhoto() {
  if (lastFrame == nullptr) {
    server.send(404, "text/plain", "No photos found yet");
    return;
  }

  WiFiClient client = server.client();
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: image/jpeg\r\n");
  client.print("Content-Length: ");
  client.print(lastFrame->len);
  client.print("\r\n");
  client.print("Cache-Control: no-cache\r\n");
  client.print("Connection: close\r\n");
  client.print("\r\n");

  client.write(lastFrame->buf, lastFrame->len);
}

static void handleCapture() {
  if (isCaptureBusy()) {
    server.send(409, "text/plain", "Capture already in progress");
    return;
  }

  bool success = capturePhotoBlockingUntilDone();

  if (!success) {
    server.send(500, "text/plain", "Capture failed");
    return;
  }

  server.send(200, "text/plain", "OK");
}

static void beginWebServer() {
  WiFi.mode(WIFI_AP);
  bool apStarted = WiFi.softAP(AP_SSID, AP_PASS);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/photo", HTTP_GET, handlePhoto);
  server.on("/capture", HTTP_GET, handleCapture);
  server.begin();
}

static void updateErrorBlink() {
  unsigned long now = millis();

  switch (blinkState) {
  case BLINK_ON:
    if (now - blinkStateStartedMs >= ERROR_BLINK_INTERVAL_MS) {
      digitalWrite(FLASH_LED_PIN, LOW);
      blinkState = BLINK_OFF;
      blinkStateStartedMs = now;
    }
    break;

  case BLINK_OFF:
    if (now - blinkStateStartedMs >= ERROR_BLINK_INTERVAL_MS) {
      digitalWrite(FLASH_LED_PIN, HIGH);
      blinkState = BLINK_ON;
      blinkStateStartedMs = now;
    }
    break;
  }
}

void begin() {
  Serial.begin(115200);

  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, LOW);

  if (!initCamera()) {
    controllerState = CONTROLLER_CAMERA_ERROR;
    blinkState = BLINK_ON;
    blinkStateStartedMs = millis();
    digitalWrite(FLASH_LED_PIN, HIGH);
    return;
  }

  beginWebServer();

  capturePhotoBlockingUntilDone();

  controllerState = CONTROLLER_READY;
}

void update() {
  if (controllerState == CONTROLLER_CAMERA_ERROR) {
    updateErrorBlink();
    return;
  }

  if (controllerState == CONTROLLER_READY) {
    server.handleClient();
    updatePhotoCapture();
  }
}