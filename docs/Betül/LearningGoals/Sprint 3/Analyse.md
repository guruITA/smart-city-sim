

# **Analysis Report — Developing and Testing FastAPI Endpoints within a Docker Environment**

## **1. Introduction**

This analysis report is part of my Sprint 3 learning goal: **Developing and testing FastAPI endpoints within a Docker environment**. The main learning question for this goal is:

> **How can I design, build, and test FastAPI endpoints that process and filter incoming data, while working effectively within a Docker-based backend environment?**
>

The goal of this learning activity is to shift my focus from embedded work toward backend development. At the start of Sprint 3, I had not yet actively developed FastAPI endpoints myself. My understanding of how the backend, Docker environment and Raspberry Pi deployment connect was still limited. Therefore, this analysis focuses on understanding the current backend context, the expected data flow, my role within the team, and the knowledge gap I need to close.

In the current project scope, I am no longer working on a T-junction street lighting tile. Instead, I am focusing on backend functionality for a **traffic light simulation at an intersection**. The backend receives JSON data from the traffic light simulation or ESP32 component. This data describes sensor events, traffic light phases, detected directions and interpreted traffic states.

## **2. Project Context**

The project is part of the Smart City SIM learning group. The system combines embedded systems and backend technology into one smart city prototype. In this project, embedded components such as ESP32 devices, sensors and actuators generate data. The backend is responsible for receiving, storing and making this data available for future analysis.

The requirements describe the backend as the “brains” of the project. This means that data from hubs with sensors and output devices should be collected in the backend. The backend should also make it possible to override decisions of individual hubs in the future, for example by controlling traffic lights on a road to make space for an emergency vehicle. The data should be stored with relevant metadata such as date/time, device, IP address and location, and all backend services should run in Docker.

For my learning goal, the most important part is not yet advanced decision-making. The current focus is on creating a reliable backend endpoint that can receive incoming traffic simulation data and store it correctly.

## **3. Current Backend Situation**

The existing backend is a shared API for The Embedded Alliance smart city project. It receives sensor data from ESP32 tiles through WiFi HTTP and can serve this data to dashboards and displays. The backend can be started using Docker Compose and runs locally at:

```
http://localhost:8000
```

The Swagger documentation is available at:

```
http://localhost:8000/docs
```

The backend project structure contains a FastAPI application, database configuration, models, schemas and routers. Important files include:

```
backend/
  app/
    main.py
    database.py
    models.py
    schemas.py
    routers/
      readings.py
      parking.py
  Dockerfile
  docker-compose.yml
  requirements.txt
```

This structure shows that the backend already follows a modular FastAPI setup. The API routes are separated into router files, the database logic is placed in a separate file, and the data models and schemas are also separated. This means that my own traffic light endpoint should probably follow the same structure instead of placing all code directly in `main.py`.

## **4. Team Role Specification**

For this sprint, the collaboration between embedded and backend is focused on sending traffic light simulation data to the backend.

The embedded side is responsible for generating and sending the traffic light data. This means that my teammate works on the traffic light simulation, sensor behaviour and WiFi communication. From the embedded side, I receive a JSON payload with information about the traffic light situation at the intersection.

An example of the data I receive is:

```
{
  "sensorId":"north_1",
  "direction":"north",
  "phase":"NS_RED",
  "interpretedState":"waiting_vehicle",
  "timestampMs":123456,
  "valid":true
}
```

My responsibility on the backend side is to create an endpoint where this data can be sent to. I need to make sure that the backend can receive the JSON data, validate the required fields, log the incoming request and store the data in the database.

In return, I give the embedded side a clear endpoint URL and a simple response format. This allows my teammate to test whether the traffic light simulation can successfully communicate with the backend.

For this sprint, the backend response can stay simple:

```
{
  "status":"received"
}
```

This means that the embedded side knows the data has reached the backend. More advanced backend logic, such as sending traffic light commands back to the simulation, can be added in a later sprint.

## **5. Input Data Analysis**

The traffic light simulation sends JSON data to the backend. The expected JSON structure is:

```
{
  "sensorId":"north_1",
  "direction":"north",
  "phase":"NS_RED",
  "interpretedState":"waiting_vehicle",
  "timestampMs":123456,
  "valid":true
}
```

Each field has a specific meaning:

| Field | Meaning |
| --- | --- |
| `sensorId` | Identifies the sensor or simulated sensor |
| `direction` | Direction of the road or lane, for example north, south, east or west |
| `phase` | Current traffic light phase, for example `NS_RED` |
| `interpretedState` | Meaning interpreted by the simulation, for example `waiting_vehicle` |
| `timestampMs` | Timestamp from the simulation in milliseconds |
| `valid` | Indicates whether the backend should accept or ignore the event |

This structure is important because it gives the backend enough context to store the event meaningfully. Instead of only storing a raw value, the backend can store which sensor sent the data, from which direction, during which traffic phase and whether the data was valid.

## **6. Data Flow Analysis**

The expected data flow for this sprint is:

```
Traffic light simulation / ESP32
→ HTTP POST request
→ FastAPI endpoint
→ Pydantic validation
→ Logging
→ Database storage
→ Simple JSON response
```

In more detail:

1. The traffic light simulation detects or generates a traffic event.
2. The simulation sends a JSON payload to the backend endpoint.
3. FastAPI receives the request.
4. Pydantic checks whether the required fields are present and have the correct type.
5. The backend logs the incoming event.
6. The backend stores the event in the database.
7. The backend returns a simple response, for example:

```
{
  "status":"received"
}
```

At this stage, the backend does **not** need to make traffic decisions. It does not need to return commands such as “turn north green” or “switch east/west to red”. That logic can be added later. For Sprint 3, the priority is receiving, validating and storing traffic events.

## **7. Docker Environment Analysis**

The backend runs inside Docker. This is important because the project requirements state that all backend services should run in Docker.

The current backend can be started with:

```
docker compose up--build
```

This starts the API and PostgreSQL database. The API then runs at:

```
http://localhost:8000
```

For my learning goal, this means I need to test my endpoint inside the Docker environment, not only by running Python locally. This is important because the final backend should be deployable in a shared environment and eventually connect to the Raspberry Pi setup.

My current understanding is:

```
FastAPI application = backend code
Dockerfile = defines how the API container is built
docker-compose.yml = starts API + database together
PostgreSQL = stores incoming traffic events
Raspberry Pi = future deployment target
```

At the start of this learning goal, I do not fully understand every part of the Docker and Raspberry Pi setup. However, I do need to understand enough to run the backend locally, test endpoints and explain how my backend code fits into the containerized environment.

## **8. Knowledge Gap Analysis**

At the start of Sprint 3, my main knowledge gap is the transition from embedded work to backend development. I have worked more on hardware-related tasks, but I have not yet actively implemented FastAPI endpoints myself.

The main things I still need to learn are:

```
1. How FastAPI endpoints are structured
2. How routers are used in the existing backend
3. How Pydantic schemas validate incoming JSON data
4. How SQLAlchemy models store data in PostgreSQL
5. How Docker Compose runs the backend and database together
6. How to test POST endpoints using curl or Swagger
7. How to document API input, processing and output clearly
```

This knowledge gap is directly connected to my learning goal. To achieve the goal successfully, I need to move from only understanding the embedded side to understanding how backend endpoints receive and process data from that embedded side.

## **9. Scope for Sprint 3**

The scope for this sprint should stay small and realistic. The first backend version should focus on receiving and storing traffic light data.

### **In scope for Sprint 3**

```
Create a traffic endpoint
Validate incoming JSON
Log incoming requests
Store traffic events in the database
Return a simple success response
Test the endpoint with curl or Swagger
Document the data flow
```

### **Out of scope for Sprint 3**

```
Advanced traffic light decision logic
State machine for traffic phases
Returning commands to the ESP32
Emergency vehicle override logic
Dashboard visualisation
Complex filtering or analytics
```

This scope is realistic because the teammate currently only needs an endpoint to send data to. The more advanced logic can be implemented in a later sprint after the data receiving and storage layer works correctly.

## **10. Proposed Backend Responsibility**

Based on this analysis, my backend responsibility should be:

> Create a FastAPI endpoint that receives traffic light simulation data, validates the JSON body, logs the incoming event, stores it in the database, and returns a simple confirmation response.
>

A possible endpoint name is:

```
POST /api/v1/traffic/update
```

The endpoint should receive this kind of input:

```
{
  "sensorId":"north_1",
  "direction":"north",
  "phase":"NS_RED",
  "interpretedState":"waiting_vehicle",
  "timestampMs":123456,
  "valid":true
}
```

And return:

```
{
  "status":"received"
}
```

This endpoint supports the project requirements because it collects sensor data from a hub/simulation and prepares the data for future analysis. It also fits the existing backend architecture because the backend already uses FastAPI, Docker, routers, schemas and database models.

## **11. Risks and Challenges**

There are a few risks in this learning goal.

The first risk is that I may spend too much time trying to understand the full backend infrastructure. Since the Docker and Raspberry Pi setup are handled by a teammate, I should focus on what I need for my own contribution: running the backend locally, adding an endpoint and testing it.

The second risk is making the endpoint too complex too early. It is tempting to start building traffic logic immediately, but that is not the current requirement. First, the backend must reliably receive and store traffic events.

## **12. Conclusion**

This analysis shows that my Sprint 3 learning goal is focused on moving from embedded work toward backend development. The Smart City backend is responsible for collecting data from embedded hubs and storing this data for future analysis. The backend runs in Docker and uses a FastAPI structure with routers, schemas, models and a PostgreSQL database.

For my specific contribution, I will focus on a traffic light simulation at an intersection. The simulation sends JSON data containing fields such as `sensorId`, `direction`, `phase`, `interpretedState`, `timestampMs` and `valid`. My backend task is to create an endpoint that receives this data, validates it, logs it, stores it and returns a simple response.

This means the first version of my backend work does not need advanced traffic logic yet. The goal is to build a reliable foundation for data collection. Later, this can be extended with filtering, traffic decisions, commands back to the ESP32 and dashboard functionality.

By completing this learning goal, I will better understand how FastAPI endpoints work, how incoming embedded data flows through the backend, how Docker supports the backend environment, and how my role fits into the larger Smart City system.
