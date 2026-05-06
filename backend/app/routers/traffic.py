from backend.app.database import get_db
from backend.app.models import Traffic
from backend.app.schemas import TrafficCreate
from fastapi import APIRouter, Depends, HTTPException
from sqlalchemy.orm import Session

router = APIRouter(
    prefix="/api/v1/traffic",
    tags=["traffic"],
    )

@router.post("/update")
def create_traffic_event(
    event: TrafficCreate,
    db: Session = Depends(get_db)
):
    try:
        traffic_event = Traffic(
            sensor_id=event.sensorId,
            direction=event.direction,
            phase=event.phase,
            interpreted_state=event.interpretedState,
            timestamp_ms=event.timestampMs,
            valid=event.valid,
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

    except Exception:
        db.rollback()
        raise HTTPException(
            status_code=500,
            detail="Could not store traffic event"
        )
