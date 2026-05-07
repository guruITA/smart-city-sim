#!/bin/bash
# Deploy City Sim backend on Raspberry Pi
# Usage: ssh into Pi, clone repo, then run this script from backend/
set -e

echo "=== City Sim Deploy ==="

# Check if .env exists
if [ ! -f .env ]; then
    echo "Creating .env from .env.example..."
    cp .env.example .env
fi

# Build and start containers
echo "Building and starting containers..."
docker compose up --build -d

# Wait for health check
echo "Waiting for API to start..."
for i in {1..30}; do
    if curl -sf http://localhost:80/health > /dev/null 2>&1; then
        echo ""
        echo "=== API is live! ==="
        echo "API:       http://$(hostname -I | awk '{print $1}')/api/v1"
        echo "Dashboard: http://$(hostname -I | awk '{print $1}')/"
        echo "Docs:      http://$(hostname -I | awk '{print $1}')/docs"
        exit 0
    fi
    printf "."
    sleep 2
done

echo ""
echo "ERROR: API did not start in time. Check logs:"
echo "  docker compose logs api"
exit 1
