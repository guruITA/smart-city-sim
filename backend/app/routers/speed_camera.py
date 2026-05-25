from fastapi import APIRouter, Depends, Query
from sqlalchemy.orm import Session
from sqlalchemy import func as sql_func
from datetime import datetime

from database import get_db
from models import SpeedReading
from schemas import (
    SpeedReadingCreate,
    SpeedReadingResponse,
    SpeedCameraStatsResponse,
    CameraRegisterRequest,
    CameraInfoResponse,
)

router = APIRouter()


latest_camera: CameraInfoResponse | None = None


@router.post("/", response_model=SpeedReadingResponse, status_code=201)
def create_speed_reading(
    data: SpeedReadingCreate,
    db: Session = Depends(get_db),
):
    """Store a new speed camera measurement from the ESP32."""
    reading = SpeedReading(
        speed_kmh=data.speed_kmh,
        direction=data.direction,
        is_violation=data.is_violation,
        speed_limit_kmh=data.speed_limit_kmh,
    )
    db.add(reading)
    db.commit()
    db.refresh(reading)
    return reading


@router.get("/", response_model=list[SpeedReadingResponse])
def get_speed_readings(
    limit: int = Query(default=50, ge=1, le=500, description="Max results"),
    db: Session = Depends(get_db),
):
    """Get recent speed camera readings, ordered by most recent first."""
    return (
        db.query(SpeedReading)
        .order_by(SpeedReading.created_at.desc())
        .limit(limit)
        .all()
    )


@router.get("/violations", response_model=list[SpeedReadingResponse])
def get_violations(
    limit: int = Query(default=50, ge=1, le=500, description="Max results"),
    db: Session = Depends(get_db),
):
    """Get only speed limit violations."""
    return (
        db.query(SpeedReading)
        .filter(SpeedReading.is_violation == True)
        .order_by(SpeedReading.created_at.desc())
        .limit(limit)
        .all()
    )


@router.get("/latest", response_model=SpeedReadingResponse | None)
def get_latest_reading(db: Session = Depends(get_db)):
    """Get the most recent speed camera reading."""
    return (
        db.query(SpeedReading)
        .order_by(SpeedReading.created_at.desc())
        .first()
    )


@router.get("/stats", response_model=SpeedCameraStatsResponse)
def get_speed_stats(db: Session = Depends(get_db)):
    """Get speed camera statistics: total readings, violations, averages."""
    total = db.query(sql_func.count(SpeedReading.id)).scalar() or 0
    violations = (
        db.query(sql_func.count(SpeedReading.id))
        .filter(SpeedReading.is_violation == True)
        .scalar()
        or 0
    )
    avg_speed = db.query(sql_func.avg(SpeedReading.speed_kmh)).scalar() or 0.0
    max_speed = db.query(sql_func.max(SpeedReading.speed_kmh)).scalar() or 0.0

    return SpeedCameraStatsResponse(
        total_readings=total,
        total_violations=violations,
        average_speed_kmh=round(avg_speed, 2),
        max_speed_kmh=round(max_speed, 2),
        violation_rate_percent=round((violations / total * 100) if total > 0 else 0, 1),
    )


@router.post("/camera/register", response_model=CameraInfoResponse)
def register_camera(data: CameraRegisterRequest):
    """Register the current IP address of the ESP32-CAM."""
    global latest_camera

    latest_camera = CameraInfoResponse(
        camera_id=data.camera_id,
        ip_address=data.ip_address,
        capture_url=f"http://{data.ip_address}/capture",
        updated_at=datetime.utcnow(),
    )

    return latest_camera


@router.get("/camera/latest", response_model=CameraInfoResponse | None)
def get_latest_camera():
    """Get the latest registered ESP32-CAM IP address."""
    return latest_camera