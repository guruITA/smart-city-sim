# Advice - Recognizing when a car is waiting at the traffic light and sending that information to the backend

## Introduction

In this advice document, I evaluate whether my chosen solution was suitable for recognizing when a car is waiting at the traffic light and sending that information to the backend.

For this learning goal, I chose to use the **KY-021 Mini Magnetic Reed Switch Module** as a simple prototype sensor. I used it to represent the function of a road loop in a simplified way. The goal of this step was not to build a full real-world traffic detection system yet, but to prove that my project can detect a traffic situation, interpret it correctly, and pass that interpreted result to the backend.

This advice builds on the wider project direction that a traffic light becomes smart when it starts reacting to real-time input instead of only following a fixed cycle. The broader second-iteration requirements already define that the system must detect vehicles, use that input in traffic-light decisions, and fall back safely if no valid input is available. 

## Starting point

Before this learning goal, I already had a working first traffic-light prototype. The first iteration already gave me a stable base with the ESP32-S3, the three traffic-light outputs, fixed phase timing, safe startup in red, and a clear implementation flow from analysis to design to realization.  The earlier Fritzing work also showed that documenting the hardware design clearly made later building and checking easier. 

That was important, because it meant I did not have to solve everything at once. I could extend an already working traffic-light system with one new smart input.

## Was my chosen solution suitable

Yes, for this iteration I think my chosen solution was suitable.

The KY-021 was a good prototype choice because it let me focus on the most important learning result of this goal:

* noticing that something is happening at the stop line
* deciding whether that means waiting traffic or not
* sending that interpreted state to the backend

For this step, I did not yet need a complex sensor. I needed a sensor that was simple enough to wire, simple enough to read, and clear enough to test. The KY-021 gave me that.

So as a **prototype learning solution**, it was suitable.