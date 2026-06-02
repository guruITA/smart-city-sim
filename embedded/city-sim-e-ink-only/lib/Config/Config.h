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

    namespace EinkDisplay {
        constexpr int MOSI_PIN = 5;
        constexpr int CLK_PIN  = 4;
        constexpr int CS_PIN   = 6;
        constexpr int DC_PIN   = 9;
        constexpr int RST_PIN  = 8;
        constexpr int BUSY_PIN = 7;
    }
}

#endif