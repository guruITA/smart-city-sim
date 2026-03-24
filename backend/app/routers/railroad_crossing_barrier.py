from datetime import datetime

from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session

from database import get_db
from models import Barrier
from schemas import BarrierCreate, BarrierResponse

router = APIRouter()


@router.get("/railroadcrossing/barrier", response_model=list[BarrierResponse])
def get_barrier_logs(
    limit: int = Query(default=50, ge=1, le=500, description="Max results"),
    db: Session = Depends(get_db),
):
    """
    Get recent Barrier sensor readings, ordered by most recent first.
    """
    query = db.query(Barrier).order_by(Barrier.created_at.desc())

    return query.limit(limit).all()

@router.get("/railroadcrossing/barrier/is-closed", response_model=bool)
def is_barrier_closed( db: Session = Depends(get_db)):
    """Return whether the barrier is currently closed based on latest log."""

    latest_log = (
        db.query(Barrier)
        .order_by(Barrier.created_at.desc())
        .first()
    )

    if not latest_log:
        raise HTTPException(status_code=404, detail="No barrier logs found")

    return latest_log.is_closed

@router.post("/railroadcrossing/barrier", response_model=BarrierResponse, status_code=201)
def create_barrier_log(barrier: BarrierCreate, db: Session = Depends(get_db)):
    """Create a new barrier log, optionally linked to a train."""

    db_barrier = Barrier(**barrier.model_dump())
    db.add(db_barrier)
    db.commit()
    db.refresh(db_barrier)
    
    return db_barrier