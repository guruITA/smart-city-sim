# Advice Document

## Table of Contents

- [1. Overview](#1-overview)
- [2. Evaluation vs Requirements](#2-evaluation-vs-requirements)
- [3. Challenges Faced](#3-challenges-faced)
- [4. What Went Well](#4-what-went-well)
- [5. What Could Be Improved](#5-what-could-be-improved)
- [6. Recommendations](#6-recommendations)
- [7. Advice for the Shared Project (visual components)](#7-advice-for-the-shared-project-visual-components)
- [8. References and Links](#8-references-and-links)

## 1. Overview

This document evaluates the Sprint 3 e-ink tile implementation, reflects on the technical challenges encountered, and provides recommendations for future improvements and for how the shared city-sim project can better support visual components like displays.

## 2. Evaluation vs Requirements

- Requirement: Drive a 2.9" e-ink display from an ESP32 reliably. Result: implemented as a reusable class (`EinkDisplay`) that exposes `begin()` and `startSyncTask()` and runs updates in a background task. See the implementation in the repo links below.
- Requirement: Respect e-ink constraints (low power, slow refresh, ghosting avoidance). Result: conservative update strategy was used; background sync and BUSY detection implemented but can be optimized.
- Requirement: Integrate with the backend so tiles can be updated from the city-sim system. Result: router and static UI pieces exist to provide tile content and import images, integration is present but could be hardened for intermittent connectivity.

Overall: the solution meets the core functional requirements (drive the display, run on ESP32, integrate with backend). Non-functional requirements (update efficiency, robustness to network failure, and power optimisations) are partially met and are good candidates for improvement.

## 3. Challenges Faced

- E-ink characteristics: slow full refreshes, ghosting, and limited update frequency require careful update strategies.
- Hardware wiring and correct pin sequencing (RST / BUSY / DC / CS) were fiddly; powering the display reliably during updates required attention. See wiring image in References.
- Memory and buffering: keeping a full frame buffer on the ESP32 is costly; image conversion and sizing needed to be addressed.
- Concurrency: making sure the display task cooperates with other FreeRTOS tasks and the network stack without blocking the system was non-trivial.
- Backend coupling: synchronising content and retrying failed updates in a robust way (retries, backoff, idempotency) required extra engineering.

## 4. What Went Well

- Modular design: `EinkDisplay` is implemented as a class-based component that can be reused across tiles.
- Background updates: using a FreeRTOS task to perform display updates avoids blocking main application logic.
- Clear pin configuration: `Config.h` pin mapping allows easy remapping for different ESP boards.
- Documentation and assets: the repo contains a wiring diagram and a `Realise.md` walkthrough which made assembly and testing easier.

## 5. What Could Be Improved

- Update strategy: move from full-screen, frequent refreshes to batched, partial updates or change detection to reduce wear and power use.
- Image pipeline: add conversion tooling to produce correctly sized, dithered bitmaps (or compressed payloads) on the backend to reduce ESP-side work.
- Robustness: add persistent queues and retry/backoff for updates when the network is flaky; make updates idempotent so repeats are safe.
- Power management: implement deeper sleep between updates and ensure proper power sequencing for the display to reduce consumption.
- Telemetry and logging: more run-time metrics (update latency, failures, BUSY time) to help tune performance.

## 6. Recommendations

- Implement a standardized display interface in the shared firmware (e.g. `IDisplay` with `show(Bitmap)`, `partialUpdate(Rect)`, `status()`), and refactor `EinkDisplay` to implement it.
- Add a display manager that serializes requests, batches multiple small updates, and enforces safe intervals to avoid ghosting.
- Define a small update protocol between backend and tiles (JSON schema) that supports: full-image, delta, and commands (clear, partial-update, metatada), with versioning.

## 7. References and Links

- Design document and wiring diagram: [docs/Thijmen/Sprint 3/Realise.md](docs/Thijmen/Sprint 3/Realise.md)
- Implementation notes: [docs/Thijmen/Sprint 3/Realise.md](docs/Thijmen/Sprint 3/Realise.md)
- Display library source files: `embedded/city-sim/lib/EinkDisplay/EinkDisplay.h` and `embedded/city-sim/lib/EinkDisplay/EinkDisplay.cpp` 
- Firmware usage example: `embedded/city-sim/city-sim.ino`

