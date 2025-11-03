# V4 HAL (Hardware Abstraction Layer)

**Status**: Core Component (Required)

Unified hardware abstraction layer for V4 RTOS using C++17 CRTP for zero-overhead platform independence.

## Features

- Platform-independent GPIO, UART, Timer, Interrupt APIs
- C++17 CRTP (Curiously Recurring Template Pattern)
- Zero runtime overhead
- Multiple platform support (ESP32, POSIX, CH32V)
- Type-safe interfaces

## Structure

```
hal/
├── include/
│   ├── hal_gpio.hpp     # GPIO interface
│   ├── hal_uart.hpp     # UART interface
│   ├── hal_timer.hpp    # Timer interface
│   └── hal_interrupt.hpp # Interrupt interface
├── src/
│   ├── bridge/          # C bridge for VM
│   └── common/          # Common utilities
└── ports/
    ├── posix/           # POSIX simulation
    ├── esp32/           # ESP32-C6/S3
    └── ch32v203/        # CH32V RISC-V
```

## Supported Platforms

- **ESP32-C6/S3**: via ESP-IDF
- **POSIX**: Linux/macOS simulation
- **CH32V203**: RISC-V microcontroller (planned)

## Building

```bash
cmake -B build -DV4_BUILD_HAL=ON
cmake --build build
```

## API

See [HAL API Documentation](../docs/api-reference/hal.md) for complete reference.

### Quick Example

```cpp
#include "hal/ports/esp32/esp32_hal.hpp"

ESP32_GPIO gpio;

void setup() {
    gpio.set_mode(GPIO_NUM_7, GPIO_MODE_OUTPUT);
}

void loop() {
    gpio.toggle(GPIO_NUM_7);
    delay_ms(1000);
}
```

## Design Pattern

```cpp
template<typename Derived>
class GPIO {
public:
    void set_mode(Pin pin, Mode mode) {
        static_cast<Derived*>(this)->impl_set_mode(pin, mode);
    }
};

class ESP32_GPIO : public GPIO<ESP32_GPIO> {
    void impl_set_mode(Pin pin, Mode mode) {
        // ESP32-specific implementation
    }
};
```

## Adding a New Platform

1. Create directory in `ports/myplatform/`
2. Implement CRTP derived classes for each peripheral
3. Add CMakeLists.txt
4. Update platform selection in main CMakeLists.txt

See [Porting Guide](../docs/porting-guide.md) for details.

## Memory Footprint

- Flash: ~5.7 KB
- RAM: ~512 bytes

## License

Dual licensed under MIT or Apache-2.0.
