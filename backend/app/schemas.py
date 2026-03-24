from typing import Optional

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

# --- Rail road crossing ---

class TrainSecondSensorUpdate(BaseModel):
    predicted_arrival_seconds: float
class TrainResponse(BaseModel):
    id: int
    is_approaching: bool
    first_sensor_time: datetime
    second_sensor_time: datetime | None
    predicted_arrival_seconds: float | None
    updated_at: datetime

class BarrierResponse(BaseModel):
    is_closed: bool
    input_mode: str
    train_id:  int | None
    created_at: datetime

class BarrierCreate(BaseModel):
    input_mode: str = Field(..., example="manual", description="Type of input (manual or train)")
    is_closed: bool = Field(..., example=True, description="Barrier closing or opening")
    train_id: Optional[int] = Field(None, example=123, description="Associated train ID (if input_mode is train)")