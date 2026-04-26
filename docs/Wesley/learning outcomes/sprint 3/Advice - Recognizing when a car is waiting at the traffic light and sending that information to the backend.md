# Advice - Recognizing when a car is waiting at the traffic light and sending that information to the backend

## Introduction

In this advice document, I evaluate whether my chosen solution was suitable for recognizing when a car is waiting at the traffic light and sending that information to the backend.

For this learning goal, I chose to use the **KY-021 Mini Magnetic Reed Switch Module** as a simple prototype sensor. I used it to represent the function of a road loop in a simplified way. The goal of this step was not to build a full real-world traffic detection system yet, but to prove that my project can detect a traffic situation, interpret it correctly, and pass that interpreted result to the backend.

This advice builds on the wider project direction that a traffic light becomes smart when it starts reacting to real-time input instead of only following a fixed cycle. The broader second-iteration requirements already define that the system must detect vehicles, use that input in traffic-light decisions, and fall back safely if no valid input is available. 