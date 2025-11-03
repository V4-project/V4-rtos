# HAL API Reference

The Hardware Abstraction Layer (HAL) provides a unified interface for accessing hardware peripherals across different platforms.

## Design

V4 HAL uses C++17 CRTP (Curiously Recurring Template Pattern) for zero-overhead abstraction:

```cpp
template<typename Derived>
class GPIO {
    // Platform-independent interface
};

class ESP32_GPIO : public GPIO<ESP32_GPIO> {
    // Platform-specific implementation
};
```

## GPIO

### Pin Configuration

```cpp
void gpio_set_mode(Pin pin, Mode mode);
```

**Parameters:**
- `pin` - GPIO pin number
- `mode` - Pin mode:
  - `GPIO_MODE_INPUT` - Digital input
  - `GPIO_MODE_OUTPUT` - Digital output
  - `GPIO_MODE_INPUT_PULLUP` - Input with pull-up
  - `GPIO_MODE_INPUT_PULLDOWN` - Input with pull-down

**Example:**

```cpp
// Configure LED pin as output
gpio_set_mode(GPIO_NUM_7, GPIO_MODE_OUTPUT);

// Configure button pin with pull-up
gpio_set_mode(GPIO_NUM_9, GPIO_MODE_INPUT_PULLUP);
```

### Digital I/O

```cpp
void gpio_write(Pin pin, Level level);
Level gpio_read(Pin pin);
void gpio_toggle(Pin pin);
```

**Example:**

```cpp
// Turn LED on
gpio_write(LED_PIN, GPIO_LEVEL_HIGH);

// Read button state
if (gpio_read(BUTTON_PIN) == GPIO_LEVEL_LOW) {
    // Button pressed
}

// Toggle LED
gpio_toggle(LED_PIN);
```

### Interrupts

```cpp
void gpio_set_interrupt(
    Pin pin,
    InterruptMode mode,
    gpio_isr_t handler,
    void* arg
);
```

**Parameters:**
- `pin` - GPIO pin
- `mode` - Interrupt trigger:
  - `GPIO_INTR_RISING` - Rising edge
  - `GPIO_INTR_FALLING` - Falling edge
  - `GPIO_INTR_BOTH` - Both edges
- `handler` - ISR callback
- `arg` - User argument for callback

**Example:**

```cpp
void button_isr(void* arg) {
    // Handle button press
    gpio_toggle(LED_PIN);
}

gpio_set_interrupt(
    BUTTON_PIN,
    GPIO_INTR_FALLING,
    button_isr,
    NULL
);
```

## UART

### Initialization

```cpp
void uart_init(
    UartNum uart,
    uint32_t baud_rate,
    UartConfig config
);
```

**Parameters:**
- `uart` - UART port number (UART_NUM_0, UART_NUM_1, etc.)
- `baud_rate` - Baud rate (e.g., 115200)
- `config` - Configuration:
  - `data_bits` - 5, 6, 7, or 8
  - `parity` - NONE, EVEN, ODD
  - `stop_bits` - 1 or 2

**Example:**

```cpp
UartConfig config = {
    .data_bits = 8,
    .parity = UART_PARITY_NONE,
    .stop_bits = 1
};

uart_init(UART_NUM_0, 115200, config);
```

### Data Transfer

```cpp
int uart_write(UartNum uart, const uint8_t* data, size_t len);
int uart_read(UartNum uart, uint8_t* buffer, size_t len);
int uart_available(UartNum uart);
```

**Example:**

```cpp
// Write string
const char* msg = "Hello UART\n";
uart_write(UART_NUM_0, (const uint8_t*)msg, strlen(msg));

// Read with timeout
uint8_t buffer[64];
int len = uart_read(UART_NUM_0, buffer, sizeof(buffer));
if (len > 0) {
    // Process received data
}
```

## Timer

### One-shot Timer

```cpp
void timer_oneshot(
    uint32_t timeout_ms,
    timer_callback_t callback,
    void* arg
);
```

**Example:**

```cpp
void timer_expired(void* arg) {
    printf("Timer expired!\n");
}

timer_oneshot(5000, timer_expired, NULL);  // 5 second timer
```

### Periodic Timer

```cpp
TimerHandle timer_periodic(
    uint32_t period_ms,
    timer_callback_t callback,
    void* arg
);

void timer_stop(TimerHandle handle);
```

**Example:**

```cpp
void heartbeat(void* arg) {
    gpio_toggle(LED_PIN);
}

// Blink LED every 500ms
TimerHandle timer = timer_periodic(500, heartbeat, NULL);

// Stop later
timer_stop(timer);
```

### Delay

```cpp
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
```

**Note:** These are blocking delays. Use `v4_task_delay()` for non-blocking delays in RTOS tasks.

## Interrupt Management

### Enable/Disable

```cpp
void interrupt_enable(void);
void interrupt_disable(void);
uint32_t interrupt_save(void);
void interrupt_restore(uint32_t state);
```

**Example:**

```cpp
// Critical section
uint32_t state = interrupt_save();
// ... critical code ...
interrupt_restore(state);
```

### ISR Utilities

```cpp
bool is_in_isr(void);
void isr_yield(void);  // Trigger context switch from ISR
```

## Platform-Specific APIs

### ESP32-C6

```cpp
#include "hal/ports/esp32/esp32_hal.h"

// Access ESP-IDF directly
esp_err_t result = esp_wifi_init(&cfg);
```

### POSIX (Simulation)

```cpp
#include "hal/ports/posix/posix_hal.h"

// File I/O for testing
int fd = open("/dev/ttyUSB0", O_RDWR);
```

## Error Handling

HAL functions return status codes:

```cpp
#define HAL_OK          0
#define HAL_ERR_INVALID -1
#define HAL_ERR_TIMEOUT -2
#define HAL_ERR_BUSY    -3
```

## Performance

Typical operation timing on ESP32-C6 @ 160MHz:

| Operation | Time |
|-----------|------|
| `gpio_write()` | ~500ns |
| `gpio_read()` | ~600ns |
| `uart_write()` (byte) | ~8μs @ 115200 |
| `timer_oneshot()` | ~2μs |
| Interrupt latency | ~3μs |

## Thread Safety

- **GPIO**: Thread-safe
- **UART**: Thread-safe (uses locks)
- **Timer**: Thread-safe
- **Interrupt**: ISR-safe functions are marked

## Porting Guide

To add a new platform:

1. Implement CRTP derived classes
2. Add port directory: `hal/ports/myplatform/`
3. Implement required interfaces
4. Add platform-specific build configuration

See [Porting Guide](../porting-guide.md) for details.

## See Also

- [Architecture](../architecture.md) - HAL design
- [Kernel API](kernel.md) - RTOS functions
- [Porting Guide](../porting-guide.md) - Adding platforms
