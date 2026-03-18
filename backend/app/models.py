from sqlalchemy import Column, Integer, Float, String, Boolean, DateTime, func
from database import Base


class SensorReading(Base):
    """
    Generic sensor reading table.
    Every tile (parking, streetlight, traffic light, railroad crossing)
    sends data here via POST /api/v1/readings.
    """

    __tablename__ = "sensor_readings"

    id = Column(Integer, primary_key=True, index=True)
    tile = Column(String(50), nullable=False, index=True)  # parking, streetlight, etc.
    sensor_type = Column(String(50), nullable=False)  # sonar, ldr, ir, button
    value = Column(Float, nullable=False)  # raw sensor value
    unit = Column(String(20), default="")  # cm, lux, bool, ms
    created_at = Column(DateTime(timezone=True), server_default=func.now())


class ParkingSpot(Base):
    """
    Realtime status per parking spot.
    Updated every time a new sonar reading comes in.
    """

    __tablename__ = "parking_spots"

    id = Column(Integer, primary_key=True, index=True)
    spot_number = Column(Integer, unique=True, nullable=False)  # 1, 2, 3, ...
    is_occupied = Column(Boolean, default=False)
    distance_cm = Column(Float, default=0.0)  # last measured distance
    updated_at = Column(DateTime(timezone=True), server_default=func.now(), onupdate=func.now())
