from database import get_db
from models import Traffic
from schemas import TrafficCreate, TrafficEventResponse
from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session

router = APIRouter(
    tags=["traffic"],
    )

@router.post("")
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


@router.get("", response_model=list[TrafficEventResponse])
def get_traffic_events(
    limit: int = Query(default=20, le=200),
    db: Session = Depends(get_db)
):
    return db.query(Traffic).order_by(Traffic.created_at.desc()).limit(limit).all()


@router.get("/latest", response_model=list[TrafficEventResponse])
def get_latest_per_direction(
    db: Session = Depends(get_db)
):
    """Get the most recent event for each unique sensor_id+direction combo."""
    from sqlalchemy import func
    subq = (
        db.query(
            Traffic.sensor_id,
            Traffic.direction,
            func.max(Traffic.id).label("max_id")
        )
        .group_by(Traffic.sensor_id, Traffic.direction)
        .subquery()
    )
    results = (
        db.query(Traffic)
        .join(subq, Traffic.id == subq.c.max_id)
        .order_by(Traffic.sensor_id, Traffic.direction)
        .all()
    )
    return results
