from datetime import datetime

from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session

from database import get_db
from models import Barrier, Train
from schemas import BarrierCreate, BarrierResponse

router = APIRouter()

@router.get("/", response_model=list[BarrierResponse])
def get_barrier_logs(
    limit: int = Query(default=50, ge=1, le=500, description="Max results"),
    db: Session = Depends(get_db),
):
    """
    Get recent Barrier sensor readings, ordered by most recent first.
    """
    query = db.query(Barrier).order_by(Barrier.created_at.desc())

    return query.limit(limit).all()

@router.get("/is-closed", response_model=bool)
def is_barrier_closed(db: Session = Depends(get_db)):
    """
    Return whether the barrier is currently closed based on latest log.
    Falls back to train status if no barrier logs exist.
    """

    latest_log = (
        db.query(Barrier)
        .order_by(Barrier.created_at.desc())
        .first()
    )

    if latest_log:
        return latest_log.is_closed
    
    approaching_train = (
        db.query(Train)
        .filter(Train.is_approaching == True)
        .order_by(Train.updated_at.desc())
        .first()
    )

    if approaching_train:
        return True  # barrier moet dicht

    return False  # barrier open

@router.post("/", response_model=BarrierResponse, status_code=201)
def create_barrier_log(barrier: BarrierCreate, db: Session = Depends(get_db)):
    """Create a new barrier log, optionally linked to a train."""

    db_barrier = Barrier(**barrier.model_dump())
    db.add(db_barrier)
    db.commit()
    db.refresh(db_barrier)
    
    return db_barrier