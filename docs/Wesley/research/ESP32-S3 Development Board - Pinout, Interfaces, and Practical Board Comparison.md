# ESP32-S3 Development Board: Pinout, Interfaces, and Practical Board Comparison

## Table of Contents
- [Overview](#overview)
- [1. Board overview and main components](#1-board-overview-and-main-components)
- [2. Pin layout and functional pin groups](#2-pin-layout-and-functional-pin-groups)
- [3. Communication and control interfaces](#3-communication-and-control-interfaces)
- [4. Board used in class](#4-board-used-in-class)
- [5. Conclusion](#5-conclusion)
- [Appendix A. Official pin-function tables](#appendix-a-official-pin-function-tables)
- [References](#references)

## Overview

In this document, I explain the ESP32-S3 development board and the most important pins and interfaces that can be used during practical work. I first use the official ESP32-S3-DevKitC-1 as a reference board, because Espressif provides the clearest documentation for it. After that, I discuss the most important pin groups, such as GPIO, touch pins, ADC pins, RTC GPIO, PWM, SPI, UART, interrupts, strapping pins, and the EN/RST pin. In the last part, I also look at the board that is used in class, because it appears to be a YD-ESP32-S3 board and that board does not fully match the official DevKitC-1 layout.

The goal of this document is to give a clear overview of the board, while also showing which information is useful for wiring, programming, and understanding the available hardware functions.

## 1. Board overview and main components

The ESP32-S3-DevKitC-1 is a development board based on the ESP32-S3 module. It is designed for general embedded development and supports Wi-Fi and Bluetooth Low Energy. Most of the available I/O pins are broken out to the side headers, which makes the board easy to use on a breadboard or with jumper wires (_ESP32-S3-DEVKITC-1 v1.1_, n.d.).

![Figure 1. Main components of the ESP32-S3-DevKitC-1](../assets/ESP32-S3%20Development%20Board:%20Pinout,%20Interfaces,%20and%20Practical%20Board%20Comparison/figure01_components.png)

*Figure 1. Main components of the ESP32-S3-DevKitC-1. Source: _ESP32-S3-DEVKITC-1 v1.1_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html

### Main components

| Component             | Description                                                                                                                       |
| --------------------- | --------------------------------------------------------------------------------------------------------------------------------- |
| ESP32-S3-WROOM module | The main microcontroller module on the board. This module contains the ESP32-S3 chip and supports Wi-Fi and Bluetooth Low Energy. |
| Pin headers           | These give access to the GPIO pins so external components can be connected.                                                       |
| USB-to-UART port      | This port is used for power, flashing the board, and serial communication through the USB-to-UART bridge.                         |
| USB port              | This is the native USB connection of the ESP32-S3. It can be used for flashing, debugging, and USB communication.                 |
| BOOT button           | This button is used to put the board into download mode when needed.                                                              |
| RESET button          | This button restarts the board.                                                                                                   |
| RGB LED and power LED | The RGB LED can be controlled in software, while the power LED shows that the board is powered.                                   |

A practical point is that the USB-to-UART port and the native USB port are not exactly the same. For serial logging and normal flashing, the USB-to-UART connection is usually the easiest option. The native USB connection is also useful, but it depends more on the firmware configuration and the selected development setup (_Establish Serial Connection With ESP32-S3_, n.d.; _USB Serial/JTAG Controller Console_, n.d.; _USB OTG Console_, n.d.).

## 2. Pin layout and functional pin groups

The pinout diagram is useful as a quick overview of the board. It shows which pins can be used for digital input and output, which pins support analog functions, and which pins should be handled carefully because they are connected to internal memory or boot configuration.

![Figure 2. ESP32-S3 DevKitC pinout overview](../assets/ESP32-S3%20Development%20Board:%20Pinout,%20Interfaces,%20and%20Practical%20Board%20Comparison/figure02_official_pinout.png)

*Figure 2. ESP32-S3 DevKitC pinout overview. Source: _ESP32-S3-DEVKITC-1 v1.1_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html

### Functional pin groups

| Group | Pins / range | Main purpose | Notes |
|---|---|---|---|
| General GPIO | Most exposed pins | Digital input and output | Many peripheral signals can be assigned flexibly through the GPIO matrix. |
| Flash / PSRAM | GPIO26-GPIO32 | Internal memory connection | These pins are reserved for onboard flash or PSRAM and should not be treated as normal user pins. |
| Touch | GPIO1-GPIO14 | Capacitive touch input | Useful for touch buttons or touch-based input. |
| ADC | GPIO1-GPIO20 | Analog input | These pins can measure analog voltages, but the voltage range depends on the configuration. |
| RTC GPIO | GPIO0-GPIO21 | Low-power and wake-up functions | Important when deep sleep or wake-up functions are used. |
| Strapping pins | GPIO0, GPIO3, GPIO45, GPIO46 | Boot configuration | These pins should be used carefully because their state during reset can affect startup behaviour. |
(Santos, 2025)

### 2.1 GPIO

GPIO stands for General Purpose Input/Output. These are the pins that are mainly used when connecting LEDs, buttons, sensors, relays, or communication lines. One of the advantages of the ESP32-S3 is that many functions can be routed through the GPIO matrix, so several interfaces are not fixed to only one physical pin (_GPIO & RTC GPIO_, n.d.).

### 2.2 Flash and PSRAM pins

Some pins are connected to the flash memory and PSRAM of the board. On the ESP32-S3, GPIO26 to GPIO32 are commonly associated with this memory connection. These pins should not be used as normal I/O pins on the official development board, because they are needed for the board to work correctly (_ESP32-S3-DEVKITC-1 v1.1_, n.d.; SPI Master Driver_, n.d.).

### 2.3 Capacitive touch pins

GPIO1 to GPIO14 can be used for capacitive touch. These pins can detect changes in capacitance, which makes them useful for touch buttons, sliders, and other touch-based input systems. This can be helpful when creating a simple human interface without using mechanical switches (_Capacitive Touch Sensor_, n.d.).

### 2.4 ADC pins

The ESP32-S3 also has analog-to-digital conversion. GPIO1 to GPIO20 can be used as ADC pins. The raw reading is 12-bit, which means the value can range from 0 to 4095. However, it is important to note that the measured voltage range depends on the attenuation setting and calibration, so it is not always correct to say that the range is simply 0 V to 3.3 V in every situation (_Analog to Digital Converter (ADC)_, n.d.).

### 2.5 RTC GPIO

RTC GPIO pins are linked to the real-time clock subsystem. These pins are important in low-power applications, especially when the board enters deep sleep and needs to wake up again because of a pin signal or another event. This makes them useful in battery-powered systems or projects that need power saving (_GPIO & RTC GPIO_, n.d.).

## 3. Communication and control interfaces

Besides normal GPIO use, the ESP32-S3 also supports several communication and control interfaces. These are important in embedded systems because they make it possible to connect displays, sensors, external modules, and serial tools.

| Feature | Description | Practical use |
|---|---|---|
| PWM (LEDC) | Generates a pulse-width modulated signal. | Useful for LED dimming, buzzers, and motor control. |
| SPI | High-speed serial communication bus. | Often used for displays, sensors, and memory devices. |
| UART | Serial communication interface. | Used for debugging, serial monitors, and communication with other devices. |
| Interrupts | Reacts to events on pins. | Useful for buttons, sensors, and event-driven programming. |
| Native USB | USB communication directly through the ESP32-S3. | Can be used for flashing, debugging, and USB-related functions. |
| EN / RST | Reset and enable function of the board. | Used to restart the board or control startup behaviour. |
(Santos, 2025)
### 3.1 PWM

PWM stands for Pulse Width Modulation. On the ESP32-S3, PWM is usually handled through the LEDC peripheral. With PWM, the board switches a signal on and off very quickly. By changing the duty cycle, the average power of the signal changes. This is useful for dimming LEDs, controlling motors, or generating simple tones with a buzzer (_LED Control (LEDC)_, n.d.).

### 3.2 SPI

SPI is a serial communication protocol that is often used when connecting external hardware such as screens, sensors, or memory modules. On the ESP32-S3, SPI0 and SPI1 are connected to flash and PSRAM, which means they are not the buses that are normally used for custom hardware connections. For external communication, SPI2 and SPI3 are the general-purpose SPI controllers (_SPI Master Driver_, n.d.).

### 3.3 UART

UART is one of the most common ways to communicate with the board. It is used for the serial monitor, debugging messages, and communication with other serial devices. The ESP32-S3 has three UART controllers. GPIO43 and GPIO44 are often used as default console pins, but these signals can be reassigned in many cases because the ESP32-S3 supports flexible signal routing (_GPIO & RTC GPIO_, n.d.; _Establish Serial Connection With ESP32-S3_, n.d.).

### 3.4 Interrupts, strapping pins, and EN/RST

GPIO interrupts make it possible for the program to respond immediately to events such as a button press or a signal change from a sensor. Strapping pins need extra attention, because their logic level during startup influences the boot configuration of the chip. The EN/RST pin is different from a normal GPIO pin, because it is related to enabling and resetting the board (ESP32-S3-DEVKITC-1 v1.1_, n.d.; Santos, 2025).

## 4. Board used in class

The board used during class appears to be different from the official ESP32-S3-DevKitC-1. Based on the available pinout and board picture, it looks like a YD-ESP32-S3 board. This means that the official Espressif documentation is still very useful for understanding the platform, but the practical wiring should be checked against the actual board that is used in class.

| Topic | Official DevKitC-1 | Board used in class |
|---|---|---|
| Main reference | Espressif documentation | YD-ESP32-S3 pinout and board images |
| Best use | Learning the official functions and hardware layout | Checking the physical labels and wiring on the actual board |
| Risk | May not fully match the board in class | May not match all official tutorials exactly |

![Figure 3. Pinout image for the board used in class](../assets/ESP32-S3%20Development%20Board:%20Pinout,%20Interfaces,%20and%20Practical%20Board%20Comparison/figure04_class_board_pinout.png)

*Figure 3. Pinout image for the board used in class. Source: Rtek. (n.d.). _YD-ESP32-23_. GitHub. https://github.com/rtek1000/YD-ESP32-23

![Figure 4. Board photograph of the YD-ESP32-S3 board](../assets/ESP32-S3%20Development%20Board:%20Pinout,%20Interfaces,%20and%20Practical%20Board%20Comparison/figure05_class_board_photo.png)

*Figure 4. Board photograph of the YD-ESP32-S3 board. Source: Rtek. (n.d.). _YD-ESP32-23_. GitHub. https://github.com/rtek1000/YD-ESP32-23

In practice, this means it is best to use the official documentation to understand how the ESP32-S3 works, but to use the pinout of the class board when connecting components in a real setup.

## 5. Conclusion

The ESP32-S3 is a flexible development board with many functions that are useful in embedded systems. It offers digital I/O, analog input, touch sensing, low-power RTC functions, PWM, SPI, UART, and USB support. Because of this, the board can be used for a wide range of projects, from basic experiments with LEDs and buttons to more advanced systems with sensors, displays, and wireless communication.

A very important point is that the board used in class does not seem to be exactly the same as the official ESP32-S3-DevKitC-1. Because of that, the official documentation is the best source for understanding the chip and its functions, while the pinout of the actual class board is the best source for wiring and practical use.

## Appendix A. Official pin-function tables

The following screenshot shows the official pin-function tables. These tables are useful as a quick reference when checking which functions belong to which pins.

![Figure A1. Official pin-function table screenshots](../assets/appendix_figureA1_pin_tables.png)

*Figure A1. Official pin-function table screenshots. Source: ESP32-S3-DEVKITC-1 v1.1_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html

## Reference
_Analog to Digital Converter (ADC)_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/adc/index.html, Visited March 16, 2026

_Capacitive Touch Sensor_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/cap_touch_sens.html, Visited March 16, 2026

_ESP32-S3-DEVKITC-1 v1.1_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html, Visited February 11, 2026

_Establish serial connection with ESP32-S3_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/establish-serial-connection.html, Visited March 16, 2026

_GPIO & RTC GPIO_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/gpio.html, Visited March 16, 2026

_LED Control (LEDC)_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/ledc.html, Visited March 16, 2026

_Rtek. (n.d.). _YD-ESP32-23_. GitHub. https://github.com/rtek1000/YD-ESP32-23, Visited March 16, 2026

Santos, S. (2025, March 12). _ESP32-S3 DevKITC Pinout Reference Guide: GPIOs explained_. Random Nerd Tutorials. https://randomnerdtutorials.com/esp32-s3-devkitc-pinout-guide/ Visited February 11, 2026

_SPI Master Driver_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/spi_master.html, Visited March 16, 2026

_USB OTG console_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-guides/usb-otg-console.html, Visited March 16, 2026

_USB Serial/JTAG Controller Console_. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-guides/usb-serial-jtag-console.html, Visited March 16, 2026


