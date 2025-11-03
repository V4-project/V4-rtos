#!/bin/bash
# Build all V4 RTOS components
#
# SPDX-License-Identifier: MIT OR Apache-2.0

set -e

echo "=== Building V4 RTOS Components ==="

# Build native components (kernel, HAL, compiler, etc.)
echo ""
echo "Building native components..."
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

echo ""
echo "=== Build Complete ==="
echo ""
echo "To build ESP32-C6 examples:"
echo "  cd bsp/esp32c6/examples/nanoc6/hello-rtos"
echo "  idf.py build"
