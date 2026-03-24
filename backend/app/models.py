from sqlalchemy import CheckConstraint, Column, ForeignKey, Integer, Float, String, Boolean, DateTime, func
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

class Train(Base):
    __tablename__ = "train"

    id = Column(Integer, primary_key=True, index=True)
    is_approaching = Column(Boolean, default=True)
    first_sensor_time = Column(DateTime, nullable=True, default=func.now())
    second_sensor_time = Column(DateTime, nullable=True)
    predicted_arrival_seconds = Column(Float, default=0.0)
    updated_at = Column(DateTime(timezone=True), server_default=func.now(), onupdate=func.now())


class Barrier(Base):
    __tablename__ = "barrier"
    __table_args__ = (
        CheckConstraint(
            "input_mode IN ('manual', 'train')",
            name="check_input_mode"
        ),
    )
    
    id = Column(Integer, primary_key=True, index=True)
    is_closed = Column(Boolean, default=False)
    input_mode = Column(String, default="manual")
    train_id = Column(Integer)
    closed_at = Column(DateTime(timezone=True), server_default=func.now())
