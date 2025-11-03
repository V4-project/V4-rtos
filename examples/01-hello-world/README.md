# Example 01: Hello World

**Status**: Planned

Minimal V4 RTOS example demonstrating basic initialization and task creation.

## Overview

This will be the simplest possible V4 RTOS program:

```c
#include "v4_kernel.h"

void hello_task(void* arg) {
    printf("Hello from V4 RTOS!\n");
}

int main() {
    v4_init();
    v4_task_create(hello_task, NULL, 0);
    v4_scheduler_start();
    return 0;
}
```

## What It Demonstrates

- V4 kernel initialization
- Creating a single task
- Starting the scheduler
- Basic task execution

## Building

Coming soon - requires kernel implementation.

## Expected Output

```
V4 RTOS v1.0.0
Initializing...
Creating task...
Starting scheduler...
Hello from V4 RTOS!
Task complete
```

## Next Steps

After this example works, try:
- [02-multitasking](../02-multitasking/) - Multiple tasks
- [03-message-passing](../03-message-passing/) - Task communication
