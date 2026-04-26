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

## What worked well

### 1. The solution was simple enough to understand and test

The KY-021 works with a simple digital signal. That made it easier to connect to the ESP32-S3 and easier to reason about in software. For this learning goal, that simplicity was helpful, because I wanted to focus on the interpretation logic rather than on complicated sensor calibration.

### 2. It fit well on top of my existing traffic-light prototype

Because the first iteration already worked physically and logically, I could use that earlier prototype as the base and only add the new sensor flow. That made the step realistic and manageable instead of too large at once. The earlier implementation already proved that my hardware base and traffic-light sequence were stable enough to build on. 

### 3. It helped me think in traffic states instead of raw inputs

This was the strongest part of the solution. The real learning was not that the pin can become active. The real learning was that the same sensor trigger can mean different things depending on the traffic-light phase.

Because of that, this learning goal pushed me to work with interpreted states such as:

* no vehicle
* waiting vehicle
* passing vehicle
* unclear input

That is a more useful system design than only sending raw sensor values to the backend.

### 4. It matched the broader smart-traffic direction of the project

The wider project already says that smart behavior begins when the system uses sensor input as part of traffic-light decisions. This solution fits that direction, because it is the first step where the traffic light starts responding to something happening on the road instead of only following a timer. 