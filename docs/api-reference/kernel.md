# Kernel API Reference

The kernel provides the core V4 VM, scheduler, and task management functionality.

## Task Management

### v4_task_create

Create a new task.

```c
uint32_t v4_task_create(
    void (*entry_point)(void*),
    void* arg,
    uint32_t priority
);
```

**Parameters:**
- `entry_point` - Task entry function
- `arg` - Argument passed to task
- `priority` - Task priority (0-7, lower is higher priority)

**Returns:**
- Task ID on success
- `V4_INVALID_TASK_ID` on failure

**Example:**

```c
void my_task(void* arg) {
    while (1) {
        // Task code
        v4_task_delay(1000);
    }
}

uint32_t tid = v4_task_create(my_task, NULL, 0);
```

**Thread Safety:** Not thread-safe (call from main task only)

### v4_task_delete

Delete a task.

```c
int v4_task_delete(uint32_t task_id);
```

**Parameters:**
- `task_id` - ID of task to delete

**Returns:**
- `0` on success
- `V4_ERR_INVALID_TASK` if task doesn't exist

**Thread Safety:** Not thread-safe

### v4_task_delay

Delay current task for specified milliseconds.

```c
void v4_task_delay(uint32_t ms);
```

**Parameters:**
- `ms` - Delay in milliseconds

**Example:**

```c
// Blink LED every second
while (1) {
    gpio_toggle(LED_PIN);
    v4_task_delay(1000);
}
```

**Thread Safety:** Thread-safe

## Message Passing

### v4_send_message

Send message to another task.

```c
int v4_send_message(
    uint32_t task_id,
    const char* message
);
```

**Parameters:**
- `task_id` - Destination task ID
- `message` - Null-terminated message string

**Returns:**
- `0` on success
- `V4_ERR_QUEUE_FULL` if message queue is full
- `V4_ERR_INVALID_TASK` if task doesn't exist

**Example:**

```c
v4_send_message(task2_id, "Hello from task 1");
```

**Thread Safety:** Thread-safe

### v4_recv_message

Receive message (blocking).

```c
int v4_recv_message(
    char* buffer,
    uint32_t buffer_size
);
```

**Parameters:**
- `buffer` - Buffer to store received message
- `buffer_size` - Size of buffer

**Returns:**
- Message length on success
- `V4_ERR_BUFFER_TOO_SMALL` if message doesn't fit

**Example:**

```c
char msg[64];
int len = v4_recv_message(msg, sizeof(msg));
if (len > 0) {
    printf("Received: %s\n", msg);
}
```

**Thread Safety:** Thread-safe (blocks if no messages)

### v4_recv_message_timeout

Receive message with timeout.

```c
int v4_recv_message_timeout(
    char* buffer,
    uint32_t buffer_size,
    uint32_t timeout_ms
);
```

**Parameters:**
- `buffer` - Buffer to store received message
- `buffer_size` - Size of buffer
- `timeout_ms` - Timeout in milliseconds

**Returns:**
- Message length on success
- `V4_ERR_TIMEOUT` if timeout occurs
- `V4_ERR_BUFFER_TOO_SMALL` if message doesn't fit

**Thread Safety:** Thread-safe

## Scheduler Control

### v4_scheduler_start

Start the RTOS scheduler.

```c
void v4_scheduler_start(void);
```

**Note:** This function does not return. Call after all tasks are created.

**Example:**

```c
int main() {
    v4_task_create(task1, NULL, 0);
    v4_task_create(task2, NULL, 0);
    v4_scheduler_start();
    // Never reached
}
```

**Thread Safety:** Not thread-safe (call once from main)

### v4_scheduler_suspend

Temporarily suspend scheduling.

```c
void v4_scheduler_suspend(void);
```

**Use Case:** Critical sections that must not be interrupted

**Thread Safety:** ISR-safe

### v4_scheduler_resume

Resume scheduling.

```c
void v4_scheduler_resume(void);
```

**Thread Safety:** ISR-safe

## System Information

### v4_get_tick_count

Get system tick count.

```c
uint32_t v4_get_tick_count(void);
```

**Returns:** Number of ticks since scheduler started (1 tick = 1ms)

**Thread Safety:** Thread-safe, ISR-safe

### v4_get_task_info

Get information about a task.

```c
int v4_get_task_info(
    uint32_t task_id,
    v4_task_info_t* info
);
```

**Parameters:**
- `task_id` - Task to query
- `info` - Pointer to info structure

**Returns:**
- `0` on success
- `V4_ERR_INVALID_TASK` if task doesn't exist

**Example:**

```c
v4_task_info_t info;
if (v4_get_task_info(tid, &info) == 0) {
    printf("Task state: %d\n", info.state);
    printf("Stack usage: %d bytes\n", info.stack_used);
}
```

**Thread Safety:** Thread-safe

## Error Codes

```c
#define V4_OK                    0
#define V4_ERR_INVALID_TASK     -1
#define V4_ERR_QUEUE_FULL       -2
#define V4_ERR_BUFFER_TOO_SMALL -3
#define V4_ERR_TIMEOUT          -4
#define V4_INVALID_TASK_ID      0xFFFFFFFF
```

## Data Structures

```c
typedef struct {
    uint32_t task_id;
    uint32_t state;        // V4_TASK_STATE_*
    uint32_t priority;
    uint32_t stack_used;   // Bytes
    uint32_t stack_total;  // Bytes
} v4_task_info_t;
```

Task states:

```c
#define V4_TASK_STATE_RUNNING  0
#define V4_TASK_STATE_READY    1
#define V4_TASK_STATE_BLOCKED  2
#define V4_TASK_STATE_SLEEPING 3
```

## Performance Notes

- Task switch: <100μs on ESP32-C6
- Message send: ~50μs
- Message receive (no wait): ~30μs
- Task creation: ~200μs

## See Also

- [Architecture](../architecture.md) - Scheduler design
- [HAL API](hal.md) - Hardware abstraction
- [System Calls](syscalls.md) - Low-level VM operations
