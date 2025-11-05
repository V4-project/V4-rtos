/**
 * @file esp32c6_ddt_provider.hpp
 * @brief DDT Provider for ESP32-C6 (M5Stack NanoC6)
 *
 * SPDX-License-Identifier: MIT OR Apache-2.0
 */

#ifndef ESP32C6_DDT_PROVIDER_HPP
#define ESP32C6_DDT_PROVIDER_HPP

#include "v4std/ddt.hpp"
#include "v4std/ddt_types.h"

namespace v4rtos
{

/**
 * @brief DDT Provider for ESP32-C6 (M5Stack NanoC6)
 *
 * Provides device descriptors for:
 * - STATUS LED (GPIO7)
 * - USER BUTTON (GPIO9)
 */
class Esp32c6DdtProvider : public v4std::DdtProvider
{
 public:
  v4std::span<const v4dev_desc_t> get_devices() const override;
};

}  // namespace v4rtos

#endif  // ESP32C6_DDT_PROVIDER_HPP
