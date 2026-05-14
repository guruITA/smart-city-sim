#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#define FLASH_LED_PIN 4

const char* AP_SSID = "ESP32CAM_CAPTURE";
const char* AP_PASS = "12345678";

camera_fb_t* lastFrame = nullptr;

WebServer server(80);

unsigned long lastCaptureMs = 0;

bool initCamera() {
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

  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 15;
  config.fb_location = CAMERA_FB_IN_PSRAM;

  if (psramFound()) {
    config.fb_count = 2;
    config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
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
    s->set_brightness(s, 0);
    s->set_contrast(s, 1);
    s->set_saturation(s, 0);
  }

  return true;
}

void captureAndStorePhoto() {
  camera_fb_t* fb = esp_camera_fb_get();
  if (fb) {
    esp_camera_fb_return(fb);
    fb = nullptr;
  }

  delay(5);

  digitalWrite(FLASH_LED_PIN, HIGH);
  delay(10);

  fb = esp_camera_fb_get();

  digitalWrite(FLASH_LED_PIN, LOW);

  if (!fb) {
    return;
  }

  if (lastFrame != nullptr) {
    esp_camera_fb_return(lastFrame);
    lastFrame = nullptr;
  }

  lastFrame = fb;
}

void captureTriggeredPhoto() {
  captureAndStorePhoto();
}

void handleRoot() {
  String html;
  html += "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32-CAM Capture Test</title></head><body>";
  html += "<h2>ESP32-CAM Capture Test</h2>";
  html += "<button onclick=\"fetch('/capture').then(() => { "
          "document.getElementById('foto').src='/photo?t=' + Date.now(); });\">Maak foto</button>";
  html += "<br><br>";
  html += "<img id='foto' src='/photo?t=" + String(millis()) +
          "' style='max-width:100%;height:auto;border:1px solid #ccc;'>";
  html += "<script>";
  html += "setInterval(function(){";
  html += "document.getElementById('foto').src='/photo?t=' + Date.now();";
  html += "}, 500);";
  html += "</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handlePhoto() {
  if (lastFrame == nullptr) {
    server.send(404, "text/plain", "Nog geen foto");
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

void handleCapture() {
  captureAndStorePhoto();
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, LOW);

  if (!initCamera()) {
    while (true) {
      digitalWrite(FLASH_LED_PIN, HIGH);
      delay(100);
      digitalWrite(FLASH_LED_PIN, LOW);
      delay(100);
    }
  }

  WiFi.mode(WIFI_AP);
  bool apStarted = WiFi.softAP(AP_SSID, AP_PASS);

  if (!apStarted) {
    Serial.println("SoftAP starten mislukt.");
  } else {
    Serial.println("SoftAP gestart.");
    Serial.print("SSID: ");
    Serial.println(AP_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/photo", HTTP_GET, handlePhoto);
  server.on("/capture", HTTP_GET, handleCapture);
  server.begin();

  captureAndStorePhoto();
}

void loop() {
  server.handleClient();
}