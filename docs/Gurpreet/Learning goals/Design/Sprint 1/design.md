# Smart Cities Learning group goal: Design

- Naam: Gurpreet Singh
- Datum: 11-2-2026

## Table of Contents
- [Smart Cities Learning group goal: Design](#smart-cities-learning-group-goal-design)
  - [Table of Contents](#table-of-contents)
  - [What is a breadboard?](#what-is-a-breadboard)
  - [Sources (used scribbr)](#sources-used-scribbr)

## What is a breadboard?

According to (How To Use A Breadboard - SparkFun Learn, z.d.), a breadboard is a solderless component used during prototyping. This makes it useful for beginners like me, because nothing has to be soldered and components can easily be changed or moved if something is connected incorrectly.

The most important thing for my design is understanding how the internal connections of the breadboard work. Each horizontal row has 5 connected clips on the left side and 5 connected clips on the right side of the ravine in the middle of the breadboard. The left and right side are not connected to each other. This is important for me to understand before making a Fritzing design.

The red + and blue/black - power rails run vertically on the sides of the breadboard. This means I can connect the power supply to these rails and then distribute voltage and ground more easily to the LEDs, LDR module and relay module.

Rules I need to remember:

- Left and right side of the ravine are separate, which is useful for placing the ESP32-S3.
- Power rails may need jumper wires if I want to use both sides of the breadboard.
- I should always check for short circuits before powering the circuit.
- I should use clear wire colors during wiring, for example red for 5V or 3.3V, black for GND, and other colors for signal wires, so that it stays understandable for the team as described in the discussion on (Miniika, n.d.).

## Sources (used scribbr)

1. How to Use a Breadboard - SparkFun Learn. (z.d.). https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard/all#introduction
2. Miniika. (n.d.). Color conventions for breadboard wires? : r/AskElectronics. https://www.reddit.com/r/AskElectronics/comments/llitd8/color_conventions_for_breadboard_wires/