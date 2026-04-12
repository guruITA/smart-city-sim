# Realise Documentation

Below is a short explanation of the most important parts of my code. The goal of the program is to control two opposite pedestrian crossings using the ESP32‑S3. Each side has a red and green LED, and each side has a button that can request a crossing.

### 1. Pin Setup

```
// Pedestrian lights side A
const int pedA_Green = 5;
const int pedA_Red   = 6;

// Pedestrian lights side B
const int pedB_Green = 1;
const int pedB_Red   = 2;

// Buttons
const int buttonA = 12;
const int buttonB = 13;
```

**Explanation:** Here I define which GPIO pins are used for the LEDs and the buttons. Each LED has its own pin so the ESP32 can control them individually.

## **2. Button Input Mode**

```
pinMode(buttonA, INPUT_PULLUP);
pinMode(buttonB, INPUT_PULLUP);
```

**Explanation:**

The buttons use **INPUT_PULLUP**, which means:

- Default state = **HIGH**
- When pressed = **LOW**

This prevents floating input and makes the button stable.

In my wiring, the button connects the pin to **GND** when pressed.

## **3. Default LED State**

```
digitalWrite(pedA_Red, HIGH);
digitalWrite(pedB_Red, HIGH);
```

**Explanation:**

At startup, both pedestrian crossings show **RED**, meaning pedestrians must wait.

This matches real‑world behavior.

## **4. Button Detection with Debounce**

```
if ((millis() - lastPressTime) > debounceDelay) {
  if ((digitalRead(buttonA) == LOW || digitalRead(buttonB) == LOW) && !systemBusy) {
      request = true;
      lastPressTime = millis();
  }
}
```

**Explanation:**

- The code checks if one of the buttons is pressed.
- `debounceDelay` prevents false triggers from noisy button signals.
- The system only accepts a request when it is not already running a crossing cycle.

## **5. Running the Crossing Sequence**

```
if (request) {
    systemBusy = true;
    runCrossing();
    request = false;
    systemBusy = false;
}
```

**Explanation:**

When a button is pressed, the system starts the crossing sequence.

`systemBusy` prevents multiple requests at the same time.

## **6. Crossing Logic (runCrossing function)**

```
// WAIT TIME (cars stop)
delay(8000);
```

**Explanation:**

Before pedestrians get green, the system waits 8 seconds.

This simulates the time needed for cars to stop safely.

```
// GREEN
digitalWrite(pedA_Red, LOW);
digitalWrite(pedB_Red, LOW);

digitalWrite(pedA_Green, HIGH);
digitalWrite(pedB_Green, HIGH);

delay(10000);
```

**Explanation:**

Both sides get **green** at the same time.

Pedestrians have 10 seconds to cross.

```
// BACK TO RED
digitalWrite(pedA_Green, LOW);
digitalWrite(pedB_Green, LOW);

digitalWrite(pedA_Red, HIGH);
digitalWrite(pedB_Red, HIGH);

delay(10000);
```

**Explanation:**

After the crossing time, both sides return to **red**.

A cooldown prevents spamming the system.

I programmed the full control logic: initial red state, button detection, waiting time, green phase, and cooldown.

I tested and adjusted the timing to ensure both sides always switch together.

I used serial prints to debug button input and timing behavior.

**Breadboard Assembly**

Below are the photos of my breadboard prototype.

The wiring follows the same structure as the Wokwi simulation and matches the code described above.

- I wired all LEDs with their 220Ω resistors and connected them to the correct GPIO pins (5, 6, 1, 2).
- I connected the buttons to GPIO 12 and 13 and to GND, using `INPUT_PULLUP` in the code.
- I connected the GND rail of the breadboard to the ESP32‑S3 GND pin to complete the circuit.
- The **ESP32‑S3** is placed across the center gap of the breadboard so each pin has its own row.

[](https://portfolio-lti-production.s3.eu-west-1.amazonaws.com/hva_brightspace_productie/va9azwa6g3t843rfvhy7vc7ctid9?response-content-disposition=inline%3B%20filename%3D%22IMG_0918.jpeg%22%3B%20filename%2A%3DUTF-8%27%27IMG_0918.jpeg&response-content-type=image%2Fjpeg&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAZUHNSBLFLRASQZLQ%2F20260412%2Feu-west-1%2Fs3%2Faws4_request&X-Amz-Date=20260412T190820Z&X-Amz-Expires=43200&X-Amz-SignedHeaders=host&X-Amz-Signature=1d1122dad77e53dcfa1c20111a568508f5d91ec92d0504ba1f8e01da55be8ba8)

*These photos shows the full breadboard setup with all LEDs and buttons connected according to my pin mapping*

[](https://portfolio-lti-production.s3.eu-west-1.amazonaws.com/hva_brightspace_productie/m40w7a4l5gdzplk1x2bedtd39hn3?response-content-disposition=inline%3B%20filename%3D%22IMG_0920.jpeg%22%3B%20filename%2A%3DUTF-8%27%27IMG_0920.jpeg&response-content-type=image%2Fjpeg&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAZUHNSBLFLRASQZLQ%2F20260412%2Feu-west-1%2Fs3%2Faws4_request&X-Amz-Date=20260412T190820Z&X-Amz-Expires=43200&X-Amz-SignedHeaders=host&X-Amz-Signature=7a98f594a7d0f277d387ed7c95db9aaedee352f1041b2737153afaf3951e9f68)

[](https://portfolio-lti-production.s3.eu-west-1.amazonaws.com/hva_brightspace_productie/1hvbf4afc7hzhi8vwqza0igbp5rs?response-content-disposition=inline%3B%20filename%3D%22IMG_0919.jpeg%22%3B%20filename%2A%3DUTF-8%27%27IMG_0919.jpeg&response-content-type=image%2Fjpeg&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAZUHNSBLFLRASQZLQ%2F20260412%2Feu-west-1%2Fs3%2Faws4_request&X-Amz-Date=20260412T190820Z&X-Amz-Expires=43200&X-Amz-SignedHeaders=host&X-Amz-Signature=bcb3b0409255c74ab50c5c0c0d22ddd2495b6d3a15a6a21217cb40a9a77e69ee)

**Testing & Validation**

- I tested each LED individually to confirm correct wiring.
- I tested the full flow: press button → red → green → red.
- I verified that both crossings always turn green at the same moment.

This prototype fully matches my Sprint 2 design and confirms that the system logic, wiring, and timing work correctly in real hardware.