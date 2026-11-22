#!/usr/bin/env bash
# Build and start all services; Ctrl+C stops them all.
set -euo pipefail
cd "$(dirname "$0")/.."

cmake -S . -B build
cmake --build build -j"$(nproc)"

pids=()
cleanup() {
  echo "Stopping services..."
  kill "${pids[@]}" 2>/dev/null || true
  wait 2>/dev/null || true
}
trap cleanup INT TERM EXIT

# Start order matters: servers first, then clients.
./build/services/motor_bridge/motor_bridge &        pids+=($!)
./build/services/decision_service/decision_service & pids+=($!)
sleep 0.2
./build/services/camera_service/camera_service &    pids+=($!)

wait
