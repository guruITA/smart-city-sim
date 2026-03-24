from datetime import datetime

from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session

from database import get_db
from models import Train
from schemas import TrainResponse, TrainSecondSensorUpdate

router = APIRouter()


@router.get("/railroadcrossing/train", response_model=list[TrainResponse])
def get_trains(
    limit: int = Query(default=50, ge=1, le=500, description="Max results"),
    db: Session = Depends(get_db),
):
    """
    Get recent train sensor readings, ordered by most recent first.
    """
    query = db.query(Train).order_by(Train.updated_at.desc())

    return query.limit(limit).all()

@router.get("/railroadcrossing/train/approaching", response_model=list[TrainResponse])
def get_approaching_trains(
    limit: int = Query(default=50, ge=1, le=500, description="Max results"),
    db: Session = Depends(get_db),
):
    """
    Get recent approaching trains, ordered by most recent first.
    """
    query = db.query(Train).filter(Train.is_approaching == True).order_by(Train.updated_at.desc())

    return query.limit(limit).all()

@router.post("/railroadcrossing/train/first", response_model= TrainResponse, status_code=201)
def create_train_first_sensor( db: Session = Depends(get_db)):
    """Create a new train reading."""
    db_train = Train()
    db.add(db_train)
    db.commit()
    db.refresh(db_train)
    return db_train

from datetime import datetime, timezone
from fastapi import HTTPException

@router.put("/railroadcrossing/train/{id}/second", response_model=TrainResponse)
def update_train_second_sensor(
    id: int,
    data: TrainSecondSensorUpdate,
    db: Session = Depends(get_db)
):
    """Update second sensor time and predicted arrival."""

    db_train = db.query(Train).filter(Train.id == id).first()

    if not db_train:
        raise HTTPException(status_code=404, detail="Train not found")

    db_train.second_sensor_time = datetime.now(timezone.utc)
    db_train.predicted_arrival_seconds = data.predicted_arrival_seconds

    db.commit()
    db.refresh(db_train)

    return db_train

@router.put("/railroadcrossing/train/{id}/crossed", response_model=TrainResponse)
def update_train_crossed(
    id: int,
    db: Session = Depends(get_db)
):
    """Update is approaching to false"""

    db_train = db.query(Train).filter(Train.id == id).first()

    if not db_train:
        raise HTTPException(status_code=404, detail="Train not found")

    db_train.is_approaching = False
   

    db.commit()
    db.refresh(db_train)

    return db_train
