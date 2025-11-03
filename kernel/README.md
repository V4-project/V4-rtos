# V4 Kernel

The V4 kernel provides the core virtual machine, preemptive scheduler, and task management functionality for V4 RTOS.

## Features

- Stack-based virtual machine executing V4 bytecode
- Preemptive multitasking (up to 8 tasks)
- Message-passing inter-task communication
- Round-robin scheduling with <100μs context switch
- 16-message queue per task
- Platform-independent implementation

## Structure

```
kernel/
├── include/
│   ├── v4_vm.h          # VM core
│   ├── v4_scheduler.h   # Scheduler
│   ├── v4_task.h        # Task management
│   └── v4_message.h     # Message passing
├── src/
│   ├── vm.c             # VM implementation
│   ├── scheduler.c      # Scheduler implementation
│   ├── task.c           # Task management
│   └── message.c        # Message queue
└── tests/
    └── test_kernel.c    # Unit tests
```

## Building

The kernel is built as part of the V4 RTOS project:

```bash
cmake -B build -DV4_BUILD_KERNEL=ON
cmake --build build
```

## API

See [Kernel API Documentation](../docs/api-reference/kernel.md) for complete API reference.

### Quick Example

```c
#include "v4_kernel.h"

void task1(void* arg) {
    while (1) {
        printf("Task 1\n");
        v4_task_delay(1000);
    }
}

int main() {
    v4_init();
    v4_task_create(task1, NULL, 0);
    v4_scheduler_start();
    return 0;
}
```

## Memory Requirements

- Flash: ~42 KB
- RAM per task: ~8 KB (4KB data stack + 4KB return stack)
- Message queue: ~1 KB (16 messages x 64 bytes)

## Performance

On ESP32-C6 @ 160MHz:

- Context switch: <100μs
- Syscall overhead: ~10 cycles
- Message pass: ~50μs

## License

Dual licensed under MIT or Apache-2.0.
