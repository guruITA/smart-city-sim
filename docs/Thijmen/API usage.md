# TrainPredictionSignal API Documentation

## Overview

The `TrainPredictionSignal` class integrates a hardware railroad crossing system with a backend API. It tracks train events, predicts arrival times, and manages barrier actions.

---

## **Current API Usage**

These endpoints are already implemented and functional.

| Feature                                      | Endpoint                                           | Method | Request Body                             | Description                                                                    |
| -------------------------------------------- | -------------------------------------------------- | ------ | ---------------------------------------- | ------------------------------------------------------------------------------ |
| **Create Train**                             | `/api/v1/railroadcrossing/train/first`             | POST   | `{}`                                     | Registers a new train and returns a `trainId`.                                 |
| **Update Second Sensor / Predicted Arrival** | `/api/v1/railroadcrossing/train/{trainId}/second`  | PUT    | `{"predicted_arrival_seconds": <float>}` | Sends predicted arrival time at the second sensor. Requires `trainId`.         |
| **Mark Train as Crossed**                    | `/api/v1/railroadcrossing/train/{trainId}/crossed` | PUT    | `{}`                                     | Notifies the backend that the train has passed the crossing. Resets `trainId`. |

**Usage Notes:**

* API calls are triggered by button presses.
* Local simulation continues if backend is unavailable.
* HTTP requests are handled with `httpRequest()`, which uses `NetworkController`.

---

## **Future Workflow with Remote Barrier**

1. Train detection and prediction happen as usual.
2. Backend can remotely trigger barrier:
   * Calls with `"open"` or `"closed"`.
   * Barrier position updates locally and logs.
3. Train crossing events are updated as usual with `/second` and `/crossed`.
4. Barrier logs create historical records for safety auditing.

---
