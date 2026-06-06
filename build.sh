#!/bin/bash
set -e
cmake --preset gcc-debug
cmake --build --preset build-debug
cd build && ctest --output-on-failure