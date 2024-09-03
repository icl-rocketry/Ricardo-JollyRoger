// Standard imports
#include <exception>

// Third-party imports
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include <Arduino.h>
#define ARDUINO_LOOP_STACK_SIZE 8192

// Internal imports
#include "system.h"

// Set exceptions flag
/// @todo Integrate into configuration options
static constexpr bool exceptionsEnabled = true;

// Declare handle for loop task
TaskHandle_t loopTaskHandle = NULL;

// Declare system
System ricSystem;

void setup_task()
{
    // Setup core system
    ricSystem.coreSystemSetup();
}

void inner_loop_task()
{
    // Update core system
    ricSystem.coreSystemUpdate();
}

void loopTask(void *pvParameters)
{
    // Initialise system
    setup_task();

    // Enter execution loop
    for (;;)
    {
        // Execute tasks
        inner_loop_task();

        // Delay execution loop
        /// @note this is required to allow the watchdog to be reset, and to allow any other threads to execute
        vTaskDelay(1);
    }
}

extern "C" void app_main()
{
    // Initialise Arduino
    /// @todo Check if this is required
    initArduino();

    // Create looping task
    xTaskCreateUniversal(loopTask, "loopTask", ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, 1);
}
