# V4 RTOS API Reference

Complete API documentation for V4 RTOS components.

## Components

### Core APIs

- **[Kernel API](kernel.md)** - VM, scheduler, and task management
  - Task creation and control
  - Message passing
  - Timing and delays
  - System calls

- **[HAL API](hal.md)** - Hardware abstraction layer
  - GPIO
  - UART
  - Timer
  - Interrupt handling

### Development APIs

- **[Compiler API](compiler.md)** - Forth compiler interface
  - Compilation
  - Error handling
  - Symbol management

- **[System Calls](syscalls.md)** - SYS instruction reference
  - Complete list of VM system calls
  - Usage examples
  - Performance notes

## Usage

Each API document includes:

- Function prototypes
- Parameter descriptions
- Return values
- Usage examples
- Platform-specific notes

## Conventions

### Naming

- C functions: `v4_function_name()`
- C++ classes: `V4ClassName`
- Forth words: `FORTH-WORD`
- Constants: `V4_CONSTANT_NAME`

### Return Values

Most functions follow these conventions:

- `0` - Success
- `<0` - Error code (see error.h)
- `>0` - Success with value

### Thread Safety

- **Thread-safe**: Can be called from multiple tasks
- **Not thread-safe**: Requires external synchronization
- **ISR-safe**: Can be called from interrupt context

Each function documents its thread safety guarantees.

## Examples

Example code is provided for common use cases:

```c
// Task creation example
uint32_t task_id = v4_task_create(task_function, priority);

// Message passing example
v4_send_message(task_id, "Hello");
```

See individual API documents for detailed examples.
