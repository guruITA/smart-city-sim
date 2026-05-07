from fastapi import APIRouter, Depends, Query
from sqlalchemy.orm import Session

from database import get_db
from models import SensorReading
from schemas import ReadingCreate, ReadingResponse

router = APIRouter()


@router.post("/readings", response_model=ReadingResponse, status_code=201)
def create_reading(reading: ReadingCreate, db: Session = Depends(get_db)):
    """
    Receive a sensor reading from any ESP32 tile.

    The ESP32 sends a JSON POST with tile name, sensor type, value, and unit.
    This is the main endpoint all team members use to send their data.
    """
    db_reading = SensorReading(**reading.model_dump())
    db.add(db_reading)
    db.commit()
    db.refresh(db_reading)
    return db_reading


@router.get("/readings", response_model=list[ReadingResponse])
def get_readings(
    tile: str | None = Query(default=None, description="Filter by tile name"),
    sensor_type: str | None = Query(default=None, description="Filter by sensor type"),
    limit: int = Query(default=50, ge=1, le=500, description="Max results"),
    db: Session = Depends(get_db),
):
    """
    Get sensor readings, optionally filtered by tile and/or sensor type.

    Examples:
    - GET /api/v1/readings?tile=parking&limit=10
    - GET /api/v1/readings?tile=streetlight&sensor_type=ldr
    """
    query = db.query(SensorReading).order_by(SensorReading.created_at.desc())

    if tile:
        query = query.filter(SensorReading.tile == tile)
    if sensor_type:
        query = query.filter(SensorReading.sensor_type == sensor_type)

    return query.limit(limit).all()


@router.get("/readings/latest/{tile}", response_model=ReadingResponse | None)
def get_latest_reading(tile: str, db: Session = Depends(get_db)):
    """Get the most recent reading for a specific tile."""
    return (
        db.query(SensorReading)
        .filter(SensorReading.tile == tile)
        .order_by(SensorReading.created_at.desc())
        .first()
    )
