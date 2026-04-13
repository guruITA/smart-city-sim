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
        // will be added soon
    }

    namespace TrainPredictionSignal {
        // will be added soon
    }
}

#endif