from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
from contextlib import asynccontextmanager

from database import engine
from models import Base
from routers import readings, parking, railroad_crossing


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
app.include_router(railroad_crossing.router, prefix="/api/v1/railroadcrossing", tags=["railroad crossing"])

@app.get("/")
def root():
    return {
        "project": "City Sim - The Embedded Alliance",
        "version": "0.1.0",
        "docs": "/docs",
        "tiles": ["parking", "streetlight", "trafficLight", "railroadCrossing"],
    }


@app.get("/health")
def health():
    return {"status": "ok"}
