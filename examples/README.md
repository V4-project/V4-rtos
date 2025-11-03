# V4 RTOS Examples

Cross-platform examples demonstrating V4 RTOS features.

## Overview

These examples are designed to run on any V4 RTOS supported platform (POSIX, ESP32-C6, etc.). They demonstrate core RTOS features independent of hardware.

## Example Structure

```
examples/
├── 01-hello-world/      # Minimal "Hello RTOS" example
├── 02-multitasking/     # Multiple concurrent tasks
├── 03-message-passing/  # Inter-task communication
└── 04-hot-swap/         # Runtime code update
```

## Platform-Specific Examples

For platform-specific examples using actual hardware:

- **ESP32-C6**: See `bsp/esp32c6/examples/`
- **POSIX**: Build examples with `make` for host testing

## Building

### POSIX (Host)

```bash
# Build all examples
cmake -B build -DV4_BUILD_EXAMPLES=ON
cmake --build build

# Run example
./build/examples/01-hello-world/hello-world
```

### ESP32-C6

Platform-specific examples are in BSP directories:

```bash
cd bsp/esp32c6/examples/nanoc6/hello-rtos
idf.py build flash monitor
```

## Examples

### 01-hello-world

**Status**: Planned

Minimal example demonstrating:
- V4 initialization
- Single task
- Serial output

### 02-multitasking

**Status**: Planned

Demonstrates:
- Multiple concurrent tasks
- Preemptive scheduling
- Task priorities

### 03-message-passing

**Status**: Planned

Demonstrates:
- Sending messages between tasks
- Blocking receives
- Message queues

### 04-hot-swap

**Status**: Planned

Demonstrates:
- Runtime bytecode loading
- Live code updates
- V4-link protocol

## Development Status

Examples are currently placeholders. Implementation priority:

1. **hello-world** - Foundation for all others
2. **multitasking** - Core RTOS feature
3. **message-passing** - Inter-task communication
4. **hot-swap** - Unique V4 feature

## Hardware Examples

For ready-to-run hardware examples, see:

- [ESP32-C6 NanoC6 Examples](../bsp/esp32c6/examples/nanoc6/)
  - **hello-rtos**: Two-task demo with LED
  - **blink**: Simple LED blink

## Contributing

Example contributions welcome! Guidelines:

- Keep examples simple and focused
- Include comprehensive README
- Test on multiple platforms
- Add comments explaining key concepts

## See Also

- [Getting Started](../docs/getting-started.md)
- [Architecture](../docs/architecture.md)
- [API Reference](../docs/api-reference/)
