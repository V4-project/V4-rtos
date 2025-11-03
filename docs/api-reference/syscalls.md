# System Calls Reference

V4 VM system calls (SYS instruction) provide low-level access to kernel and hardware functionality from Forth code.

## Overview

System calls are invoked using the `SYS` instruction with a syscall number:

```forth
: TASK-DELAY  ( ms -- )  1 SYS ;
: SEND        ( msg task -- )  2 SYS ;
```

## Task Management

### SYS 0: TASK-CREATE

Create a new task.

```forth
: TASK-CREATE  ( xt priority -- task-id )
    0 SYS ;
```

**Stack:**
- Input: `xt` (execution token), `priority` (0-7)
- Output: `task-id`

**Example:**

```forth
: MY-TASK  ( -- )
    BEGIN
        ." Task running" CR
        1000 TASK-DELAY
    AGAIN ;

' MY-TASK 0 TASK-CREATE CONSTANT tid
```

### SYS 1: TASK-DELAY

Delay current task.

```forth
: TASK-DELAY  ( ms -- )
    1 SYS ;
```

**Stack:**
- Input: `ms` (delay in milliseconds)
- Output: none

**Example:**

```forth
1000 TASK-DELAY  \ Delay 1 second
```

### SYS 2: TASK-DELETE

Delete a task.

```forth
: TASK-DELETE  ( task-id -- result )
    2 SYS ;
```

**Stack:**
- Input: `task-id`
- Output: `result` (0 = success, <0 = error)

## Message Passing

### SYS 10: SEND

Send message to task.

```forth
: SEND  ( addr len task-id -- result )
    10 SYS ;
```

**Stack:**
- Input: `addr` (message address), `len` (length), `task-id`
- Output: `result` (0 = success, <0 = error)

**Example:**

```forth
: SEND-HELLO  ( task-id -- )
    S" Hello" ROT SEND DROP ;

tid SEND-HELLO
```

### SYS 11: RECV

Receive message (blocking).

```forth
: RECV  ( addr maxlen -- len )
    11 SYS ;
```

**Stack:**
- Input: `addr` (buffer address), `maxlen` (buffer size)
- Output: `len` (message length, <0 = error)

**Example:**

```forth
CREATE msg-buf 64 ALLOT

: WAIT-MESSAGE  ( -- )
    msg-buf 64 RECV
    DUP 0< IF
        DROP ." Receive error" CR
    ELSE
        msg-buf SWAP TYPE CR
    THEN ;
```

### SYS 12: RECV-TIMEOUT

Receive message with timeout.

```forth
: RECV-TIMEOUT  ( addr maxlen timeout-ms -- len )
    12 SYS ;
```

**Stack:**
- Input: `addr`, `maxlen`, `timeout-ms`
- Output: `len` (message length, <0 = error/timeout)

## GPIO

### SYS 20: GPIO-MODE

Set GPIO pin mode.

```forth
: GPIO-MODE  ( pin mode -- )
    20 SYS ;
```

**Modes:**
- `0` - INPUT
- `1` - OUTPUT
- `2` - INPUT_PULLUP
- `3` - INPUT_PULLDOWN

**Example:**

```forth
7 1 GPIO-MODE  \ Set pin 7 as output
```

### SYS 21: GPIO-WRITE

Write GPIO pin.

```forth
: GPIO-WRITE  ( pin level -- )
    21 SYS ;
```

**Example:**

```forth
: LED-ON   7 1 GPIO-WRITE ;
: LED-OFF  7 0 GPIO-WRITE ;
```

### SYS 22: GPIO-READ

Read GPIO pin.

```forth
: GPIO-READ  ( pin -- level )
    22 SYS ;
```

**Example:**

```forth
: BUTTON-PRESSED?  ( -- flag )
    9 GPIO-READ 0= ;  \ Active low
```

### SYS 23: GPIO-TOGGLE

Toggle GPIO pin.

```forth
: GPIO-TOGGLE  ( pin -- )
    23 SYS ;
```

## UART

### SYS 30: UART-WRITE

Write to UART.

```forth
: UART-WRITE  ( addr len uart -- count )
    30 SYS ;
```

**Example:**

```forth
: UART-PRINT  ( addr len -- )
    0 UART-WRITE DROP ;

S" Hello UART" UART-PRINT
```

### SYS 31: UART-READ

Read from UART.

```forth
: UART-READ  ( addr maxlen uart -- count )
    31 SYS ;
```

### SYS 32: UART-AVAILABLE

Check bytes available.

```forth
: UART-AVAILABLE  ( uart -- count )
    32 SYS ;
```

## Timer

### SYS 40: GET-TICKS

Get system tick count.

```forth
: GET-TICKS  ( -- ticks )
    40 SYS ;
```

**Example:**

```forth
: MEASURE-TIME  ( xt -- ms )
    GET-TICKS  ( xt start )
    SWAP EXECUTE
    GET-TICKS SWAP -  ;

' MY-FUNCTION MEASURE-TIME . ." ms" CR
```

### SYS 41: TIMER-ONESHOT

Create one-shot timer.

```forth
: TIMER-ONESHOT  ( xt timeout-ms -- )
    41 SYS ;
```

### SYS 42: TIMER-PERIODIC

Create periodic timer.

```forth
: TIMER-PERIODIC  ( xt period-ms -- timer-id )
    42 SYS ;
```

### SYS 43: TIMER-STOP

Stop timer.

```forth
: TIMER-STOP  ( timer-id -- )
    43 SYS ;
```

## Memory

### SYS 50: ALLOC

Allocate memory.

```forth
: ALLOC  ( size -- addr )
    50 SYS ;
```

### SYS 51: FREE

Free memory.

```forth
: FREE  ( addr -- )
    51 SYS ;
```

### SYS 52: MEMCPY

Copy memory.

```forth
: MEMCPY  ( src dest len -- )
    52 SYS ;
```

## System Information

### SYS 60: GET-TASK-ID

Get current task ID.

```forth
: GET-TASK-ID  ( -- task-id )
    60 SYS ;
```

### SYS 61: GET-FREE-HEAP

Get free heap size.

```forth
: GET-FREE-HEAP  ( -- bytes )
    61 SYS ;
```

### SYS 62: GET-TASK-INFO

Get task information.

```forth
: GET-TASK-INFO  ( task-id addr -- result )
    62 SYS ;
```

**Structure:**

```forth
\ task_info structure (24 bytes)
\ Offset 0:  task_id (4 bytes)
\ Offset 4:  state (4 bytes)
\ Offset 8:  priority (4 bytes)
\ Offset 12: stack_used (4 bytes)
\ Offset 16: stack_total (4 bytes)
\ Offset 20: reserved (4 bytes)
```

## Debug

### SYS 70: TRACE

Output trace message.

```forth
: TRACE  ( addr len -- )
    70 SYS ;
```

### SYS 71: ASSERT

Runtime assertion.

```forth
: ASSERT  ( flag msg-addr msg-len -- )
    71 SYS ;
```

**Example:**

```forth
: CHECK-RANGE  ( n -- )
    DUP 0 100 WITHIN
    S" Value out of range" ASSERT ;
```

## Complete Syscall Table

| Number | Name | Description |
|--------|------|-------------|
| 0 | TASK-CREATE | Create task |
| 1 | TASK-DELAY | Delay task |
| 2 | TASK-DELETE | Delete task |
| 10 | SEND | Send message |
| 11 | RECV | Receive message |
| 12 | RECV-TIMEOUT | Receive with timeout |
| 20 | GPIO-MODE | Set GPIO mode |
| 21 | GPIO-WRITE | Write GPIO |
| 22 | GPIO-READ | Read GPIO |
| 23 | GPIO-TOGGLE | Toggle GPIO |
| 30 | UART-WRITE | Write UART |
| 31 | UART-READ | Read UART |
| 32 | UART-AVAILABLE | Check UART bytes |
| 40 | GET-TICKS | Get tick count |
| 41 | TIMER-ONESHOT | One-shot timer |
| 42 | TIMER-PERIODIC | Periodic timer |
| 43 | TIMER-STOP | Stop timer |
| 50 | ALLOC | Allocate memory |
| 51 | FREE | Free memory |
| 52 | MEMCPY | Copy memory |
| 60 | GET-TASK-ID | Get task ID |
| 61 | GET-FREE-HEAP | Get free heap |
| 62 | GET-TASK-INFO | Get task info |
| 70 | TRACE | Debug trace |
| 71 | ASSERT | Runtime assert |

## Performance

System call overhead on ESP32-C6 @ 160MHz:

- Syscall entry/exit: ~10 cycles (~62ns)
- Simple syscalls (GET-TICKS): ~15 cycles
- Complex syscalls (SEND): ~500 cycles

## Error Codes

```forth
0 CONSTANT SYS-OK
-1 CONSTANT SYS-ERR-INVALID
-2 CONSTANT SYS-ERR-TIMEOUT
-3 CONSTANT SYS-ERR-NOMEM
-4 CONSTANT SYS-ERR-BUSY
```

## See Also

- [Kernel API](kernel.md) - C API equivalents
- [HAL API](hal.md) - Hardware abstraction
- [Architecture](../architecture.md) - VM design
