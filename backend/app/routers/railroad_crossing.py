from fastapi import APIRouter, Depends, Query
from sqlalchemy.orm import Session

from database import get_db
from models import Train
from schemas import TrainCreate, TrainResponse

router = APIRouter()


@router.post("/railroadcrossing/trains", response_model= TrainResponse, status_code=201)
def create_train(train: TrainCreate, db: Session = Depends(get_db)):
    """Create a new train reading."""
    db_reading = Train(**train.model_dump())
    db.add(db_reading)
    db.commit()
    db.refresh(db_reading)
    return db_reading


@router.get("/railroadcrossing/trains", response_model=list[TrainResponse])
def get_trains(
    limit: int = Query(default=50, ge=1, le=500, description="Max results"),
    db: Session = Depends(get_db),
):
    """
    Get recent train sensor readings, ordered by most recent first.
    """
    query = db.query(Train).order_by(Train.updated_at.desc())

    return query.limit(limit).all()
