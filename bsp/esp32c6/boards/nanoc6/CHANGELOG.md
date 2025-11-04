# Changelog - M5Stack NanoC6 Board Support

All notable changes to M5Stack NanoC6 board support will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- `peripherals.h` with LED control helpers (`board_led_on()`, `board_led_off()`, `board_led_toggle()`)
- Button reading helper (`board_button_read()`)
- GPIO initialization helpers for LED, button, and RGB LED
- `board_peripherals_init()` convenience function
- Complete ESP-IDF `sdkconfig` for NanoC6 board
- ESP32-C6 target configuration (4MB flash, USB Serial/JTAG console)
- FreeRTOS 1000Hz tick rate configuration
- ADC calibration for battery voltage monitoring

### Changed
- N/A

### Fixed
- N/A

### Removed
- N/A

## [0.1.0] - 2025-01-03

### Added
- Initial board definition (`board.h`)
- Pin definitions for LED (GPIO7), RGB LED (GPIO8), Button (GPIO9)
- I2C Grove connector pins (GPIO1/GPIO2)
- UART configuration (USB Serial/JTAG)
- Board identification macros
- Memory and clock configuration constants
- Battery voltage reading API declarations

[Unreleased]: https://github.com/V4-project/V4-runtime/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/V4-project/V4-runtime/releases/tag/v0.1.0
