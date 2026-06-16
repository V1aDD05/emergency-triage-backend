#!/usr/bin/env bash
set -e

echo "Starting server..."
./build/hospital_triage &
SERVER_PID=$!

trap "kill $SERVER_PID 2>/dev/null" EXIT

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
