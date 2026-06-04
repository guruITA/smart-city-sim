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
        constexpr const char* API_BASE_URL = "http://145.92.8.137";
    }

    /** @brief Configuration values for the street light. */
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
        constexpr float SENSOR_DISTANCE_M = 0.05f;
        constexpr float SCALE_FACTOR = 64.0f;
        constexpr float SPEED_LIMIT_KMH = 50.0f;
        constexpr unsigned long PASS_TIMEOUT_US = 4000000UL;

        constexpr unsigned long MEASUREMENT_COOLDOWN_MS = 500;
        constexpr unsigned long RESULT_SCREEN_HOLD_MS = 2000;

        constexpr unsigned long BOOT_SCREEN_HOLD_MS = 1500;
        constexpr unsigned long UI_REFRESH_INTERVAL_MS = 150;
    }

    /** @brief Configuration values for the parking system. */
    namespace Parking {

         /** @brief GPIO pin used as SDA for the parking OLED display. */
        constexpr uint8_t OLED_SDA_PIN = 8;

        /** @brief GPIO pin used as SCL for the parking OLED display. */
        constexpr uint8_t OLED_SCL_PIN = 9;

        /** @brief I2C address of the MCP23017 I/O expander. */
        constexpr uint8_t MCP23017_ADDRESS = 0x20;

        /** @brief MCP23017 pin connected to the trigger pin of ultrasonic sensor 1. */
        constexpr uint8_t TRIG1_PIN = 7;

        /** @brief MCP23017 pin connected to the trigger pin of ultrasonic sensor 2. */
        constexpr uint8_t TRIG2_PIN = 6;

        /** @brief MCP23017 pin connected to the trigger pin of ultrasonic sensor 3. */
        constexpr uint8_t TRIG3_PIN = 5;

        /** @brief MCP23017 pin connected to the trigger pin of ultrasonic sensor 4. */
        constexpr uint8_t TRIG4_PIN = 4;

        /** @brief MCP23017 pin connected to the echo pin of ultrasonic sensor 1. */
        constexpr uint8_t ECHO1_PIN = 8;

        /** @brief MCP23017 pin connected to the echo pin of ultrasonic sensor 2. */
        constexpr uint8_t ECHO2_PIN = 9;

        /** @brief MCP23017 pin connected to the echo pin of ultrasonic sensor 3. */
        constexpr uint8_t ECHO3_PIN = 10;

        /** @brief MCP23017 pin connected to the echo pin of ultrasonic sensor 4. */
        constexpr uint8_t ECHO4_PIN = 11;

        /** @brief Width of the OLED display in pixels. */
        constexpr int SCREEN_WIDTH = 128;

        /** @brief Height of the OLED display in pixels. */
        constexpr int SCREEN_HEIGHT = 64;

        /** @brief I2C address of the OLED display. */
        constexpr uint8_t OLED_ADDRESS = 0x3C;

        /** @brief Speed of sound used to calculate distance in centimeters per microsecond. */
        constexpr float SOUND_SPEED = 0.0343f;

        /** @brief Distance threshold where a parking spot becomes occupied. */
        constexpr float PARKED_THRESHOLD_ON_CM = 10.0f;

        /** @brief Distance threshold where a parking spot becomes free again. */
        constexpr float PARKED_THRESHOLD_OFF_CM = 15.0f;

        /** @brief Value used when the measured distance is invalid. */
        constexpr float INVALID_DISTANCE_CM = -1.0f;

        /** @brief Divider used because the ultrasonic signal travels to the object and back. */
        constexpr float ECHO_TRAVEL_DIVIDER = 2.0f;

        /** @brief Maximum time to wait for an echo signal before the measurement is invalid. */
        constexpr unsigned long ECHO_TIMEOUT_MICROSECONDS = 30000UL;

        /** @brief Interval for refreshing the parking OLED display. */
        constexpr unsigned long UI_REFRESH_INTERVAL_MS = 250UL;

        /** @brief Interval for updating the parking sensor measurement steps. */
        constexpr unsigned long SENSOR_MEASURE_INTERVAL_MS = 2UL;
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