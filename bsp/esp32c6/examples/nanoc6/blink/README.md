# Blink Example

The simplest possible example - blinks the LED on M5Stack NanoC6.

## Purpose

This example is used to:

- Verify your board is working
- Test your ESP-IDF installation
- Learn basic GPIO control
- Confirm LED pin assignment

## What It Does

- Blinks the built-in LED (GPIO 7) every 1 second
- Prints "LED ON" / "LED OFF" to serial monitor

## How to Use

```bash
cd bsp/esp32c6/examples/nanoc6/blink
idf.py build flash monitor
```

## Expected Output

```
I (123) blink: === V4 RTOS Blink Example ===
I (124) blink: Board: M5Stack NanoC6
I (125) blink: LED pin 7 configured
I (126) blink: Blinking at 1000 ms interval
I (127) blink: LED OFF
I (1127) blink: LED ON
I (2127) blink: LED OFF
I (3127) blink: LED ON
...
```

## Hardware

You should see the LED (next to USB-C port) blinking:

```
NanoC6
┌─────────┐
│ [USB-C] │
│         │
│  [LED]◄─── Blinks here (GPIO 7)
│   ●     │
└─────────┘
```

## Code Explanation

```c
// Configure GPIO as output
gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << LED_PIN),  // GPIO 7
    .mode = GPIO_MODE_OUTPUT,
};
gpio_config(&io_conf);

// Toggle LED
while (1) {
    gpio_set_level(LED_PIN, level);
    level = !level;
    vTaskDelay(pdMS_TO_TICKS(1000));  // 1 second
}
```

## Customization

### Change Blink Speed

Edit `BLINK_PERIOD_MS` in main.c:

```c
#define BLINK_PERIOD_MS  500  // Blink faster (500ms)
```

### Use Different Pin

Edit `board.h` or override in code:

```c
#define MY_LED_PIN  GPIO_NUM_8  // Use GPIO 8 instead
```

## Troubleshooting

### LED doesn't blink

1. **Wrong board?** - This is configured for NanoC6 (LED on GPIO 7)
2. **Power issue?** - Check USB connection
3. **Pin conflict?** - Verify nothing else using GPIO 7

### Build fails

- Source ESP-IDF: `. $HOME/esp/esp-idf/export.sh`
- Check version: `idf.py --version` (need v5.1+)

### Flash fails

- Check USB cable (must support data, not power-only)
- Try: `idf.py -b 115200 flash`
- Erase first: `idf.py erase-flash`

## Next Steps

Once blink works:

- Try **hello-rtos** example for multitasking
- Modify blink pattern (SOS, heartbeat, etc.)
- Add button control

## References

- [NanoC6 Pinout](../../../boards/nanoc6/README.md)
- [ESP-IDF GPIO API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/api-reference/peripherals/gpio.html)
