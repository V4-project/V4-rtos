# Hello RTOS Example

Minimal V4 RTOS example demonstrating basic multitasking on M5Stack NanoC6.

## Features

This example demonstrates:

- **Two concurrent tasks** running simultaneously
- **Task 1**: Blinks LED every second, prints counter
- **Task 2**: Monitors button state, prints every 2 seconds
- **Preemptive scheduling** - tasks automatically switch
- **Board abstraction** using `board.h` configuration

## Hardware Required

- M5Stack NanoC6 board
- USB-C cable

## How to Use

### Build and Flash

```bash
# Navigate to example directory
cd bsp/esp32c6/examples/nanoc6/hello-rtos

# Build
idf.py build

# Flash and monitor
idf.py flash monitor
```

### Expected Output

```
I (123) hello-rtos: === V4 RTOS Hello World ===
I (124) hello-rtos: Board: M5Stack NanoC6
I (125) hello-rtos: MCU: ESP32-C6
I (126) hello-rtos: GPIO initialized (LED: 7, Button: 9)
I (127) hello-rtos: Task 1 created
I (128) hello-rtos: Task 2 created
I (129) hello-rtos: Scheduler started - tasks running
I (130) hello-rtos: Task 1 started
I (131) hello-rtos: Task 2 started
I (1130) hello-rtos: [Task 1] Count: 0
I (2131) hello-rtos: [Task 2] Running...
I (2130) hello-rtos: [Task 1] Count: 1
I (3130) hello-rtos: [Task 1] Count: 2
I (4131) hello-rtos: [Task 2] Running...
I (4130) hello-rtos: [Task 1] Count: 3
...
```

## What's Happening

1. **Initialization**: Board GPIO configured for LED and button
2. **Task Creation**: Two FreeRTOS tasks created
3. **Task 1**:
   - Runs every 1 second
   - Toggles LED (GPIO 7)
   - Prints incrementing counter
4. **Task 2**:
   - Runs every 2 seconds
   - Reads button state (GPIO 9)
   - Prints status

## Hardware Behavior

- **LED (GPIO 7)**: Blinks every second
- **Button (GPIO 9)**: Press to see "Button pressed!" message

## Code Structure

```c
void task1(void *pvParameters) {
    while (1) {
        // Toggle LED
        gpio_set_level(LED_PIN, counter % 2);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task2(void *pvParameters) {
    while (1) {
        // Check button
        int button_state = gpio_get_level(BUTTON_PIN);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void) {
    board_gpio_init();
    xTaskCreate(task1, "task1", 4096, NULL, 5, NULL);
    xTaskCreate(task2, "task2", 4096, NULL, 5, NULL);
}
```

## Next Steps

- Modify task delays to see different timing
- Add more tasks
- Implement message passing between tasks
- Try the blink example for LED-only demo

## Note on V4 RTOS Integration

This example currently uses FreeRTOS (included with ESP-IDF) as a placeholder.
Future versions will use the V4 kernel for scheduling and message passing.

The structure is designed to make migration straightforward:

```c
// Current (FreeRTOS)
xTaskCreate(task1, "task1", 4096, NULL, 5, NULL);
vTaskDelay(pdMS_TO_TICKS(1000));

// Future (V4 RTOS)
v4_task_create(task1, NULL, 0);
v4_task_delay(1000);
```

## Troubleshooting

### Build fails

- Ensure ESP-IDF is sourced: `. $HOME/esp/esp-idf/export.sh`
- Check ESP-IDF version: `idf.py --version` (need v5.1+)

### Flash fails

- Check USB connection
- Try: `idf.py -b 115200 flash`
- Erase flash first: `idf.py erase-flash`

### LED doesn't blink

- Verify board is NanoC6 (LED on GPIO 7)
- Check board connections
- Try the dedicated blink example

### No serial output

- Press reset button on board
- Check serial port: `ls /dev/ttyUSB* /dev/ttyACM*`
- Try different baud rate: `idf.py monitor -b 115200`

## References

- [Getting Started Guide](../../../../../docs/getting-started.md)
- [NanoC6 Board Documentation](../../../boards/nanoc6/README.md)
- [V4 RTOS Architecture](../../../../../docs/architecture.md)
