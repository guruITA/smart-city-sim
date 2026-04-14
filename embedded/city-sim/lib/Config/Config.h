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

        constexpr const char* CAMERA_CAPTURE_URL = "http://192.168.4.1/capture";
    }

    namespace TrainPredictionSignal {
        // will be added soon
    }
}

#endif