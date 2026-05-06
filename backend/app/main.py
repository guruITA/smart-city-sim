import os
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from fastapi.responses import HTMLResponse
from contextlib import asynccontextmanager

from database import engine
from models import Base
from routers import readings, parking, railroad_crossing_train, railroad_crossing_barrier, eink_display, speed_camera


@asynccontextmanager
async def lifespan(app: FastAPI):
    # Create all tables on startup
    Base.metadata.create_all(bind=engine)
    print("Database tables created")
    yield


app = FastAPI(
    title="City Sim API",
    description="Shared backend for The Embedded Alliance smart city sensor data",
    version="0.1.0",
    lifespan=lifespan,
)

# Allow ESP32 and dashboard to connect from any origin
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

# Register routers
app.include_router(readings.router, prefix="/api/v1", tags=["readings"])
app.include_router(parking.router, prefix="/api/v1/parking", tags=["parking"])
app.include_router(railroad_crossing_train.router, prefix="/api/v1/railroadcrossing/train", tags=["railroad crossing - train"])
app.include_router(railroad_crossing_barrier.router, prefix="/api/v1/railroadcrossing/barrier", tags=["railroad crossing - barrier"])
app.include_router(eink_display.router, prefix="/api/v1", tags=["eink display"])


# Serve dashboard HTML at root
static_dir = os.path.join(os.path.dirname(__file__), "static")


@app.get("/", response_class=HTMLResponse)
def root():
    with open(os.path.join(static_dir, "index.html")) as f:
        return f.read()


@app.get("/health")
def health():
    return {"status": "ok"}
