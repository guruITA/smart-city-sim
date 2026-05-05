#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/** @brief Configuration values for the application. */
namespace Config {

    /** @brief Configuration values for the network. */
    namespace Network {

        /** @brief WiFi network credentials. */
        constexpr const char* WIFI_SSID = "";
        constexpr const char* WIFI_PASSWORD = "";

        /** @brief Base URL for the API endpoint. */
        constexpr const char* API_BASE_URL = "";
    }

    namespace Streetlight {

        /** @brief GPIO pin connected to the analog output of the LDR sensor. */
        constexpr uint8_t LDR_PIN = 4;

        /** @brief GPIO pin connected to the relay control signal. */
        constexpr uint8_t RELAY_PIN = 5;

        /** @brief Light threshold for streetlight activation. */
        constexpr int THRESHOLD = 650;

        /** @brief Interval for updating streetlight status. */
        constexpr unsigned long INTERVAL_MS = 1000;
    }

    namespace SpeedCamera {
        constexpr uint8_t IR1_PIN = 6;
        constexpr uint8_t IR2_PIN = 12;

        constexpr uint8_t OLED_SDA_PIN = 17;
        constexpr uint8_t OLED_SCL_PIN = 46;

        constexpr int SCREEN_WIDTH = 128;
        constexpr int SCREEN_HEIGHT = 64;
        constexpr uint8_t OLED_ADDRESS = 0x3C;

        constexpr int IR_ACTIVE_STATE = LOW;

        constexpr float SENSOR_DISTANCE_M = 0.10f;
        constexpr float SPEED_LIMIT_KMH = 1.0f;

        constexpr unsigned long PASS_TIMEOUT_US = 2000000UL;
        constexpr unsigned long MEASUREMENT_COOLDOWN_MS = 500;
        constexpr unsigned long RESULT_SCREEN_HOLD_MS = 2000;

        constexpr unsigned long BOOT_SCREEN_HOLD_MS = 1500;
        constexpr unsigned long UI_REFRESH_INTERVAL_MS = 150;

        constexpr const char* CAMERA_WIFI_SSID = "ESP32CAM_CAPTURE";
        constexpr const char* CAMERA_WIFI_PASSWORD = "12345678";

        constexpr const char* CAMERA_CAPTURE_URL = "http://192.168.4.1/capture";

        constexpr unsigned long CAMERA_WIFI_CONNECT_TIMEOUT_MS = 8000;
        }

    namespace Parking {
        constexpr uint8_t OLED_SDA_PIN = 8;
        constexpr uint8_t OLED_SCL_PIN = 9;

        constexpr uint8_t TRIG_PIN = 13;
        constexpr uint8_t ECHO1_PIN = 14;
        constexpr uint8_t ECHO2_PIN = 15;
        constexpr uint8_t ECHO3_PIN = 3;
        constexpr uint8_t ECHO4_PIN = 10;

        constexpr int SCREEN_WIDTH = 128;
        constexpr int SCREEN_HEIGHT = 64;
        constexpr uint8_t OLED_ADDRESS = 0x3C;

        constexpr float SOUND_SPEED = 0.0343f;
        constexpr float PARKED_THRESHOLD_ON_CM = 5.0f;
        constexpr float PARKED_THRESHOLD_OFF_CM = 10.0f;
        constexpr float INVALID_DISTANCE_CM = -1.0f;
        constexpr float ECHO_TRAVEL_DIVIDER = 2.0f;

        constexpr unsigned long ECHO_TIMEOUT_MICROSECONDS = 100000UL;
        constexpr unsigned long UI_REFRESH_INTERVAL_MS = 250UL;
        constexpr unsigned long SENSOR_MEASURE_INTERVAL_MS = 10UL;
    }

    namespace TrainPredictionSignal {
        constexpr int LED1_PIN = 37;
        constexpr int LED2_PIN = 36;
        constexpr int BUZZER_PIN = 42;
        constexpr int SERVO_PIN = 18;

        constexpr int BTN_PIN = 45;

        constexpr int AB_DISTANCE = 200;
        constexpr int BC_DISTANCE = 1000;

        constexpr unsigned int SAFETY_MARGIN = 5000;

        constexpr int BUZZER_FREQ = 1000;
        constexpr int BUZZER_RESOLUTION = 8;

        constexpr int BARRIER_OPEN_ANGLE = 0;
        constexpr int BARRIER_CLOSED_ANGLE = 90;
    }

    namespace EinkDisplay {
        constexpr int CLK_PIN  = 13;
        constexpr int MOSI_PIN = 11;
        constexpr int CS_PIN   = 10;
        constexpr int DC_PIN   = 9;
        constexpr int RST_PIN  = 8;
        constexpr int BUSY_PIN = 7;
    }
}

#endif