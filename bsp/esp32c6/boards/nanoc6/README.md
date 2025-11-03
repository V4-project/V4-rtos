# M5Stack NanoC6 Board Configuration

Board support for the M5Stack NanoC6, a compact ESP32-C6 development board.

## Board Features

- **MCU**: ESP32-C6 (RISC-V 32-bit @ 160 MHz)
- **Memory**: 512 KB SRAM, 4 MB Flash
- **LED**: 1x built-in LED (GPIO7), 1x RGB LED WS2812 (GPIO8)
- **Button**: 1x programmable button (GPIO9)
- **Connectivity**: WiFi 6, Bluetooth 5, IEEE 802.15.4
- **Interface**: USB-C (Serial/JTAG)
- **Expansion**: Grove I2C connector
- **Battery**: Built-in battery connector with monitoring
- **Size**: 24mm x 48mm

## Pinout

```
        NanoC6 Pinout
    ┌─────────────────┐
    │   [USB-C]       │
    │                 │
GPIO2│O               │O GPIO1  (I2C SCL)
GPIO3│O               │O GPIO0  (I2C SDA)
GPIO4│O               │O GND
GPIO5│O               │O 5V
GPIO6│O               │O 3V3
GPIO7│O  [LED]  [RGB] │O GPIO8
    │    [BTN]        │
    │   GPIO9         │
    └─────────────────┘
```

## Pin Assignments

| Pin | Function | Description |
|-----|----------|-------------|
| GPIO0 | I2C SDA | I2C data, Grove connector |
| GPIO1 | I2C SCL | I2C clock, Grove connector |
| GPIO7 | LED | Built-in LED (active high) |
| GPIO8 | RGB LED | WS2812 NeoPixel |
| GPIO9 | BUTTON | User button (active low, pullup) |
| GPIO2-6 | GPIO | General purpose I/O |

## Power

- **USB-C**: 5V input
- **Battery**: JST-PH 2.0 connector (3.7V Li-Po)
- **Battery Monitor**: ADC channel 0 (voltage divider)
- **3.3V Output**: Available on pin header

## Getting Started

### 1. Install ESP-IDF

```bash
. $HOME/esp/esp-idf/export.sh
```

### 2. Build Example

```bash
cd bsp/esp32c6/examples/nanoc6/hello-rtos
idf.py build
```

### 3. Flash

Connect NanoC6 via USB-C:

```bash
idf.py flash monitor
```

## Board-Specific API

The board provides helper functions in `board.h`:

```c
#include "board.h"

void app_main() {
    // Initialize board (LED, button, I2C)
    board_init();

    // Check battery
    uint32_t battery_mv = board_get_battery_voltage_mv();
    printf("Battery: %d mV\n", battery_mv);

    // Check button
    if (board_button_is_pressed()) {
        printf("Button pressed!\n");
    }

    // Blink LED
    gpio_set_mode(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_write(LED_PIN, 1);
}
```

## Troubleshooting

### Board not detected

1. Check USB-C cable (must support data, not power-only)
2. Install USB drivers if needed (Windows)
3. Check device permissions (Linux): `sudo usermod -a -G dialout $USER`

### Flash fails

1. Hold button while plugging in USB to enter download mode
2. Try lower baud rate: `idf.py -b 115200 flash`
3. Erase flash first: `idf.py erase-flash`

### RGB LED not working

NanoC6 uses WS2812 RGB LED on GPIO8. Requires specific driver:

```c
#include "led_strip.h"

led_strip_handle_t strip;
led_strip_config_t config = {
    .strip_gpio_num = RGB_LED_PIN,
    .max_leds = RGB_LED_COUNT,
};
led_strip_new_rmt_device(&config, &rmt_config, &strip);

// Set color (R, G, B)
led_strip_set_pixel(strip, 0, 255, 0, 0);  // Red
led_strip_refresh(strip);
```

## Resources

- [M5Stack NanoC6 Documentation](https://docs.m5stack.com/en/core/M5NanoC6)
- [Schematic](https://static-cdn.m5stack.com/resource/docs/products/core/M5NanoC6/m5nanoc6_sch_v1.0.pdf)
- [ESP32-C6 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-c6_datasheet_en.pdf)

## Examples

Available in `bsp/esp32c6/examples/nanoc6/`:

- **hello-rtos**: Minimal RTOS example with two tasks
- **blink**: LED blink example
- **multitask-demo**: Multiple tasks with message passing (planned)
- **repl-demo**: Interactive Forth REPL (planned)
