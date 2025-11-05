/**
 * @file esp32c6_ddt_provider.cpp
 * @brief DDT Provider for ESP32-C6 (M5Stack NanoC6)
 *
 * SPDX-License-Identifier: MIT OR Apache-2.0
 */

#include "esp32c6_ddt_provider.hpp"

extern "C"
{
#include "nanoc6/board.h"
}

namespace v4rtos
{

v4std::span<const v4dev_desc_t> Esp32c6DdtProvider::get_devices() const
{
  // Device descriptor table for M5Stack NanoC6
  static constexpr v4dev_desc_t devices[] = {
      // STATUS LED (GPIO7, active-high)
      {
          .kind = V4DEV_LED,
          .role = V4ROLE_STATUS,
          .index = 0,
          .flags = 0,  // Active-high (no V4DEV_FLAG_ACTIVE_LOW)
          .handle = LED_PIN,
      },
      // USER BUTTON (GPIO9, active-low with pullup)
      {
          .kind = V4DEV_BUTTON,
          .role = V4ROLE_USER,
          .index = 0,
          .flags = V4DEV_FLAG_ACTIVE_LOW,
          .handle = BUTTON_PIN,
      },
      // Future: Add RGB LED (GPIO8, WS2812) once RGB support is implemented
      // Future: Add I2C, UART, ADC devices as needed
  };

  return v4std::span<const v4dev_desc_t>{devices, 2};
}

}  // namespace v4rtos
