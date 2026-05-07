# **Realisation Report — FastAPI Endpoint for Traffic Light Simulation Data**

## **1. Introduction**

This realisation document describes how I implemented and tested a FastAPI endpoint for receiving traffic light simulation data. This is part of my Sprint 3 learning goal: **Developing and testing FastAPI endpoints within a Docker environment**.

The realisation deliverable for this learning goal requires an implementation of FastAPI endpoints, an explanation of how I run and test the backend locally using Docker, testing evidence such as curl requests, and a reflection on my learning progress from not knowing to implementing.

The implemented endpoint receives JSON data from a traffic light simulation at an intersection. The backend validates the incoming data, logs the request, stores the event in the database and returns a simple response to confirm that the data was received.


## **2. Implementation Goal**

The goal of the implementation was to realise the design from the previous design document.

The implemented backend functionality should:

```
Receive traffic light simulation data
Validate the incoming JSON structure
Log incoming traffic events
Store the event in the database
Return a simple response to the embedded side
Run and be testable inside Docker
```

This matches the agreement with the embedded teammate. The embedded side needed an endpoint URL to send data to, while my backend responsibility was to create the endpoint, validate the JSON, log the request, store it in the database and return a simple response.


## **3. Files Changed**

To implement the traffic light endpoint, I made changes in the backend structure. The existing backend already uses a modular FastAPI structure with separate files for the app setup, database, models, schemas and routers.

The implementation affected these files:

| File | Change |
| --- | --- |
| `app/models.py` | Added the `TrafficEvent` database model |
| `app/schemas.py` | Added the `TrafficEventCreate` Pydantic schema |
| `app/routers/traffic.py` | Created a new router for traffic endpoints |
| `app/main.py` | Registered the traffic router |
| `docker-compose.yml` | Used existing Docker setup to run backend and database |


## **4. Database Model Implementation**

I first added a database model for traffic events. This model defines how incoming traffic light simulation events are stored in the PostgreSQL database.

```python
class TrafficEvent(Base):
    __tablename__ = "traffic_events"

    id = Column(Integer, primary_key=True, index=True)
    sensor_id = Column(String, nullable=False)
    direction = Column(String, nullable=False)
    phase = Column(String, nullable=False)
    interpreted_state = Column(String, nullable=False)
    timestamp_ms = Column(Integer, nullable=False)
    valid = Column(Boolean, nullable=False)
    created_at = Column(DateTime, default=datetime.utcnow)
```

The table stores both the data from the simulation and backend metadata. The simulation timestamp is stored in `timestamp_ms`, while `created_at` records when the backend received and stored the event.

This is useful because the project requires city data to be stored for future analysis with relevant metadata such as date/time, device and location. 


## **5. Schema Implementation**

Next, I created a Pydantic schema to validate incoming JSON data before it is stored.

```python
class TrafficEventCreate(BaseModel):
    sensorId: str
    direction: str
    phase: str
    interpretedState: str
    timestampMs: int
    valid: bool
```

This schema checks whether the embedded side sends all required fields with the correct data types.

The expected JSON payload is:

```json
{
  "sensorId": "north_1",
  "direction": "north",
  "phase": "NS_RED",
  "interpretedState": "waiting_vehicle",
  "timestampMs": 123456,
  "valid": true
}
```

This structure comes from the embedded side. It contains the sensor ID, direction, current traffic light phase, interpreted traffic state, timestamp and whether the data is valid.


## **6. Router Implementation**

I created a new router file:

```
app/routers/traffic.py
```

This keeps the traffic endpoint separate from other existing API routes, such as parking or generic readings. The existing backend already uses routers for different parts of the system, so I followed the same structure.

The router uses this prefix:

```python
router = APIRouter(
    prefix="/api/v1/traffic",
    tags=["traffic"]
)
```

This creates a clean route group for traffic-related endpoints.


## **7. Endpoint Implementation**

The main endpoint I implemented is:

```
POST /api/v1/traffic/update
```

The endpoint receives a traffic event, converts the JSON fields into database fields, saves the event and returns a simple response.

```python
@router.post("/update")
def create_traffic_event(
    event: TrafficEventCreate,
    db: Session = Depends(get_db)
):
    traffic_event = TrafficEvent(
        sensor_id=event.sensorId,
        direction=event.direction,
        phase=event.phase,
        interpreted_state=event.interpretedState,
        timestamp_ms=event.timestampMs,
        valid=event.valid
    )

    db.add(traffic_event)
    db.commit()
    db.refresh(traffic_event)

    print(
        f"Received traffic event: "
        f"sensor={event.sensorId}, "
        f"direction={event.direction}, "
        f"phase={event.phase}, "
        f"state={event.interpretedState}, "
        f"valid={event.valid}"
    )

    return {"status": "received"}
```

The endpoint keeps the logic intentionally simple. It does not calculate traffic light states yet and it does not send commands back to the ESP32. This matches the sprint agreement: this sprint focuses on storing JSON data, while traffic logic and action commands can be added later.


## **8. Registering the Router**

To make the endpoint available in the FastAPI application, I registered the new router in `main.py`.

```
fromapp.routersimporttraffic

app.include_router(traffic.router)
```

After registering the router, the endpoint becomes available locally at:

```
http://localhost:8000/api/v1/traffic/update
```

When the backend is deployed on the Raspberry Pi, the same endpoint becomes available at:

```
http://145.92.8.137:80/api/v1/traffic/update
```

This is important because local testing only proves that the endpoint works on my laptop. The deployed Raspberry Pi endpoint is needed so that my teammate’s ESP32 or traffic light simulation can send data to the shared backend.

Swagger can be used locally at:

```
http://localhost:8000/docs
```

And on the Raspberry Pi at:

```
http://145.92.8.137:80/docs
```


## **9. Running the Backend Locally in Docker**

After implementing the endpoint, I first ran the backend locally using Docker Compose.

```
docker compose up--build
```

This starts both the FastAPI API and the PostgreSQL database. The existing backend documentation describes this as the quick start command for running the backend locally.

The local backend URL is:

```
http://localhost:8000
```

The local Swagger documentation is:

```
http://localhost:8000/docs
```

This local test is useful because I can check whether my code works before deploying it to the shared Raspberry Pi server.

### **Evidence**

![image.png](attachment:70fe849e-ce1c-41b7-9aaf-0895acec2440:image.png)

![image.png](attachment:dd134416-9d49-4b80-a191-79a749b9a03e:image.png)

---

## **10. Deploying the Backend to the Raspberry Pi**

After testing the endpoint locally, the backend needs to be deployed to the Raspberry Pi so that other team members and ESP32 devices can reach it.

The Raspberry Pi is used as the central server for the City Sim project. It runs the shared FastAPI backend and PostgreSQL database in Docker containers. The Raspberry Pi is reachable on the HvA network at:

```
http://145.92.8.137:80
```

The deployment guide explains that the backend is deployed on the Raspberry Pi from the backend folder using:

```
bash deploy.sh
```

The deployment script builds and starts the Docker containers, waits for the health check and prints the access URLs.

After deployment, I can verify that the backend is running with:

```
curl http://145.92.8.137:80/health
```

The deployed Swagger documentation is available at:

```
http://145.92.8.137:80/docs
```

The deployed traffic endpoint is:

```
POST http://145.92.8.137:80/api/v1/traffic/update
```

This deployment step is necessary because my teammate cannot send ESP32 data to my local `localhost`. `localhost` only works on my own laptop. The ESP32 needs to send data to the Raspberry Pi IP address, because that is the shared backend server on the HvA network.

### **Evidence**

```
[Add screenshot: health check on http://145.92.8.137:80/health]
[Add screenshot: deployed Swagger at http://145.92.8.137:80/docs]
[Add screenshot: traffic endpoint visible on deployed Swagger]
```
---
will be updated