/**
 * @file main.c
 * @brief Simple LED Blink Example for M5Stack NanoC6
 *
 * This is the simplest possible example - just blinks the LED.
 * Use this to verify your board and toolchain are working.
 *
 * SPDX-License-Identifier: MIT OR Apache-2.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "../../../boards/nanoc6/board.h"

static const char *TAG = "blink";

#define BLINK_PERIOD_MS  1000

void app_main(void)
{
    ESP_LOGI(TAG, "=== V4 RTOS Blink Example ===");
    ESP_LOGI(TAG, "Board: %s", BOARD_NAME);

    // Configure LED pin as output
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    ESP_LOGI(TAG, "LED pin %d configured", LED_PIN);
    ESP_LOGI(TAG, "Blinking at %d ms interval", BLINK_PERIOD_MS);

    // Blink loop
    uint32_t level = 0;
    while (1) {
        gpio_set_level(LED_PIN, level);
        level = !level;

        ESP_LOGI(TAG, "LED %s", level ? "ON" : "OFF");

        vTaskDelay(pdMS_TO_TICKS(BLINK_PERIOD_MS));
    }
}
