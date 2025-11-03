# V4 Shell (V4-repl)

Interactive Read-Eval-Print Loop (REPL) for V4 RTOS, enabling live Forth programming on embedded devices.

## Features

- Interactive Forth interpreter
- Command line editing with history
- Built-in help system
- Stack and memory inspection
- Task debugging
- Integration with V4 compiler and VM

## Structure

```
shell/
├── include/
│   ├── repl.h           # REPL core
│   ├── editor.h         # Line editor
│   ├── history.h        # Command history
│   └── help.h           # Help system
├── src/
│   ├── repl.c
│   ├── editor.c
│   ├── history.c
│   └── help.c
└── tests/
    └── test_shell.c
```

## Building

```bash
cmake -B build -DV4_BUILD_KERNEL=ON -DV4_BUILD_COMPILER=ON -DV4_BUILD_SHELL=ON
cmake --build build
```

## Usage

```c
#include "v4_shell.h"

int main() {
    v4_init();
    v4_shell_init();
    v4_shell_run();  // Enter interactive mode
    return 0;
}
```

## Interactive Commands

### Standard Forth

```forth
V4> 1 2 + .
3 ok

V4> : DOUBLE  2 * ;
ok

V4> 5 DOUBLE .
10 ok
```

### Debug Commands

```forth
V4> .S              \ Display stack
<3> 1 2 3 ok

V4> WORDS           \ List all words
DOUBLE + - * / DUP DROP ... ok

V4> SEE DOUBLE      \ Disassemble word
: DOUBLE
  LIT 2
  *
  EXIT
; ok

V4> TASKS           \ List running tasks
ID  STATE    PRIORITY  NAME
0   RUNNING  0         main
1   READY    1         task1
2   BLOCKED  1         task2
ok
```

### RTOS Commands

```forth
V4> TASK-CREATE     \ Create new task
V4> SEND            \ Send message
V4> RECV            \ Receive message
V4> TASK-DELAY      \ Delay task
```

## Line Editor

- Arrow keys: Navigate history
- Ctrl+A: Beginning of line
- Ctrl+E: End of line
- Ctrl+K: Kill to end of line
- Ctrl+U: Kill entire line
- Ctrl+C: Cancel current line
- Tab: Word completion (planned)

## Help System

```forth
V4> HELP
Available commands:
  .S        - Display stack
  WORDS     - List all words
  SEE       - Disassemble word
  TASKS     - List tasks
  HELP      - This help
  BYE       - Exit REPL

V4> HELP .S
.S ( -- )
Display contents of data stack
```

## Memory Footprint

- Flash: ~91 KB
- RAM: ~16 KB (line buffer, history, symbol table)

## Configuration

```c
#define V4_SHELL_HISTORY_SIZE  32      // Command history entries
#define V4_SHELL_LINE_SIZE     128     // Max line length
#define V4_SHELL_PROMPT        "V4> "  // Prompt string
```

## Integration Example

```c
// main.c
void my_task(void* arg) {
    // Task can be debugged via REPL
    while (1) {
        printf("Task running\n");
        v4_task_delay(1000);
    }
}

int main() {
    v4_init();
    v4_shell_init();

    // Create task
    v4_task_create(my_task, NULL, 1);

    // Enter REPL (can interact with running tasks)
    v4_shell_run();

    return 0;
}
```

## License

Dual licensed under MIT or Apache-2.0.
