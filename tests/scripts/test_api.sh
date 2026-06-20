#!/usr/bin/env bash

HOST="${SERVER_HOST:-127.0.0.1}"
PORT="${SERVER_PORT:-8080}"
BASE_URL="http://$HOST:$PORT"

## Manual testing 'GET /patients' before adding any patient
echo "=== GET /patients ==="
curl -sS -X GET "$BASE_URL/patients" | jq '.'
echo ""

## Manual testing 'POST /patients'
echo "=== POST /patients (valid) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{
        "emergency_params": {
            "is_bleeding": false,
            "is_extensive_wounds": false,
            "is_penetrating_wounds": false,
            "is_consciousness_depression": false,
            "is_respiratory_depression": false,
            "is_hemodynamic_depression": false,
            "is_severe_combined_injury": false
        },
        "triage_data": {
            "eye_response": 2,
            "verbal_response": 2,
            "motor_response": 2,
            "respiratory_rate": 25,
            "systolic_bp": 120
        },
        "demography_data": {
            "age": 32,
            "sex": "male"
        }
    }' | jq '.'
echo ""

echo "=== POST /patients (negative age) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{
        "emergency_params": {
            "is_bleeding": false,
            "is_extensive_wounds": false,
            "is_penetrating_wounds": false,
            "is_consciousness_depression": false,
            "is_respiratory_depression": false,
            "is_hemodynamic_depression": false,
            "is_severe_combined_injury": false
        },
        "triage_data": {
            "eye_response": 2,
            "verbal_response": 2,
            "motor_response": 2,
            "respiratory_rate": 25,
            "systolic_bp": 120
        },
        "demography_data": {
            "age": -32,
            "sex": "male"
        }
    }' | jq '.'
echo ""

echo "=== POST /patients (age out of bounds uint8_t) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{
        "emergency_params": {
            "is_bleeding": false,
            "is_extensive_wounds": false,
            "is_penetrating_wounds": false,
            "is_consciousness_depression": false,
            "is_respiratory_depression": false,
            "is_hemodynamic_depression": false,
            "is_severe_combined_injury": false
        },
        "triage_data": {
            "eye_response": 2,
            "verbal_response": 2,
            "motor_response": 2,
            "respiratory_rate": 25,
            "systolic_bp": 120
        },
        "demography_data": {
            "age": 256,
            "sex": "male"
        }
    }' | jq '.'
echo ""

echo "=== POST /patients (age is not an integer) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{
        "emergency_params": {
            "is_bleeding": false,
            "is_extensive_wounds": false,
            "is_penetrating_wounds": false,
            "is_consciousness_depression": false,
            "is_respiratory_depression": false,
            "is_hemodynamic_depression": false,
            "is_severe_combined_injury": false
        },
        "triage_data": {
            "eye_response": 2,
            "verbal_response": 2,
            "motor_response": 2,
            "respiratory_rate": 25,
            "systolic_bp": 120
        },
        "demography_data": {
            "age": "abc",
            "sex": "male"
        }
    }' | jq '.'
echo ""

echo "=== POST /patients (invalid sex) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{
        "emergency_params": {
            "is_bleeding": false,
            "is_extensive_wounds": false,
            "is_penetrating_wounds": false,
            "is_consciousness_depression": false,
            "is_respiratory_depression": false,
            "is_hemodynamic_depression": false,
            "is_severe_combined_injury": false
        },
        "triage_data": {
            "eye_response": 2,
            "verbal_response": 2,
            "motor_response": 2,
            "respiratory_rate": 25,
            "systolic_bp": 120
        },
        "demography_data": {
            "age": 32,
            "sex": "alien"
        }
    }' | jq '.'
echo ""

## Manual testing 'GET /patients/{id}'
echo "=== GET /patients/1 ==="
curl -sS -X GET "$BASE_URL/patients/1" | jq '.'
echo ""

echo "=== GET /patients/2 (no patient with such id) ==="
curl -sS -X GET "$BASE_URL/patients/2" | jq '.'
echo ""

echo "=== GET /patients/-1 (invalid id) ==="
curl -sS -X GET "$BASE_URL/patients/-1" | jq '.'
echo ""

echo "=== GET /patients/abc (id is not an integer) ==="
curl -sS -X GET "$BASE_URL/patients/abc" | jq '.'
echo ""

echo "=== GET /patients/4294967296 (id out of bound uint32_t) ==="
curl -sS -X GET "$BASE_URL/patients/4294967296" | jq '.'
echo ""


## Manual testing 'GET /patients'
echo "=== GET /patients ==="
curl -sS -X GET "$BASE_URL/patients" | jq '.'
echo ""
