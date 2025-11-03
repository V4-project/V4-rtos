# V4 RTOS Documentation

Welcome to the V4 RTOS documentation. This directory contains comprehensive guides and references for using and developing with V4 RTOS.

## Quick Links

- [Getting Started](getting-started.md) - 10-minute quick start guide
- [Architecture](architecture.md) - System architecture and design
- [Building Guide](building.md) - Build system and compilation
- [Porting Guide](porting-guide.md) - Adding support for new platforms
- [API Reference](api-reference/) - Complete API documentation
- [Comparison](comparison.md) - Comparison with other RTOSes

## Documentation Structure

### User Guides

- **[getting-started.md](getting-started.md)** - First steps with V4 RTOS
  - ESP32-C6 setup
  - Building and flashing your first program
  - Hello RTOS example walkthrough

- **[building.md](building.md)** - Build system reference
  - CMake configuration
  - Make targets
  - Cross-compilation

### Developer Guides

- **[architecture.md](architecture.md)** - System architecture
  - Component overview
  - Data flow
  - Scheduler design
  - Memory layout

- **[porting-guide.md](porting-guide.md)** - Platform porting
  - HAL implementation
  - BSP structure
  - Board configuration
  - Platform-specific considerations

- **[comparison.md](comparison.md)** - RTOS comparison
  - V4 RTOS vs FreeRTOS
  - V4 RTOS vs Zephyr
  - V4 RTOS vs Mecrisp
  - Use case recommendations

### API Reference

- **[api-reference/](api-reference/)** - Complete API documentation
  - [Kernel API](api-reference/kernel.md) - VM and scheduler
  - [HAL API](api-reference/hal.md) - Hardware abstraction
  - [Compiler API](api-reference/compiler.md) - Forth compiler
  - [System Calls](api-reference/syscalls.md) - SYS instruction reference

## Contributing

Contributions to documentation are welcome! Please ensure:

- Clear, concise language
- Code examples are tested
- Proper markdown formatting
- Cross-references where appropriate

## License

Documentation is licensed under the same dual MIT/Apache-2.0 license as the code.
