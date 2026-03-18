from pydantic import BaseModel, Field
from datetime import datetime


# --- Sensor Readings ---

class ReadingCreate(BaseModel):
    """Schema for POST /api/v1/readings — sent by ESP32 devices."""

    tile: str = Field(..., example="parking", description="Which tile sent this reading")
    sensor_type: str = Field(..., example="sonar", description="Type of sensor")
    value: float = Field(..., example=12.5, description="Raw sensor value")
    unit: str = Field(default="", example="cm", description="Unit of measurement")


class ReadingResponse(BaseModel):
    id: int
    tile: str
    sensor_type: str
    value: float
    unit: str
    created_at: datetime

    model_config = {"from_attributes": True}


# --- Parking ---

class ParkingSpotResponse(BaseModel):
    spot_number: int
    is_occupied: bool
    distance_cm: float
    updated_at: datetime

    model_config = {"from_attributes": True}


class ParkingStatusResponse(BaseModel):
    total_spots: int
    occupied: int
    available: int
    spots: list[ParkingSpotResponse]
