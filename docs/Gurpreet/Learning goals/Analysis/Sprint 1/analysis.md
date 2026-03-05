# Smart Cities Learning group goal: Analyse

Naam: Gurpreet Singh

Datum: 11-2-2026

Learning question: How can I analyse what is needed to turn a normal streetlight into a “smart” streetlight using an ESP32-S3 and simple sensors, while I am still learning how the ESP32 works?

## Smart Streetlight + ESP32-basis

Learning Question
How can I analyse what is needed to turn a normal streetlight into a “smart” streetlight using an ESP32-S3 and simple sensors, while I am still learning how the ESP32 works?

S — Situation
In the first sprint of the City Sim Learning Group I want to build a smart streetlight on my wooden city tile. The idea is that the light should react to the environment instead of always being on, for example by switching on when it is dark. I am new to Embedded & Robotics and I do not yet understand how the ESP32-S3 works or which pins and features I can use. At the same time, the ESP32-S3-DevKitC-1 is the microcontroller I must use in this project.

T — Task
My goal in this sprint is to analyse what “smart” should mean for this streetlight and what is needed to realise that with the ESP32-S3. I want to find out when and why the streetlight should turn on or off, which sensors and other components are needed for that behaviour, and which basic ESP32-S3 features and pins are relevant for connecting a light sensor and a lamp.

A — Action
To reach this goal I will first look at examples of smart streetlights and simple IoT lighting projects to understand typical functions such as turning on in the dark, dimming, or reacting to presence. I will then write down functional requirements, for example that the light turns on when a certain darkness level is reached, and simple non-functional requirements such as low energy use and reliability. In parallel, I will study the official Espressif documentation for the ESP32-S3-DevKitC-1 to understand which pins, voltages and features I can use for a light sensor and LED.

R — Result
The expected result is a short, clear problem-analysis document in which I describe what my smart streetlight must do, which components are realistic candidates, and which ESP32-S3 features I need to use for this first sprint. This document will show how the initial idea is translated into concrete technical needs that I can use in the design phase.

R — Reflection
After finishing this analysis I will reflect on how much my understanding of the problem has improved and where my lack of ESP32 knowledge still limits me. I will also look at how useful the ESP32 documentation was for me as a beginner and what I still find unclear or confusing.

T — Transfer
I will use the results of this analysis directly in the next phase, where I design the Fritzing schematic for the smart streetlight. The way I combined problem-analysis with studying the ESP32 documentation will serve as a template for later sprints, for example when I analyse the traffic light with a pressure sensor or the pedestrian crossing.

Appendix (optional)

Detailed logs
Additional figures/tables
Full code/artefacts

References

Author, A. (Year). Title. Publisher. [https://link](https://link/) (accessed YYYY-MM-DD)
Organization. (Year). Title of webpage/report. [https://link](https://link/) (accessed YYYY-MM-DD)
Dataset/Tool. Version. Provider. [https://link](https://link/) (accessed YYYY-MM-DD)

# Feedback from Mats
Mats feedback is that I should think about the overall system and all the tiles, not just my own tile. He advises discussing this with my team to make sure everything connects properly. While developing this, I should also consider how I am going to set it up in a way that it works across the entire city as a whole. Additionally, Mats mentions that the “Result, Reflection and Transfer” section does not need to be filled in yet, and should only be completed after finishing the action. He also advises that I should clearly and concretely describe what I am going to deliver in the action section. Lastly, he emphasizes that I should keep updating this continuously.

