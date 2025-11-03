#!/bin/bash
# Flash ESP32-C6 example to board
#
# SPDX-License-Identifier: MIT OR Apache-2.0

set -e

# Default example
EXAMPLE=${1:-hello-rtos}
BOARD=${2:-nanoc6}

EXAMPLE_PATH="bsp/esp32c6/examples/$BOARD/$EXAMPLE"

if [ ! -d "$EXAMPLE_PATH" ]; then
    echo "Error: Example not found: $EXAMPLE_PATH"
    echo ""
    echo "Usage: $0 [example] [board]"
    echo ""
    echo "Available examples for $BOARD:"
    ls -1 "bsp/esp32c6/examples/$BOARD/" 2>/dev/null || echo "  (none)"
    exit 1
fi

echo "=== Flashing $EXAMPLE to ESP32-C6 $BOARD ==="

cd "$EXAMPLE_PATH"

# Build if needed
if [ ! -d "build" ]; then
    echo "Building first..."
    idf.py build
fi

# Flash
echo "Flashing..."
idf.py flash

echo ""
echo "=== Flash Complete ==="
echo ""
echo "To monitor output:"
echo "  cd $EXAMPLE_PATH"
echo "  idf.py monitor"
