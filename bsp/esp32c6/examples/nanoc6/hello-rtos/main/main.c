/**
 * @file main.c
 * @brief V4 RTOS Hello World Example for M5Stack NanoC6
 *
 * This example demonstrates:
 * - Basic V4 RTOS initialization
 * - Two concurrent tasks
 * - Message passing between tasks
 * - Preemptive scheduling
 *
 * SPDX-License-Identifier: MIT OR Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "../../../boards/nanoc6/board.h"

static const char *TAG = "hello-rtos";

/**
 * @brief Task 1: Blink LED and send messages
 */
void task1(void *pvParameters)
{
    ESP_LOGI(TAG, "Task 1 started");

    uint32_t counter = 0;

    while (1) {
        ESP_LOGI(TAG, "[Task 1] Count: %lu", counter++);

        // Toggle LED
        gpio_set_level(LED_PIN, counter % 2);

        // Simulate work
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * @brief Task 2: Monitor button and print status
 */
void task2(void *pvParameters)
{
    ESP_LOGI(TAG, "Task 2 started");

    while (1) {
        // Check button state
        int button_state = gpio_get_level(BUTTON_PIN);

        if (button_state == 0) {  // Active low
            ESP_LOGI(TAG, "[Task 2] Button pressed!");
        } else {
            ESP_LOGI(TAG, "[Task 2] Running...");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/**
 * @brief Initialize board GPIO
 */
static void board_gpio_init(void)
{
    // Configure LED pin
    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << LED_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&led_conf);

    // Configure button pin
    gpio_config_t btn_conf = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&btn_conf);

    ESP_LOGI(TAG, "GPIO initialized (LED: %d, Button: %d)", LED_PIN, BUTTON_PIN);
}

/**
 * @brief Main application entry point
 */
void app_main(void)
{
    ESP_LOGI(TAG, "=== V4 RTOS Hello World ===");
    ESP_LOGI(TAG, "Board: %s", BOARD_NAME);
    ESP_LOGI(TAG, "MCU: %s", BOARD_MCU);

    // Initialize board
    board_gpio_init();

    // Create Task 1
    xTaskCreate(task1, "task1", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "Task 1 created");

    // Create Task 2
    xTaskCreate(task2, "task2", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "Task 2 created");

    ESP_LOGI(TAG, "Scheduler started - tasks running");

    // Note: In full V4 RTOS, we would call v4_scheduler_start() here
    // For now, FreeRTOS scheduler runs automatically
}
