# V4 RTOS Architecture

This document describes the overall architecture and design of V4 RTOS.

## Overview

V4 RTOS is a lightweight, preemptive real-time operating system built around a stack-based virtual machine executing Forth bytecode. It consists of several layered components that work together to provide a complete embedded development environment.

## System Architecture

```
┌─────────────────────────────────────────────────┐
│              User Application                    │
│         (Forth Bytecode / C Code)               │
└──────────────────┬──────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────┐
│              Shell (REPL)                       │
│        Interactive Development                  │
│              (91 KB)                            │
└──────────────────┬──────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────┐
│            Compiler (V4-front)                  │
│         Forth → Bytecode                        │
└──────────────────┬──────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────┐
│        Kernel (VM + Scheduler)                  │
│                                                 │
│  ┌─────────────┐  ┌──────────────┐            │
│  │  V4 VM      │  │  Scheduler   │            │
│  │  (Stack VM) │  │  (8 tasks)   │            │
│  └─────────────┘  └──────────────┘            │
│  ┌──────────────────────────────┐             │
│  │    Message Queue (16 msg)    │             │
│  └──────────────────────────────┘             │
│                (42 KB)                         │
└──────────────────┬──────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────┐
│          HAL (Hardware Abstraction)             │
│    GPIO | UART | Timer | Interrupt              │
│               (5.7 KB)                          │
└──────────────────┬──────────────────────────────┘
                   │
┌──────────────────▼──────────────────────────────┐
│              BSP (Board)                        │
│     Platform-specific Implementation            │
│      (ESP-IDF, POSIX, CH32V, etc.)             │
└─────────────────────────────────────────────────┘
```

## Core Components

### 1. Kernel (V4 Core)

**Location:** `kernel/`

The kernel is the heart of V4 RTOS, providing:

- **Stack-based VM**: Executes V4 bytecode instructions
- **Preemptive Scheduler**: Time-sliced multitasking for up to 8 tasks
- **Message Passing**: 16-message queue for inter-task communication
- **Memory Management**: Separate stacks per task

**Key Features:**

- Context switch: <100μs
- Task states: Running, Ready, Blocked, Sleeping
- Priority-based scheduling (future)
- Binary size: ~42KB

**Data Structures:**

```c
typedef struct {
    uint32_t pc;              // Program counter
    uint32_t sp;              // Stack pointer
    uint32_t rp;              // Return stack pointer
    uint32_t state;           // Task state
    uint32_t stack[STACK_SIZE];
    uint32_t rstack[RSTACK_SIZE];
} task_t;
```

### 2. HAL (Hardware Abstraction Layer)

**Location:** `hal/`

The HAL provides a unified C++17 CRTP-based interface for hardware access:

- **GPIO**: Digital I/O with interrupts
- **UART**: Serial communication
- **Timer**: Periodic and one-shot timers
- **Interrupt**: Interrupt management

**Design Pattern:**

```cpp
template<typename Derived>
class GPIO {
public:
    void set_mode(Pin pin, Mode mode) {
        static_cast<Derived*>(this)->impl_set_mode(pin, mode);
    }
    // ...
};

class ESP32_GPIO : public GPIO<ESP32_GPIO> {
    void impl_set_mode(Pin pin, Mode mode);
    // ...
};
```

**Ports:**

- `posix/` - POSIX simulation (Linux/macOS development)
- `esp32/` - ESP32-C6/S3 via ESP-IDF
- `ch32v203/` - WCH CH32V203 RISC-V (future)

### 3. Compiler (V4-front)

**Location:** `compiler/`

Forth-to-bytecode compiler:

- **Lexer**: Tokenizes Forth source
- **Parser**: Builds syntax tree
- **Compiler**: Generates V4 bytecode
- **Optimizer**: Peephole optimizations (future)

**Compilation Flow:**

```
Forth Source → Lexer → Parser → Compiler → V4 Bytecode
   ": FOO      → tokens → AST  → codegen → [LIT, 1, ...]
      1 2 + ;"
```

### 4. Shell (V4-repl)

**Location:** `shell/`

Interactive Read-Eval-Print Loop:

- **Line Editor**: Command line editing with history
- **Interpreter**: Execute Forth interactively
- **Debugger**: Inspect stack, memory, tasks
- **Help System**: Built-in documentation

**Commands:**

- Standard Forth words: `+`, `-`, `DUP`, `SWAP`, etc.
- RTOS commands: `TASK-CREATE`, `SEND`, `RECV`
- Debug commands: `.S` (stack), `WORDS`, `SEE`

### 5. Protocol (V4-link)

**Location:** `protocol/`

Bytecode transfer protocol for OTA updates:

- **Framing**: Packet-based communication
- **CRC**: Error detection
- **Compression**: Optional LZ4 compression (future)
- **Encryption**: Optional AES encryption (future)

### 6. BSP (Board Support Package)

**Location:** `bsp/`

Platform-specific integration:

**Structure:**

```
bsp/esp32c6/
├── hal_integration/    # HAL → ESP-IDF bridge
├── linker/            # Memory layout
├── boards/            # Board-specific configs
│   └── nanoc6/
│       ├── board.h    # Pin definitions
│       └── sdkconfig  # ESP-IDF config
└── examples/          # Board examples
```

**Hybrid Design:**

- **Chip-level**: Common HAL integration, linker scripts
- **Board-level**: Pin mappings, peripheral configs

## Memory Layout

### ESP32-C6 Example

```
0x4200_0000 ┌──────────────────┐
            │   Code (Flash)   │
            │   - Kernel       │
            │   - HAL          │
            │   - Application  │
0x4204_0000 ├──────────────────┤
            │   Rodata         │
0x4080_0000 ├──────────────────┤
            │   BSS            │
            ├──────────────────┤
            │   Heap           │
            ├──────────────────┤
            │   Task Stacks    │
            │   (8 x 4KB)      │
0x4087_FFFF └──────────────────┘
```

## Scheduling

### Preemptive Round-Robin

1. Timer interrupt every 1ms
2. Save current task context
3. Select next ready task
4. Restore context and resume

### Task States

```
    ┌─────────┐
    │ READY   │◄──────┐
    └────┬────┘       │
         │            │
         ▼            │
    ┌─────────┐      │
    │ RUNNING │──────┘
    └────┬────┘
         │
         ├──► BLOCKED (waiting for message)
         │
         └──► SLEEPING (timer delay)
```

## Message Passing

Tasks communicate via message queue:

```c
// Task 1 sends message
v4_send_message(task2_id, "Hello");

// Task 2 receives message
char msg[32];
v4_recv_message(msg, sizeof(msg));
```

**Implementation:**

- Circular buffer (16 messages)
- Blocking receive (task sleeps if empty)
- Non-blocking send (drops if full)

## Data Flow

```
User Input → REPL → Compiler → Bytecode → VM → HAL → Hardware
                                             ↓
                                        Scheduler
                                        (Tasks)
```

## Performance

### Memory Footprint

| Component | Flash | RAM |
|-----------|-------|-----|
| Kernel    | 42 KB | 8 KB (per task) |
| HAL       | 5.7 KB | 512 B |
| Compiler  | 35 KB | 4 KB |
| Shell     | 91 KB | 16 KB |
| **Total** | **174 KB** | **96 KB** |

### Timing

- Context switch: <100μs
- Syscall overhead: ~10 cycles
- Message pass: ~50μs
- Task wake-up: ~20μs

## Future Enhancements

- JIT compilation for hot code paths
- Priority-based scheduling
- Mutex/semaphore primitives
- Dynamic task creation
- Memory protection (MPU)

## References

- [Kernel API](api-reference/kernel.md)
- [HAL API](api-reference/hal.md)
- [Porting Guide](porting-guide.md)
