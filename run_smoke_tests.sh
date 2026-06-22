#!/usr/bin/env bash
set -e

HOST="${SERVER_HOST:-127.0.0.1}"
PORT="${SERVER_PORT:-8080}"
BASE_URL="http://$HOST:$PORT"

echo "Checking if server is already working..."
if curl -s -o /dev/null -w "%{http_code}" "$BASE_URL/patients" | grep -q "200\|404"; then
    echo "ERROR: server is already running on port $PORT. Please stop it before runnung tests."
    exit 1
fi

echo "Port is free, starting server..."
./build/app/hospital_triage &
SERVER_PID=$!

trap "kill $SERVER_PID 2>/dev/null || true" EXIT

echo "Waiting for server to start..."
TIMEOUT=10
while ! curl -s http://localhost:8080/patients > /dev/null; do
    sleep 1
    TIMEOUT=$((TIMEOUT - 1))
    if [ $TIMEOUT -eq 0 ]; then
        echo "Server failed to start within 10 seconds"
        exit 1
    fi
done
echo "Server is ready."

echo "Running API tests..."
./tests/scripts/test_api.sh

echo "API tests completed successfully."
