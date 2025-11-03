#!/bin/bash
# Setup development environment for V4 RTOS
#
# SPDX-License-Identifier: MIT OR Apache-2.0

set -e

echo "=== V4 RTOS Environment Setup ==="

# Check if ESP-IDF is installed
if [ -z "$IDF_PATH" ]; then
    echo "ESP-IDF not found. Installing..."

    # Check if esp directory exists
    if [ ! -d "$HOME/esp" ]; then
        mkdir -p "$HOME/esp"
    fi

    # Check if ESP-IDF is already cloned
    if [ ! -d "$HOME/esp/esp-idf" ]; then
        echo "Cloning ESP-IDF..."
        cd "$HOME/esp"
        git clone --recursive https://github.com/espressif/esp-idf.git
        cd esp-idf
        git checkout v5.1.2
    fi

    # Install ESP-IDF
    echo "Installing ESP-IDF for ESP32-C6..."
    cd "$HOME/esp/esp-idf"
    ./install.sh esp32c6

    echo ""
    echo "ESP-IDF installed successfully!"
    echo "To activate it, run:"
    echo "  source \$HOME/esp/esp-idf/export.sh"
    echo ""
    echo "Or add this to your ~/.bashrc or ~/.zshrc:"
    echo "  alias get_idf='. \$HOME/esp/esp-idf/export.sh'"
else
    echo "ESP-IDF found at: $IDF_PATH"
fi

# Check CMake
if ! command -v cmake &> /dev/null; then
    echo "CMake not found. Please install CMake 3.16 or later."
    exit 1
fi

echo "CMake found: $(cmake --version | head -1)"

# Check Python
if ! command -v python3 &> /dev/null; then
    echo "Python 3 not found. Please install Python 3.8 or later."
    exit 1
fi

echo "Python found: $(python3 --version)"

echo ""
echo "=== Environment Setup Complete ==="
echo ""
echo "Next steps:"
echo "  1. Source ESP-IDF: . \$HOME/esp/esp-idf/export.sh"
echo "  2. Build an example:"
echo "     cd bsp/esp32c6/examples/nanoc6/hello-rtos"
echo "     idf.py build"
echo "  3. Flash to board:"
echo "     idf.py flash monitor"
