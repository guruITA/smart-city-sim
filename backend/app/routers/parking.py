from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

from database import get_db
from models import ParkingSpot
from schemas import ParkingSpotResponse, ParkingStatusResponse

router = APIRouter()

# Distance threshold in cm: if object is closer than this, spot is occupied
OCCUPIED_THRESHOLD_CM = 10.0


@router.get("/status", response_model=ParkingStatusResponse)
def get_parking_status(db: Session = Depends(get_db)):
    """
    Get realtime parking status: how many spots are free/occupied.
    This is what the dashboard and OLED display use.
    """
    spots = db.query(ParkingSpot).order_by(ParkingSpot.spot_number).all()
    occupied = sum(1 for s in spots if s.is_occupied)

    return ParkingStatusResponse(
        total_spots=len(spots),
        occupied=occupied,
        available=len(spots) - occupied,
        spots=spots,
    )


@router.post("/update/{spot_number}", response_model=ParkingSpotResponse)
def update_spot(spot_number: int, distance_cm: float, db: Session = Depends(get_db)):
    """
    Update a parking spot with a new sonar distance reading.

    The ESP32 calls this after measuring the distance with the ultrasonic sensor.
    If distance < threshold (10 cm), spot is marked as occupied.

    Also logs the reading to the generic sensor_readings table.
    """
    # Find or create the parking spot
    spot = db.query(ParkingSpot).filter(ParkingSpot.spot_number == spot_number).first()

    if not spot:
        spot = ParkingSpot(spot_number=spot_number)
        db.add(spot)

    # Update status based on distance
    spot.is_occupied = distance_cm < OCCUPIED_THRESHOLD_CM

    db.commit()
    db.refresh(spot)
    return spot


@router.get("/spots", response_model=list[ParkingSpotResponse])
def get_all_spots(db: Session = Depends(get_db)):
    """Get all parking spots and their current status."""
    return db.query(ParkingSpot).order_by(ParkingSpot.spot_number).all()


@router.get("/spots/{spot_number}", response_model=ParkingSpotResponse)
def get_spot(spot_number: int, db: Session = Depends(get_db)):
    """Get status of a specific parking spot."""
    spot = db.query(ParkingSpot).filter(ParkingSpot.spot_number == spot_number).first()
    if not spot:
        raise HTTPException(status_code=404, detail=f"Spot {spot_number} not found")
    return spot
