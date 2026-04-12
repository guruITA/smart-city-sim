# Raspberry Pi Deployment Guide

## Overview

The Raspberry Pi serves as the central server for the City Sim project. It runs the shared backend (FastAPI + PostgreSQL) in Docker containers, accessible by all ESP32 devices and browsers on the HvA network.

```
ESP32 tiles (all team members)  ──┐
                                  ├──>  http://145.92.8.137:8000  ──>  PostgreSQL
Browsers (dashboard)            ──┘         Raspberry Pi 4
```

## Network Configuration

| Setting | Value |
|---------|-------|
| IP address | `145.92.8.137/24` |
| Gateway | `145.92.8.1` |
| DNS | `145.18.145.18` |
| Network | HvA campus (wired ethernet) |

The static IP is configured in `/boot/dhcpcd.conf` on the SD card:

```
interface eth0
static ip_address=145.92.8.137/24
static routers=145.92.8.1
static domain_name_servers=145.18.145.18
```

## SD Card Setup

The following was configured on the boot partition before first boot:

1. **SSH enabled** — created empty `/boot/ssh` file so the Pi accepts remote connections
2. **Static IP set** — updated `/boot/dhcpcd.conf` with the HvA-assigned IP address

## Deploying the Backend

### 1. SSH into the Pi

```bash
ssh pi@145.92.8.137
# user: walt
# password: matin 
```

### 2. Install Docker (first time only)

```bash
curl -fsSL https://get.docker.com | sh
sudo usermod -aG docker $USER
logout
# Log back in for group change to take effect
ssh pi@145.92.8.137
```

### 3. Clone the repository

```bash
git clone https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group.git
cd city-the-embedded-alliance-city-sim-learning-group/backend
```

### 4. Deploy

```bash
bash deploy.sh
```

This script:
- Creates `.env` from `.env.example` if it doesn't exist
- Builds and starts the Docker containers
- Waits for the health check to pass
- Prints the access URLs

### 5. Verify

```bash
# Health check
curl http://145.92.8.137:8000/health

# Parking status
curl http://145.92.8.137:8000/api/v1/parking/status

# Test a parking reading
curl -X POST "http://145.92.8.137:8000/api/v1/parking/update/1?distance_cm=5.2"
```

## Access Points

| Service | URL |
|---------|-----|
| API root | `http://145.92.8.137:8000` |
| Swagger docs | `http://145.92.8.137:8000/docs` |
| Dashboard | `http://145.92.8.137:8000/dashboard` |
| Health check | `http://145.92.8.137:8000/health` |

## ESP32 Configuration

Each team member must update their ESP32 sketch to point to the Pi:

```cpp
const char* API_BASE_URL = "http://145.92.8.137:8000";
```

### WiFi requirements
- ESP32 must be connected to the HvA network
- The Pi and ESP32 must be on the same subnet (`145.92.8.x`)

## Maintenance

### View logs
```bash
cd city-the-embedded-alliance-city-sim-learning-group/backend
docker compose logs -f api    # API logs
docker compose logs -f db     # Database logs
```

### Restart after code changes
```bash
cd city-the-embedded-alliance-city-sim-learning-group/backend
git pull origin main
docker compose up --build -d
```

### Reboot survival
Docker containers are configured with `restart: always`, so they automatically start after a Pi reboot. No manual intervention needed.

### Check container status
```bash
docker ps
```

## Troubleshooting

| Problem | Solution |
|---------|----------|
| Cannot SSH to Pi | Check ethernet cable, verify IP with `ping 145.92.8.137` |
| API not responding | Run `docker compose logs api` to check errors |
| Database connection error | Run `docker compose logs db`, check if PostgreSQL is healthy |
| ESP32 cannot reach API | Verify ESP32 is on HvA WiFi and can ping `145.92.8.137` |
| Containers not starting after reboot | Run `docker compose up -d` manually |
