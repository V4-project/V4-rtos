# Board Support Packages (BSP)

BSPs provide platform and board-specific integration for V4 RTOS.

## Hybrid BSP Design

V4 RTOS uses a hybrid approach separating chip-level and board-level configurations:

### Chip Level (Common)

Shared across all boards using the same chip:

- HAL integration with platform SDK (ESP-IDF, etc.)
- Linker scripts and memory layout
- Core peripheral initialization

### Board Level (Specific)

Board-specific configurations:

- Pin assignments (`board.h`)
- Peripheral configurations
- SDK configuration files (`sdkconfig`)
- Board-specific examples

## Structure

```
bsp/
├── esp32c6/                     # ESP32-C6 chip family
│   ├── README.md               # Chip-specific docs
│   ├── hal_integration/        # HAL → ESP-IDF bridge
│   │   ├── CMakeLists.txt
│   │   └── v4_hal_esp32c6.c
│   ├── linker/                 # Memory layout
│   │   └── esp32c6.ld
│   ├── boards/                 # Board configurations
│   │   ├── nanoc6/             # M5Stack NanoC6
│   │   │   ├── board.h
│   │   │   ├── sdkconfig
│   │   │   └── README.md
│   │   └── devkit/             # ESP32-C6-DevKit
│   │       ├── board.h
│   │       ├── sdkconfig
│   │       └── README.md
│   └── examples/               # Board-specific examples
│       ├── nanoc6/
│       │   ├── hello-rtos/
│       │   ├── blink/
│       │   └── multitask-demo/
│       └── devkit/
│           └── hello-rtos/
├── esp32s3/                    # ESP32-S3 (future)
└── ch32v203/                   # CH32V RISC-V (future)
```

## Supported Platforms

| Platform | Chip | Status | Boards |
|----------|------|--------|--------|
| ESP32-C6 | ESP32-C6 | Stable | NanoC6, DevKit |
| ESP32-S3 | ESP32-S3 | Planned | CoreS3, AtomS3 |
| CH32V203 | CH32V203 | Planned | BluePill-Plus |

## Quick Start

### ESP32-C6 NanoC6

```bash
cd bsp/esp32c6/examples/nanoc6/hello-rtos
idf.py build flash monitor
```

## Adding a New Board

### For Existing Chip

If the chip is already supported (e.g., ESP32-C6):

1. Create board directory: `bsp/esp32c6/boards/myboard/`
2. Create `board.h` with pin definitions
3. Copy and customize `sdkconfig`
4. Create README.md
5. Add examples in `bsp/esp32c6/examples/myboard/`

Example `board.h`:

```c
#ifndef BOARD_MYBOARD_H
#define BOARD_MYBOARD_H

#define BOARD_NAME "My Custom Board"
#define LED_PIN    GPIO_NUM_8
#define BUTTON_PIN GPIO_NUM_9

#endif
```

### For New Chip

1. Create chip directory: `bsp/mychip/`
2. Implement HAL integration in `hal_integration/`
3. Add linker script in `linker/`
4. Create first board configuration
5. Add examples
6. Update main README.md

See [Porting Guide](../docs/porting-guide.md) for detailed instructions.

## Board Configuration Files

### board.h

Pin assignments and board identification:

```c
#define BOARD_NAME "M5Stack NanoC6"
#define LED_PIN         GPIO_NUM_7
#define RGB_LED_PIN     GPIO_NUM_8
#define BUTTON_PIN      GPIO_NUM_9
#define I2C_SDA_PIN     GPIO_NUM_1
#define I2C_SCL_PIN     GPIO_NUM_2
```

### sdkconfig

Platform SDK configuration (ESP-IDF, etc.):

- CPU frequency
- Flash size
- UART settings
- WiFi/Bluetooth configuration (if applicable)
- Component options

## Building for Different Boards

### ESP-IDF (ESP32)

```bash
cd bsp/esp32c6/examples/nanoc6/hello-rtos
idf.py set-target esp32c6
idf.py build
```

### Generic CMake

```bash
cmake -B build -DBOARD=nanoc6
cmake --build build
```

## Examples

Each board should provide at least:

- **hello-rtos**: Minimal RTOS example
- **blink**: LED blink (hardware test)

Optional examples:

- **multitask-demo**: Multiple tasks
- **repl-demo**: Interactive shell
- **link-demo**: OTA update
- **peripheral-demo**: Board-specific peripherals

## Memory Layouts

Different boards may have different memory configurations. Check the linker script for your board:

```
bsp/<chip>/linker/<chip>.ld
```

Example for ESP32-C6:

```
MEMORY
{
  iram_seg (RWX) : ORIGIN = 0x40800000, LENGTH = 0x20000  /* 128KB */
  dram_seg (RW)  : ORIGIN = 0x3FC80000, LENGTH = 0x50000  /* 320KB */
  flash_seg (RX) : ORIGIN = 0x42000000, LENGTH = 0x400000 /* 4MB */
}
```

## License

All BSP code is dual-licensed under MIT or Apache-2.0.
