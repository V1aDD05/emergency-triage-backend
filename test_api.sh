## Скрипт для ручного тестирования HTTP API
#!/bin/bash

BASE_URL="http://localhost:8080"

## Ручное тестирование обработчика 'POST /patients'
echo "=== POST /patients (valid) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"mask":13, "age":42, "sex":"male"}' | jq '.'
echo ""

echo "=== POST /patients (missing mask) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"age":42, "sex":"male"}' | jq '.'
echo ""

echo "=== POST /patients (negative mask) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"mask":-13, "age":42, "sex":"male"}' | jq '.'
echo ""

echo "=== POST /patients (mask out of bounds uint32_t) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"mask":4294967296, "age":42, "sex":"male"}' | jq '.'
echo ""

echo "=== POST /patients (mask is not an integer) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"mask":"abc", "age":42, "sex":"male"}' | jq '.'
echo ""

echo "=== POST /patients (negative age) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"mask":13, "age":-42, "sex":"male"}' | jq '.'
echo ""

echo "=== POST /patients (age out of bounds uint8_t) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"mask":13, "age":256, "sex":"male"}' | jq '.'
echo ""

echo "=== POST /patients (age is not an integer) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"mask":13, "age":"abc", "sex":"male"}' | jq '.'
echo ""

echo "=== POST /patients (invalid sex) ==="
curl -sS -X POST "$BASE_URL/patients" \
    -H "Content-Type: application/json" \
    -d '{"mask":13, "age":42, "sex":"alien"}' | jq '.'
echo ""

## Ручное тестирование обработчика 'GET /patients/{id}'
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


## Ручное тестирование обработчика 'GET /patients'
echo "=== GET /patients ==="
curl -sS -X GET "$BASE_URL/patients" | jq '.'
echo ""