# Advice: Smart parking occupancy detection

**Author:** Gurpreet Singh  
**Date:** 12-05-2026  
**Version:** 1.0  
**Classification:** External  
**Client:** Mayor Mats Otten  
**Company:** The Embedded Alliance  

---

## Table of Contents


---

## 1. Introduction

Smart parking is an important part of smart city development. In modern cities, parking availability can influence traffic flow, space usage and the experience of drivers. When drivers do not know where free parking spaces are available, they may spend unnecessary time searching for a space. A comprehensive review of smart parking systems explains that parking allocation has become a major problem in modern cities and that smart parking systems use different technologies, such as sensors, networking, user interfaces and computational methods, to improve parking management [(Fahim et al., 2021)](https://www.sciencedirect.com/science/article/pii/S2405844021011531).

For this reason, smart parking is not only a technical project, but also a traffic management and urban mobility topic. A parking occupancy system can support stakeholders by showing which parking spaces are free or occupied. This can help drivers, parking operators and municipalities make better use of available parking capacity. [(Fahim et al., 2021)](https://www.sciencedirect.com/science/article/pii/S2405844021011531) explain that smart parking systems can be compared based on technological approach, sensor type, networking technology, user interface and services.

This advice document is written for Mayor Mats Otten and other stakeholders in the Smart City project, such as parking garage operators, municipal officials and facility managers. The document advises which technical direction is most suitable for parking occupancy detection in the current project phase. The focus is not only on the current four parking spaces, but also on future expansion toward a larger parking environment.

The current project uses an ESP32-S3 microcontroller as the main controller. The ESP32-S3 is suitable for IoT-based sensor systems because Espressif describes it as a low-power MCU-based system-on-chip with integrated 2.4 GHz Wi-Fi and Bluetooth Low Energy [(Espressif Systems, z.d.)](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf). This is useful for a smart parking system because parking data may later be sent to a dashboard, backend or database.

To make the system more scalable, this document also advises the use of an MCP23017 I/O expander. The MCP23017 provides 16-bit general purpose parallel I/O expansion for I2C applications, which means extra digital input/output pins can be added without using many direct ESP32-S3 GPIO pins [Microchip Technology Inc., 2005](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf).

The purpose of this document is to give clear and defendable advice on the most suitable sensor and controller setup for smart parking occupancy detection. The advised solution should be simple enough for the current prototype, understandable for non-embedded stakeholders and scalable enough to support future growth.

---

## 2. Main advice question and subquestions

The main advice question of this document is:

**Which sensor and controller setup should be advised for parking occupancy detection using an ESP32-S3, while keeping the system suitable for the current prototype and scalable for future expansion?**

To answer this main question, the following subquestions are used:

1. Which sensor technology is most suitable for parking occupancy detection in this project phase?

2. How do ultrasonic, infrared, pressure and camera-based systems compare in terms of cost, complexity, installation impact and suitability?

3. Why is the ESP32-S3 suitable as the main controller for this smart parking system?

4. How can the system be made scalable for more parking spaces in the future?

5. What risks must be considered when using ultrasonic sensors and an MCP23017 I/O expander?

These subquestions help structure the advice and make the reasoning clear for stakeholders.

---

## 3. Methodology

This advice document is based on a literature review of smart parking systems, parking occupancy detection and relevant sensor technologies. The literature was used to compare ultrasonic, infrared, pressure and camera-based detection methods in terms of suitability, complexity, installation impact and scalability.

The findings from the literature were then translated into stakeholder-oriented technical advice for the current Smart City project. The advice phase focuses on choosing a suitable technical direction. Detailed wiring, programming and testing are not discussed in this document, because those subjects belong to the design and realise phases.

---

## 4. Chapter 1: Advice for the most suitable parking occupancy sensor

### 4.1 Introduction

This chapter answers the following subquestion:

**Which sensor technology is most suitable for parking occupancy detection in this project phase?**

The goal is to compare possible sensor technologies and advise the option that best fits the current Smart City project.

---

### 4.2 Sensor options considered

For parking occupancy detection, several sensor types can be considered. This document focuses on ultrasonic sensors, infrared sensors, pressure sensors and camera-based systems. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that parking occupancy sensors can be installed overhead, in the ground or on the ground, and that different active and passive sensor technologies can be used for indoor and outdoor parking occupancy detection.

[(Kumar et al., 2023)](https://www.mdpi.com/2624-6511/6/5/119) also describe parking slot detection as an important part of smart parking systems for smart cities. Their review discusses different parking slot types and detection techniques, which supports the need to compare multiple sensor options before choosing one solution.

#### Ultrasonic sensors

Ultrasonic sensors detect objects by sending sound waves and measuring how long it takes for the reflected wave to return. In parking occupancy detection, this means that the sensor can measure the distance between the sensor and the parking space. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that ultrasonic sensors use sound waves to measure distance and that vehicle presence can be detected by comparing the measured distance with a reference distance for an empty parking space.

The HC-SR04 is a practical ultrasonic sensor example for this project. The HC-SR04 datasheet states that the module provides non-contact measurement from 2 cm to 400 cm, requires a trigger signal of at least 10 microseconds and sends eight ultrasonic pulses at 40 kHz to detect the returned pulse [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

For this project, ultrasonic sensing is suitable because the system only needs to determine whether a parking space is free or occupied. It does not need to identify the vehicle or read a license plate.

#### Infrared sensors

Infrared sensors can also be used for object or distance detection in parking environments. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that infrared ranging sensors can be installed overhead or on the ground, but that they are generally prone to interference by ambient light.

This makes infrared sensors less suitable as the main advised solution for this project phase. In a parking environment, vehicles can differ in colour, material and reflective surface. Lighting and alignment can also influence infrared detection. Therefore, infrared sensors are possible, but they are not the strongest option for a simple and reliable first prototype.

#### Pressure sensors

Pressure sensors, or contact-based sensors, detect the physical presence of a vehicle through force, pressure or weight. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that piezoelectric sensors can be placed under the asphalt surface and can generate electrical charge when pressure is applied.

This makes pressure-based detection useful for direct vehicle detection, but less suitable for this project phase. The main reason is installation impact. If a sensor must be placed in or under the parking surface, the system becomes harder to install, harder to adjust and less practical for a small prototype or demonstration.

#### Camera-based systems

Camera-based systems use images to detect whether parking spaces are occupied. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that cameras can observe parking spaces and that image analysis can be used to detect vehicles and determine parking occupancy. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) also explains that one camera can monitor multiple parking spaces, which can make camera-based systems attractive for larger parking environments.

However, camera-based systems are not a simple plug-and-play solution. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that camera-based detection often requires a model trained on images of the specific parking setup and that performance can be influenced by lighting and weather conditions. For this project phase, the goal is basic occupancy detection, not advanced image analysis. Therefore, camera-based detection is useful as a future option, but not as the advised first solution.

---

### 4.3 Comparison of sensor options

| Sensor type         | Advantages                                                            | Disadvantages                                                                                | Source support                                                                                                                                                                |
| ------------------- | --------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Ultrasonic sensor   | Contactless, distance-based, suitable for basic occupancy detection   | Can be affected by angle, environmental conditions and interference between sensors          | [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf); [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) |
| Infrared sensor     | Can be used for distance or object detection                          | Prone to ambient light interference                                                          | [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf)                      |
| Pressure sensor     | Direct detection through pressure or weight                           | Usually requires installation in or under the surface                                        | [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf)                      |
| Camera-based system | Can monitor multiple parking spaces and support image-based detection | Requires image analysis and can be affected by lighting, weather and setup-specific training | [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf)                      |

Based on this comparison, ultrasonic sensors are the most suitable option for the current project phase. They provide a practical balance between simplicity, low installation impact and suitability for basic occupancy detection.

Pressure sensors and camera-based systems may be useful in other situations, but they add more installation or software complexity than needed for the current prototype. Infrared sensors are possible, but they are less suitable as the main advised solution because they can be affected by ambient light.

---

### 4.4 Advice and justification

The advised sensor type for this project is the **ultrasonic sensor**, with the HC-SR04 as a practical example.

#### Simplicity

Ultrasonic sensors are easier to understand and implement than camera-based systems. They use distance measurement instead of image recognition. The HC-SR04 works with a trigger signal and an echo signal, which makes the working principle clear for a microcontroller-based prototype [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

#### Low installation impact

Ultrasonic sensors can be mounted above or near a parking space. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) describes ultrasonic sensors as sensors that are usually installed overhead. This is useful for the project because the prototype does not need sensors to be placed under the road surface.

#### Suitability for basic occupancy detection

The goal of this system is to detect whether a parking space is free or occupied. It does not need to identify the vehicle, analyse images or read license plates. Ultrasonic sensors are suitable for this because vehicle presence can be detected by measuring distance and comparing it with a reference distance [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf).

#### Better fit than camera-based systems in this phase

Camera-based systems can be useful for larger parking environments because one camera can monitor multiple parking spaces. However, [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that camera-based detection often requires image analysis and may need training data from the specific parking setup. For this project phase, that extra complexity is not necessary.

The advised solution should be simple, reliable and easy to explain to stakeholders. Ultrasonic distance measurement fits this project phase better than image-based detection.

---

### 4.5 Subconclusion

Based on the comparison, ultrasonic sensors are advised as the primary sensor type for this smart parking project. They are simple, contactless and suitable for basic parking occupancy detection.

Camera-based systems may be useful later when the system expands to a larger parking environment and needs image-based detection or analytics. Pressure sensors are less suitable for the current prototype because they often require installation under the surface. Infrared sensors are possible, but they are less suitable as the main advised solution because they can be affected by ambient light.

---

## 5. Chapter 2: Advice for a scalable technical setup

### 5.1 Introduction

This chapter answers the following subquestion:

**How can the system be designed for the current prototype while still allowing future expansion to more parking spaces?**

The current prototype focuses on four parking spaces. However, the technical direction should not be limited to only four spaces. A smart parking system should be designed in a way that can grow later.

---

### 5.2 Recommended system setup

The advised setup is:

- one ESP32-S3 as the main controller;
- one ultrasonic sensor per parking space;
- one MCP23017 I/O expander to support scalable input/output expansion;
- sequential sensor reading to reduce ultrasonic interference;
- threshold logic with hysteresis to determine free or occupied status;
- clear output through a display, dashboard or backend connection.

The ESP32-S3 controls the system and processes the sensor data. The ultrasonic sensors detect the distance to the parking space. The MCP23017 adds extra digital I/O capacity, which makes it easier to expand the system later.

For the current prototype, four ultrasonic sensors are enough. For future expansion, more sensors may be added. If every signal is connected directly to the ESP32-S3, the system can eventually run into GPIO limitations. The MCP23017 helps reduce this limitation because it provides 16-bit general purpose I/O expansion for I2C applications [Microchip Technology Inc., 2005](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf).

---

### 5.3 Why the ESP32-S3 is advised

The ESP32-S3 is suitable as the main controller because it is a low-power MCU-based system-on-chip with integrated 2.4 GHz Wi-Fi and Bluetooth Low Energy [(Espressif Systems, z.d.)](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf). This makes it suitable for an IoT-based smart parking prototype where sensor data may later be sent to a dashboard, database or backend system.

For this smart parking project, the ESP32-S3 is a better fit than a basic microcontroller board without built-in wireless communication, because the ESP32-S3 already includes Wi-Fi and Bluetooth Low Energy [(Espressif Systems, z.d.)](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf). This reduces the need for extra communication hardware in the prototype.

Compared to a Raspberry Pi 4, the ESP32-S3 is more focused on embedded microcontroller use. The Raspberry Pi 4 product brief describes it as a board with a high-performance 64-bit quad-core processor, up to 8 GB of RAM, dual-display support, USB, Ethernet and wireless connectivity [(Raspberry Pi Ltd, 2026)](https://pip-assets.raspberrypi.com/categories/545-raspberry-pi-4-model-b/documents/RP-008344-DS-5-raspberry-pi-4-product-brief.pdf?disposition=inline). That makes Raspberry Pi useful for heavier computing tasks, but this smart parking prototype mainly needs sensor reading, simple logic and wireless communication. For that reason, the ESP32-S3 is a more practical controller choice for this project phase.

The ESP32-S3 therefore gives a good balance between:

- enough processing capability for sensor logic;
- built-in wireless communication;
- lower complexity than a full single-board computer;
- suitability for embedded and IoT prototypes.

---

### 5.4 Why the MCP23017 is advised for scalability

The MCP23017 is advised because it makes the system more scalable. The MCP23017 is a 16-bit I/O expander with an I2C interface [Microchip Technology Inc., 2005](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). This means the ESP32-S3 can use the I2C bus to control additional digital input/output pins.

This is useful because a smart parking system may grow beyond the current four parking spaces. When more sensors, indicators or control signals are added, the number of direct GPIO pins on the ESP32-S3 can become a limitation. By using the MCP23017, the design becomes easier to expand.

The MCP23017 also supports hardware addressing. The MCP23017 datasheet describes three user-defined hardware address bits, A2, A1 and A0 [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). This means multiple MCP23017 devices can be addressed on the same I2C bus, depending on the address configuration. Adafruit also explains that with three address pins, up to eight MCP23017 devices can be used on a single bus, giving a total of 128 GPIO pins ([Clark, 2025](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-mcp23017-i2c-gpio-expander.pdf)).

For this project, the MCP23017 can be used for:

- expanding control lines;
- selecting or enabling sensors;
- controlling indicators;
- supporting future parking space expansion;
- reducing the need to connect every signal directly to the ESP32-S3.

However, one important technical limitation must be mentioned. The echo signal from an ultrasonic sensor is timing-sensitive, because distance is calculated based on the duration of the echo pulse. The HC-SR04 datasheet explains that the module sends ultrasonic pulses and detects whether there is a returned pulse signal [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf). Because the MCP23017 communicates through I2C, it should not be used as the main method for polling fast echo pulse timing. The MCP23017 is better suited for scalable control and expansion signals, while timing-critical echo measurement should be handled by the ESP32-S3 or another suitable timing method.

---

### 5.5 Advice on measurement stability

To keep the ultrasonic measurements stable, the sensors should be read one by one. They should not all measure at the same time.

The HC-SR04 starts a measurement after receiving a trigger pulse and then sends eight ultrasonic pulses at 40 kHz [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf). Because ultrasonic sensors send sound waves into the environment, reading multiple sensors at the same time increases the risk of unstable readings. Sequential reading is therefore advised.

The advised measurement cycle is:

1. Select one parking sensor.
2. Send the trigger signal.
3. Measure the echo response.
4. Calculate the distance.
5. Compare the distance with the occupancy threshold.
6. Store the parking status.
7. Continue to the next sensor.

The advised occupancy logic is:

- if the measured distance is below the occupied threshold, the parking space is occupied;
- if the measured distance is above the free threshold, the parking space is free.

To improve stability, the system should use **hysteresis**. This means that two thresholds are used:

- one threshold to change from free to occupied;
- another threshold to change from occupied back to free.

This prevents the system from rapidly switching between free and occupied when the measured distance changes slightly.

---

### 5.6 Subconclusion

Based on this chapter, the advised technical setup is to use an ESP32-S3 with ultrasonic sensors and an MCP23017 I/O expander.

The ESP32-S3 is suitable because it provides embedded control and wireless communication. The MCP23017 is suitable because it supports future expansion by adding extra digital I/O pins through I2C. Ultrasonic sensors should be read sequentially to reduce unstable measurements and interference.

This setup supports the current four-space prototype, but also gives the project a better foundation for future expansion.

---

## 6. Chapter 3: Advice, risks and implementation direction

### 6.1 Introduction

This chapter answers the following subquestion:

**What risks must be considered when using ultrasonic sensors, an ESP32-S3 and an MCP23017 in a smart parking system?**

Every technical choice has risks. The goal is not to avoid all risk. The goal is to choose a realistic solution and manage the risks properly.

---

### 6.2 Advice based on requirements

The advised solution should meet the following functional requirements:

- detect whether each parking space is free or occupied;
- update parking status automatically;
- count the number of free spaces;
- show the parking status clearly;
- support future expansion to more parking spaces.

The advised solution should also meet the following non-functional requirements:

- simple enough to build in the current project phase;
- understandable for stakeholders without embedded systems experience;
- stable enough for a demonstration or pilot;
- lower complexity than camera-based systems;
- lower installation impact than pressure/contact sensors;
- scalable for future growth;
- maintainable by the project team.

The advised solution meets these requirements because ultrasonic sensors provide simple occupancy detection, the ESP32-S3 provides embedded control and connectivity, and the MCP23017 supports future expansion.

---

### 6.3 Risks and recommendations

#### Risk 1: Measurement errors caused by sensor placement

Ultrasonic sensors depend on sound reflection. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that ultrasonic sensors determine distance by measuring the round-trip time of the reflected sound wave. If a sensor is placed at the wrong angle, the reflected sound may not return correctly. This can cause unstable or incorrect measurements.

**Recommendation:**  
Mount each sensor in a fixed and tested position. The sensor should face the expected vehicle position as directly as possible.

---

#### Risk 2: Environmental influence on ultrasonic measurements

[(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that ultrasonic distance measurements can be influenced by environmental conditions such as wind, rain, snow, fog, humidity, air temperature, high-frequency noise and multipath effects. This is important because the prototype may work well in a controlled environment, but a real parking environment can be less predictable.

**Recommendation:**  
Use ultrasonic sensors mainly for controlled pilot environments or indoor/covered parking spaces. If the system is later used outdoors, the sensor placement and thresholds should be tested under realistic conditions.

---

#### Risk 3: Interference between multiple ultrasonic sensors

The HC-SR04 sends ultrasonic pulses after receiving a trigger signal [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf). If multiple ultrasonic sensors measure at the same time, sound waves from one sensor may influence another sensor’s reading.

**Recommendation:**  
Read the sensors sequentially. Only one sensor should send an ultrasonic pulse at a time.

---

#### Risk 4: Voltage difference between HC-SR04 and ESP32-S3

The standard HC-SR04 module works at 5V according to the datasheet [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf). The ESP32-S3 is a 3.3V microcontroller platform. Because of this voltage difference, the echo signal from the HC-SR04 should not be connected directly to the ESP32-S3 without voltage protection.

---

#### Risk 5: MCP23017 is not ideal for timing-sensitive echo measurement

The MCP23017 is useful for expanding digital I/O through I2C [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). However, ultrasonic echo measurement requires accurate pulse timing. The HC-SR04 distance measurement depends on the returned echo pulse [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

**Recommendation:**  
Use the MCP23017 mainly for scalable control, selection, enable lines or indicators. Keep echo timing measurement handled by the ESP32-S3 or another suitable timing method.

---

#### Risk 6: Camera-based alternatives may become more attractive at larger scale

For the current project phase, camera-based systems are not advised because they add more software complexity. However, at a larger scale, cameras may become more attractive because one camera can monitor multiple parking spaces. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that cameras can be used to monitor multiple parking spaces, but that image-based detection can require setup-specific training and can be affected by light and weather conditions.

**Recommendation:**  
Do not use camera-based detection in the first phase. Reconsider camera-based systems only if the project later requires larger coverage, visual verification or image-based analytics.

---

#### Risk 7: Pressure sensors require more installation effort

Pressure or contact sensors can detect the pressure or weight of a vehicle. [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) explains that piezoelectric sensors can be placed under asphalt. This makes them less practical for a first prototype where low installation impact is important.

**Recommendation:**  
Avoid pressure sensors in the current project phase. Reconsider them only if a future project requires ground-based detection and the installation cost is acceptable.

---

#### Risk 8: Scalability adds complexity

Adding the MCP23017 makes the system more scalable, but it also adds extra wiring, addressing and software logic. Since the MCP23017 has hardware address pins and multiple devices can be used on one I2C bus, the address structure must be documented clearly ([Microchip Technology Inc., 2005](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf); [Clark, 2025](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-mcp23017-i2c-gpio-expander.pdf)).

**Recommendation:**  
Use the MCP23017 as part of a structured architecture. Document the I2C addresses, connected signals and expansion plan clearly.

---

### 6.4 Subconclusion

The advised solution is technically suitable, but only if the risks are managed correctly. Ultrasonic sensors should be placed carefully and read sequentially. The ESP32-S3 should handle timing-sensitive measurements safely. The MCP23017 should be used for scalable I/O expansion, but not blindly for every signal.

The best implementation direction is therefore a simple ultrasonic smart parking system with a scalable architecture.

---

## 7. Final conclusion

This advice document examined which sensor and controller setup should be advised for smart parking occupancy detection using an ESP32-S3, while also considering future scalability.

The first conclusion is that ultrasonic sensors are the most suitable sensor type for the current project phase. They are simple, contactless and suitable for basic parking occupancy detection. Ultrasonic sensors can detect vehicle presence by measuring distance and comparing that distance with a reference value [(Khoshelham, 2021)](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf).

The second conclusion is that the ESP32-S3 is suitable as the main controller. It provides embedded control and integrated Wi-Fi and Bluetooth Low Energy, which makes it suitable for an IoT-based smart parking prototype [(Espressif Systems, z.d.)](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf).

The third conclusion is that the MCP23017 should be included in the technical direction because it supports scalability. It adds 16-bit I/O expansion through I2C, and its hardware addressing supports multiple devices on the same bus ([Microchip Technology Inc., 2005](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf); [Clark, 2025](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-mcp23017-i2c-gpio-expander.pdf)).

The final advice is:

**Use ultrasonic sensors for parking occupancy detection, controlled by one ESP32-S3, with an MCP23017 I/O expander included for scalable future expansion.**

This solution is the best fit for the current Smart City project because it balances simplicity, reliability, low installation impact and future scalability.

---

## 8. Recommendations

Based on this advice document, the following recommendations are made for Mayor Mats Otten and the project stakeholders:

1. Use ultrasonic sensors as the primary detection method for the first smart parking prototype.

2. Use the ESP32-S3 as the main controller because it supports embedded control and wireless communication.

3. Add the MCP23017 I/O expander to make the architecture more scalable for future parking spaces.

4. Use the MCP23017 mainly for control signals, expansion signals and indicators, not for inaccurate polling of ultrasonic echo timing.

5. Read ultrasonic sensors sequentially to reduce unstable measurements and interference.

6. Use threshold logic with hysteresis to prevent unstable switching between free and occupied.

7. Use a voltage divider or level shifter when connecting standard 5V HC-SR04 echo signals to the ESP32-S3.

8. Avoid pressure sensors in the first phase because they require more installation work.

9. Avoid camera-based detection in the first phase because it adds more software complexity.

10. Reconsider camera-based detection later only if the project expands to a larger parking environment where visual detection or image-based analytics become useful.

11. Document the system architecture clearly so that future expansion can be done in a controlled way.

---

## 9. References

1. Clark, L. (2025, April 15). Adafruit MCP23017 I2C GPIO expander. Adafruit Learning System. [https://cdn-learn.adafruit.com/downloads/pdf/adafruit-mcp23017-i2c-gpio-expander.pdf](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-mcp23017-i2c-gpio-expander.pdf) viewed on 11 May 2026

2. Espressif Systems. (z.d.). ESP32-S3 Series datasheet. [https://documentation.espressif.com/esp32-s3_datasheet_en.pdf](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf) viewed on 11 May 2026

3. Fahim, A., Hasan, M., & Chowdhury, M. A. (2021). Smart parking systems: comprehensive review based on various aspects. Heliyon, 7(5), e07050. [https://doi.org/10.1016/j.heliyon.2021.e07050](https://doi.org/10.1016/j.heliyon.2021.e07050) viewed on 11 May 2026

4. Khoshelham, K. (2021). Sensors for Parking Occupancy Detection. In Sensors For Parking Occupancy Detection [Book-chapter]. [https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf](https://repositum.tuwien.at/bitstream/20.500.12708/18077/1/Khoshelham-2021-Sensors%20for%20Parking%20Occupancy%20Detection-vor.pdf) viewed on 11 May 2026

5. Kumar, K., Singh, V., Raja, L., & Bhagirath, S. N. (2023). A Review of Parking Slot Types and their Detection Techniques for Smart Cities. Smart Cities, 6(5), 2639–2660. [https://doi.org/10.3390/smartcities6050119](https://doi.org/10.3390/smartcities6050119) viewed on 11 May 2026

6. Microchip Technology Inc. (2005). MCP23017/MCP23S17 (pp. 1–6). [https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf) viewed on 11 May 2026

7. Raspberry Pi Ltd. (2026). Raspberry Pi 4 Model B. [https://pip-assets.raspberrypi.com/categories/545-raspberry-pi-4-model-b/documents/RP-008344-DS-5-raspberry-pi-4-product-brief.pdf?disposition=inline](https://pip-assets.raspberrypi.com/categories/545-raspberry-pi-4-model-b/documents/RP-008344-DS-5-raspberry-pi-4-product-brief.pdf?disposition=inline) viewed on 11 May 2026 

8. Tech Support. (z.d.). Ultrasonic Ranging Module HC - SR04. [https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) viewed on 17 April 2026