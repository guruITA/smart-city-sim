#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

namespace Config {

    /**
     * Streetlight configuration parameters
     */
    namespace Streetlight {
        constexpr uint8_t LDR_PIN = 4;
        constexpr uint8_t RELAY_PIN = 5;

        constexpr int THRESHOLD = 650;
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

        constexpr const char* CAMERA_CAPTURE_URL = "http://192.168.4.1/capture";
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
}

#endif