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


# Emergency state forced on every hub by one call. The backend overrules the
# whole city at once, not only the traffic lights. The command words are what each
# tile firmware polls for and obeys; they are easy to tune per tile.
EMERGENCY_COMMANDS = {
    "traffic": "all_red",       # stop all traffic
    "barrier": "close",         # lock the railroad crossing to a safe state
    "streetlight": "force_on",  # full lighting for visibility
    "parking": "full",          # mark the lot full so no one enters
    "eink": "show_alert",       # display an emergency message
}


@router.post("/emergency", response_model=list[OverrideResponse])
def emergency_corridor(db: Session = Depends(get_db)):
    """One call: put the WHOLE city into emergency mode for an emergency vehicle.

    The backend overrules every hub at once, not only the traffic lights. For each
    target it clears any earlier active override, then sets the emergency command,
    so the entire city responds to a single button.
    """
    created: list[Override] = []
    for target, command in EMERGENCY_COMMANDS.items():
        # Clear earlier active overrides for this target so only the emergency is in force.
        earlier = db.query(Override).filter(
            Override.active.is_(True), Override.target == target
        ).all()
        for record in earlier:
            record.active = False
            record.cleared_at = datetime.now(timezone.utc)

        override = Override(
            target=target,
            command=command,
            reason="emergency_vehicle",
            active=True,
        )
        db.add(override)
        created.append(override)

    db.commit()
    for override in created:
        db.refresh(override)
    print(f"EMERGENCY override: whole city forced to emergency state ({len(created)} hubs)")
    return created
