# V4 CLI Tools

**Status**: Planned

Command-line tools for V4 RTOS development.

## Overview

The V4 CLI (v4_cli) will provide tools for:

- Compiling Forth to bytecode
- Transferring code to devices via V4-link
- Interactive REPL connection
- Device management

## Planned Tools

### v4c - Compiler

```bash
# Compile Forth source to bytecode
v4c program.fth -o program.bin

# Compile and show disassembly
v4c program.fth --disassemble
```

### v4flash - Flasher

```bash
# Flash bytecode to device
v4flash -p /dev/ttyUSB0 program.bin

# Interactive upload with progress
v4flash -p /dev/ttyUSB0 program.bin --verbose
```

### v4repl - REPL Client

```bash
# Connect to device REPL
v4repl -p /dev/ttyUSB0

# Execute Forth commands
v4repl -p /dev/ttyUSB0 -c ": DOUBLE 2 * ;"
```

### v4mon - Monitor

```bash
# Monitor device output
v4mon -p /dev/ttyUSB0

# Monitor with filtering
v4mon -p /dev/ttyUSB0 --filter "ERROR|WARNING"
```

## Development

Tools will be implemented in Rust using:

- `clap` for CLI parsing
- `serialport` for device communication
- `indicatif` for progress bars

## Building

Coming soon:

```bash
cd tools
cargo build --release
```

## Installation

```bash
cargo install --path tools
```

## See Also

- [V4-link Protocol](../protocol/)
- [V4 Compiler](../compiler/)
- [V4 Shell](../shell/)
