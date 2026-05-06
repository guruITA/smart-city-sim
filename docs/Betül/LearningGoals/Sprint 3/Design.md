# Design

# **Design Report**



# **Traffic Light Simulation Endpoint**

## **1. Design Purpose**

This design describes the backend solution for receiving traffic light simulation data from the embedded side. The goal is to design a FastAPI endpoint that receives JSON data, validates the input, stores the event in the database and returns a simple response.

This design is part of my Sprint 3 learning goal, where I need to design, build and test FastAPI endpoints that process incoming data inside a Docker-based backend environment. The design deliverable specifically requires an endpoint design with input, processing, output, filtering logic and a connection to the Docker/Raspberry Pi environment.



## **2. Endpoint Design**

### **Endpoint**

```
POST /api/v1/traffic/update
```

### **Purpose**

This endpoint receives one traffic light simulation event from the embedded side.

### **Reason for using POST**

`POST` is used because the embedded side sends a new event to the backend. The backend does not only read data; it creates a new database record.

### **Final endpoint URL for local testing**

```
http://localhost:8000/api/v1/traffic/update
```

The backend already runs locally on `localhost:8000` when started with Docker Compose.



## **3. Request Body Design**

The embedded side will send the following JSON structure:

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

This is the JSON template agreed with the embedded teammate. The embedded side sends sensor ID, direction, traffic light phase, interpreted state, timestamp and whether the data is valid.

### **Request Field Specification**

| JSON field | Type | Required | Example | Design reason |
| --- | --- | --- | --- | --- |
| `sensorId` | string | yes | `north_1` | Identifies which simulated sensor sent the event |
| `direction` | string | yes | `north` | Shows which road direction the event belongs to |
| `phase` | string | yes | `NS_RED` | Stores the traffic light phase during the event |
| `interpretedState` | string | yes | `waiting_vehicle` | Stores what the simulation interpreted |
| `timestampMs` | integer | yes | `123456` | Stores the timestamp from the simulation |
| `valid` | boolean | yes | `true` | Shows whether the event is valid or should be treated as debug/invalid data |


## **4. Response Design**

### **Success Response**

```json
{
  "status": "received"
}
```

### **Reason for simple response**

For Sprint 3, the embedded side only needs confirmation that the backend received the data. No traffic commands are returned yet. The agreement with the embedded teammate is that this sprint focuses on storing JSON responses in the database, while traffic logic and action commands can be added in a later sprint.

### **Possible future response**

This is **not** part of Sprint 3, but could be added later:

```json
{
  "status": "received",
  "command": "keep_current_phase"
}
```


## **5. Validation Design**

The endpoint will use a Pydantic schema to validate incoming JSON before storing it.

### **Planned Pydantic Schema**

```python
class TrafficEventCreate(BaseModel):
    sensorId: str
    direction: str
    phase: str
    interpretedState: str
    timestampMs: int
    valid: bool
```

### **Validation rules**

| Rule | Expected behaviour |
| --- | --- |
| Missing field | Request is rejected by FastAPI |
| Wrong data type | Request is rejected by FastAPI |
| `valid = true` | Event is stored as valid event |
| `valid = false` | Event is still stored, but marked invalid |
| Unknown extra fields | Can be ignored or rejected depending on schema configuration |

### **Why use Pydantic?**

Pydantic makes the input contract clear. It also prevents incomplete traffic events from being stored without structure.


## **6. Data Processing Design**

The endpoint will process data in this order:

```
1. Receive JSON request
2. Validate request body with Pydantic
3. Convert JSON field names to database field names
4. Create TrafficEvent database object
5. Save object to PostgreSQL
6. Return success response
```

### **Processing mapping**

| Incoming JSON field | Database field |
| --- | --- |
| `sensorId` | `sensor_id` |
| `direction` | `direction` |
| `phase` | `phase` |
| `interpretedState` | `interpreted_state` |
| `timestampMs` | `timestamp_ms` |
| `valid` | `valid` |

The conversion from camelCase to snake_case keeps the API format suitable for the embedded side while keeping the backend/database style consistent with Python conventions.


## **7. Filtering Logic Design**

For Sprint 3, filtering will stay minimal. The backend will not make traffic decisions yet.

### **Chosen filtering rule**

```
If valid == true:
    store event as normal traffic simulation data

If valid == false:
    store event, but mark it as invalid
```

### **Reason**

Invalid events can still be useful for debugging. If the embedded side sends incorrect or test data, storing it helps the team inspect what happened later.

### **Not included in this sprint**

```
No traffic phase calculation
No state machine
No emergency vehicle override
No command returned to ESP32
No automatic traffic light control
```

This matches the current agreement: this sprint is about receiving and storing JSON data, while logic and action commands are planned for later.


## **8. Database Table Design**

### **Table name**

```
traffic_events
```

### **Database fields**

| Column | Type | Nullable | Reason |
| --- | --- | --- | --- |
| `id` | integer | no | Unique primary key |
| `sensor_id` | string | no | Stores which sensor sent the event |
| `direction` | string | no | Stores traffic direction |
| `phase` | string | no | Stores traffic light phase |
| `interpreted_state` | string | no | Stores interpreted simulation state |
| `timestamp_ms` | integer | no | Stores timestamp from embedded/simulation side |
| `valid` | boolean | no | Stores whether event is valid |
| `created_at` | datetime | no | Stores when backend received the event |

The project requirements state that stored backend data should include relevant metadata such as date/time, device, IP and location. For this first version, `sensor_id`, `direction`, `timestamp_ms` and `created_at` provide the most relevant metadata for the traffic simulation event.


## **9. Planned SQLAlchemy Model**

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
    created_at = Column(DateTime(timezone=True), server_default=func.now())
```

### **Design choice**

A separate `traffic_events` table is chosen instead of storing the data only in a generic readings table. The traffic light simulation sends multiple specific fields, such as `phase` and `interpretedState`, which are more specific than a simple sensor value. A separate table keeps the structure clearer for future traffic logic.


## **10. File Structure Design**

The existing backend already uses a modular structure with separate files for the FastAPI app, database, models, schemas and routers.

The traffic endpoint will be added using the same structure.

```
backend/
  app/
    main.py
    models.py
    schemas.py
    routers/
      traffic.py
```

### **Required file changes**

| File | Change |
| --- | --- |
| `models.py` | Add `TrafficEvent` database model |
| `schemas.py` | Add `TrafficEventCreate` schema |
| `routers/traffic.py` | Add traffic endpoint |
| `main.py` | Register traffic router |


## **11. Router Design**

### **Router file**

```
app/routers/traffic.py
```

### **Router prefix**

```python
router = APIRouter(
    prefix="/api/v1/traffic",
    tags=["traffic"]
)
```

### **Endpoint route**

```python
@router.post("/update")
def create_traffic_event(event: TrafficEventCreate, db: Session = Depends(get_db)):
    ...
```

This creates the final route:

```
POST /api/v1/traffic/update
```

---

## **12. Main Application Design**

The router needs to be registered in `main.py`.

```python
from app.routers import traffic

app.include_router(traffic.router)
```

This makes the traffic endpoint visible in Swagger at:

```
http://localhost:8000/docs
```

The existing backend already uses Swagger documentation at this URL when running locally.


## **13. Logging Design**

The endpoint should log every received traffic event during development.

### **Example log message**

```
Received traffic event: sensor=north_1, direction=north, phase=NS_RED, state=waiting_vehicle, valid=True
```

### **Reason for logging**

Logging helps during testing because I can check whether data from the embedded side actually reaches the backend. It also helps debug wrong JSON payloads or communication problems.


## **14. Error Handling Design**

### **Validation error**

If a required field is missing, FastAPI should automatically return a `422 Unprocessable Entity` response.

Example invalid request:

```json
{
  "sensorId": "north_1",
  "direction": "north",
  "phase": "NS_RED",
  "valid": true
}
```

This request is invalid because `interpretedState` and `timestampMs` are missing.

### **Database error**

If the database cannot save the event, the endpoint should return:

```json
{
  "detail": "Could not store traffic event"
}
```

With HTTP status:

```
500 Internal Server Error
```

This makes the API behaviour clearer during testing.


## **15. Local Test Design**

The endpoint will be tested with `curl` in PowerShell.

### **Test command**

```bash
curl -X POST "http://localhost:8000/api/v1/traffic/update" `
  -H "Content-Type: application/json" `
  -d "{\"sensorId\":\"north_1\",\"direction\":\"north\",\"phase\":\"NS_RED\",\"interpretedState\":\"waiting_vehicle\",\"timestampMs\":123456,\"valid\":true}"
```

### **Expected result**

```json
{
  "status": "received"
}
```

### **Test evidence for realisation**

For the realisation document, I should collect:

```
Screenshot of Docker containers running
Screenshot of Swagger endpoint
Screenshot of curl request
Screenshot of response
Screenshot or log showing the event was stored
```


## **16. Docker Design**

The backend will be tested inside Docker because the backend services are required to run in Docker.

The existing backend can be started with:

```bash
docker compose up --build
```

This starts the FastAPI backend and PostgreSQL database.

### **Designed local flow**

```
PowerShell curl request
→ localhost:8000
→ Docker API container
→ FastAPI traffic router
→ PostgreSQL database container
→ JSON response back to PowerShell
```


## **17. Raspberry Pi Connection Design**

The Raspberry Pi deployment is not implemented by me in this sprint. However, the endpoint is designed so it can later run in the same Docker-based backend environment on the Raspberry Pi.

### **Current local URL**

```
http://localhost:8000/api/v1/traffic/update
```

### **Future Raspberry Pi URL format**

```
http://<raspberry-pi-ip>:8000/api/v1/traffic/update
```

The embedded side only needs to replace `localhost` with the Raspberry Pi IP address once the backend is deployed there.


## **18. Design Decisions**

| Design decision | Reason |
| --- | --- |
| Create `POST /api/v1/traffic/update` | Clear endpoint for traffic simulation updates |
| Use JSON body | ESP32/simulation can send structured data through HTTP |
| Use Pydantic schema | Automatic validation of required fields |
| Store in `traffic_events` table | Traffic events have specific fields and should be stored clearly |
| Store invalid events too | Useful for debugging embedded/backend communication |
| Return `{ "status": "received" }` | Simple confirmation for embedded side |
| No traffic commands yet | Out of scope for Sprint 3 |
| Add router in `routers/traffic.py` | Keeps backend modular and consistent |
| Test through Docker | Matches backend project requirements |


## **19. Conclusion**

This design defines the backend structure for a traffic light simulation endpoint. The endpoint receives JSON data from the embedded side, validates it with Pydantic, stores it in a separate `traffic_events` table and returns a simple confirmation response.

The design keeps the implementation small and realistic for Sprint 3. It focuses on the foundation: receiving and storing traffic simulation events. More complex logic, such as traffic phase decisions or commands back to the ESP32, is intentionally left out for a later sprint.

The next step is to realise this design by implementing the schema, model, router and endpoint, then testing the endpoint through Docker with curl and Swagger or Postman.