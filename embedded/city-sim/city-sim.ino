#include "Config.h"
#include "NetworkController.h"
#include "Streetlight.h"
#include "TrainPredictionSignal.h"
#include "SpeedCamera.h"

#define builtin LED_BUILTIN

//  WiFi details
const char* WIFI_SSID = "ESP32CAM_CAPTURE";
const char* WIFI_PASSWORD = "12345678";

// backend URL
const String API_BASE_URL = "";

StreetLight lamp(
  Config::Streetlight::LDR_PIN,
  Config::Streetlight::RELAY_PIN,
  Config::Streetlight::THRESHOLD,
  Config::Streetlight::INTERVAL_MS
);

SpeedCamera speedCamera(Config::SpeedCamera::IR1_PIN, Config::SpeedCamera::IR2_PIN,
                        Config::SpeedCamera::OLED_SDA_PIN, Config::SpeedCamera::OLED_SCL_PIN,
                        Config::SpeedCamera::SCREEN_WIDTH, Config::SpeedCamera::SCREEN_HEIGHT,
                        Config::SpeedCamera::OLED_ADDRESS, Config::SpeedCamera::IR_ACTIVE_STATE,
                        Config::SpeedCamera::SENSOR_DISTANCE_M,
                        Config::SpeedCamera::SPEED_LIMIT_KMH, Config::SpeedCamera::PASS_TIMEOUT_US,
                        Config::SpeedCamera::MEASUREMENT_COOLDOWN_MS,
                        Config::SpeedCamera::RESULT_SCREEN_HOLD_MS,
                        Config::SpeedCamera::BOOT_SCREEN_HOLD_MS,
                        Config::SpeedCamera::UI_REFRESH_INTERVAL_MS,
                        Config::SpeedCamera::CAMERA_CAPTURE_URL);

// Railroad crossing tile
TrainPredictionSignal trainSignal(Config::TrainPredictionSignal::LED1_PIN, 
                                  Config::TrainPredictionSignal::LED2_PIN, 
                                  Config::TrainPredictionSignal::BUZZER_PIN, 
                                  Config::TrainPredictionSignal::SERVO_PIN, 
                                  Config::TrainPredictionSignal::BTN_PIN, 
                                  Config::TrainPredictionSignal::AB_DISTANCE, 
                                  Config::TrainPredictionSignal::BC_DISTANCE, 
                                  Config::TrainPredictionSignal::SAFETY_MARGIN, 
                                  Config::TrainPredictionSignal::BUZZER_FREQ, 
                                  Config::TrainPredictionSignal::BUZZER_RESOLUTION, 
                                  Config::TrainPredictionSignal::BARRIER_OPEN_ANGLE, 
                                  Config::TrainPredictionSignal::);

void setup() {
  pinMode(builtin, OUTPUT);
  digitalWrite(builtin, LOW);

  Serial.begin(115200);
  Serial.println("Setup start");

  if (NetworkController::begin(WIFI_SSID, WIFI_PASSWORD)) {
    Serial.println("WiFi connected, network fetch availability up");
  } else {
    Serial.println("WiFi not connected, some network features will be skipped");
  }

  NetworkController::setApiBaseUrl(API_BASE_URL);

  lamp.begin();
  trainSignal.begin();
  speedCamera.begin();
}

void loop() {
  lamp.update();
  trainSignal.update();
  speedCamera.update();
}