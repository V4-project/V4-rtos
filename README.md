# V4 Runtime

**Embedded Forth Runtime Environment**

[![CI](https://github.com/V4-project/V4-runtime/workflows/CI/badge.svg)](https://github.com/V4-project/V4-runtime/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![License: Apache 2.0](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

V4 Runtime is a lightweight Forth runtime environment built on FreeRTOS for resource-constrained microcontrollers, featuring interactive Forth development and multitasking capabilities.

## Features

### Core (Required)

- **FreeRTOS Backend** - Leverages proven FreeRTOS scheduler for multitasking
- **V4 VM Integration** - Forth bytecode execution with task support
- **Message Passing** - Inter-task communication with 16-message queue
- **Hardware Abstraction** - Unified HAL across platforms

### Optional Components

- **Interactive REPL** - Live Forth programming on device (via V4 VM)
- **V4-link Protocol** - Bytecode transfer over USB Serial/JTAG
- **OTA Updates** - Remote bytecode deployment (planned)
- **JIT Compilation** - Runtime optimization (planned)

## Quick Start (10 minutes)

### Prerequisites
- ESP32-C6 board (M5Stack NanoC6 or similar)
- ESP-IDF v5.1+ installed and sourced (`. $HOME/esp/esp-idf/export.sh`)
- USB cable

### Option 1: V4 REPL Runtime (Interactive Forth)

```bash
# Navigate to V4-runtime (if in V4-project workspace)
cd V4-runtime/bsp/esp32c6/runtime

# Build and flash
idf.py build flash monitor

# Expected output:
# V4 Runtime initialized
# V4 REPL ready
# v4>
```

You can now type Forth commands interactively!

### Option 2: Hello RTOS Example (Task Demo)

```bash
# Navigate to example
cd bsp/esp32c6/examples/nanoc6/hello-rtos

# Build and flash
idf.py build flash monitor

# Expected output:
# [Task 1] Hello from task 1!
# [Task 2] Hello from task 2!
```

See [Getting Started Guide](docs/getting-started.md) for detailed instructions.

## Architecture

```
V4 Runtime
├── bsp/         Board support packages (ESP32-C6 runtime)
│   └── esp32c6/
│       ├── runtime/     Main V4 runtime application
│       ├── boards/      Board-specific configurations (NanoC6, DevKit)
│       ├── hal_integration/  V4-hal integration layer
│       └── linker/      Linker scripts
├── hal/         V4-hal CMake integration
├── tools/       Development tools and Forth examples
├── scripts/     Build and flash helper scripts
└── docs/        Documentation
```

**Runtime Size**: ~64KB (V4 VM + FreeRTOS + HAL + V4-link)
**RAM Usage**: ~16KB base + per-task stacks

## Supported Platforms

| Platform | Status | MCU | Arch | RAM | Flash |
|----------|--------|-----|------|-----|-------|
| **ESP32-C6** | Stable | ESP32-C6 | RISC-V 32 | 512KB | 4MB |
| ESP32-S3 | Planned | ESP32-S3 | Xtensa LX7 | 512KB | 8MB |
| CH32V203 | Planned | CH32V203 | RISC-V 32 | 20KB | 64KB |
| RP2350 | Planned | RP2350 | ARM + RISC-V | 520KB | 4MB |

## Comparison with Other Forth Environments

| Feature | V4 Runtime | Mecrisp | Zeptoforth | FlashForth |
|---------|------------|---------|------------|------------|
| **Backend** | FreeRTOS | Bare metal | FreeRTOS | Bare metal |
| **Scheduler** | Preemptive | Cooperative | Preemptive | Cooperative |
| **VM** | Yes | No | No | No |
| **REPL** | Yes | Yes | Yes | Yes |
| **Flash** | 64KB~ | 16KB~ | 32KB~ | 8KB~ |
| **Multitasking** | FreeRTOS tasks | None | FreeRTOS tasks | Cooperative |
| **OTA** | Planned | Manual | Manual | Manual |
| **JIT** | Planned | No | No | No |

**V4 Runtime Advantages:**
- Leverages proven FreeRTOS for robust multitasking
- Interactive development without reflashing
- Designed for hot-swapping and live updates
- Unified VM/REPL/runtime integration
- Bytecode-based for portability and dynamic loading

## Documentation

- [Getting Started](docs/getting-started.md) - 10-minute tutorial
- [Architecture](docs/architecture.md) - System design overview
- [API Reference](docs/api-reference/) - Complete API documentation
- [Porting Guide](docs/porting-guide.md) - Adding new platforms
- [Building Guide](docs/building.md) - Build system reference

## Building

```bash
# Build all components
make build

# Run tests
make test

# Format code
make format

# Build ESP32-C6 examples
cd bsp/esp32c6/examples/nanoc6/hello-rtos
idf.py build
```

See [Building Guide](docs/building.md) for detailed instructions.

## Components

- **[bsp/esp32c6/runtime/](bsp/esp32c6/runtime/)** - Main runtime application (V4 VM + FreeRTOS)
- **[bsp/esp32c6/boards/](bsp/esp32c6/boards/)** - Board configurations (NanoC6, DevKit)
- **[hal/](hal/)** - V4-hal CMake integration
- **[tools/examples/](tools/examples/)** - Forth example programs
- **[docs/](docs/)** - Documentation and API reference
- **[scripts/](scripts/)** - Build and flash helper scripts

## Contributing

Contributions are welcome! Currently in active development - please open an issue to discuss before submitting pull requests.

## License

Dual-licensed under:
- MIT License ([LICENSE-MIT](LICENSE-MIT))
- Apache License 2.0 ([LICENSE-APACHE](LICENSE-APACHE))

Choose either license for your use.

## Links

- [Documentation](docs/)
- [Issue Tracker](https://github.com/V4-project/V4-runtime/issues)
- [Discussions](https://github.com/V4-project/V4-runtime/discussions)
