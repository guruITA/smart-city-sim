"""Backend override endpoints (Learning Goal 3 surprise feature).

The back-end requirement says it must be possible to override the decisions of
individual hubs from the backend, for example to set all traffic lights on a
road to red for an emergency vehicle. These endpoints let the backend force a
command on a tile. A tile polls GET /active for its target and obeys the forced
command instead of its own sensor logic, until the override is cleared.
"""
from datetime import datetime, timezone

from database import get_db
from models import Override
from schemas import OverrideCreate, OverrideResponse
from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session

router = APIRouter(tags=["override"])


@router.post("", response_model=OverrideResponse)
def set_override(override: OverrideCreate, db: Session = Depends(get_db)):
    """Force a command on a tile from the backend. Returns the active override."""
    record = Override(
        target=override.target,
        command=override.command,
        reason=override.reason,
        active=True,
    )
    db.add(record)
    db.commit()
    db.refresh(record)
    print(f"Override set: target={record.target}, command={record.command}, reason={record.reason}")
    return record


@router.get("/active", response_model=list[OverrideResponse])
def get_active_overrides(
    target: str | None = Query(default=None, description="Filter to one target, e.g. traffic"),
    db: Session = Depends(get_db),
):
    """Tiles poll this to learn which command the backend forces on them."""
    query = db.query(Override).filter(Override.active.is_(True))
    if target is not None:
        query = query.filter(Override.target == target)
    return query.order_by(Override.created_at.desc()).all()


@router.post("/{override_id}/clear", response_model=OverrideResponse)
def clear_override(override_id: int, db: Session = Depends(get_db)):
    """Clear one override so the tile returns to its own logic."""
    record = db.query(Override).filter(Override.id == override_id).first()
    if record is None:
        raise HTTPException(status_code=404, detail="Override not found")
    record.active = False
    record.cleared_at = datetime.now(timezone.utc)
    db.commit()
    db.refresh(record)
    print(f"Override cleared: id={record.id}, target={record.target}")
    return record


@router.get("", response_model=list[OverrideResponse])
def get_override_history(
    limit: int = Query(default=20, le=200),
    db: Session = Depends(get_db),
):
    """Full override history, newest first, for the dashboard and audit."""
    return db.query(Override).order_by(Override.created_at.desc()).limit(limit).all()


@router.post("/emergency", response_model=OverrideResponse)
def emergency_corridor(db: Session = Depends(get_db)):
    """One call for an emergency vehicle: force every traffic light to red.

    This is the surprise in action. It clears any earlier traffic override, then
    sets a single city-wide override that all traffic light tiles obey at once.
    """
    # Clear earlier active traffic overrides so only one is in force.
    earlier = db.query(Override).filter(
        Override.active.is_(True), Override.target == "traffic"
    ).all()
    for record in earlier:
        record.active = False
        record.cleared_at = datetime.now(timezone.utc)

    emergency = Override(
        target="traffic",
        command="all_red",
        reason="emergency_vehicle",
        active=True,
    )
    db.add(emergency)
    db.commit()
    db.refresh(emergency)
    print("EMERGENCY override: all traffic lights forced to red")
    return emergency
