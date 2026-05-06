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
    first_sensor_time = Column(DateTime(timezone=True), nullable=True, default=func.now())
    second_sensor_time = Column(DateTime(timezone=True), nullable=True)
    predicted_arrival_seconds = Column(Float, nullable=True)
    updated_at = Column(DateTime(timezone=True), server_default=func.now(), onupdate=func.now())


class SpeedReading(Base):
    """
    Speed camera readings from Gurpreet's tile.
    Stores each speed measurement with violation flag.
    """

    __tablename__ = "speed_readings"

    id = Column(Integer, primary_key=True, index=True)
    speed_kmh = Column(Float, nullable=False)  # measured speed in km/h
    direction = Column(String(10), nullable=False, default="1->2")  # sensor trigger order
    is_violation = Column(Boolean, default=False)  # exceeded speed limit
    speed_limit_kmh = Column(Float, default=1.0)  # limit at time of measurement
    created_at = Column(DateTime(timezone=True), server_default=func.now())


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
    train_id = Column(Integer, nullable=True)
    created_at = Column(DateTime(timezone=True), server_default=func.now())

class Traffic(Base):
    
    __tablename__ = "traffic"

    id = Column(Integer, primary_key=True, index=True)
    sensor_id = Column(String, nullable=False)
    direction = Column(String(10), nullable=False)
    phase = Column(String(20), nullable=False)
    interpreted_state = Column(String(20), nullable=False)
    timestamp_ms = Column(Integer, nullable=False)
    valid = Column(Boolean, nullable=False)
    created_at = Column(DateTime(timezone=True), server_default=func.now())