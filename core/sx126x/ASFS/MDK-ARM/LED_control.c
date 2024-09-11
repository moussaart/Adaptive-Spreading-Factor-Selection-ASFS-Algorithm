#include "LEd_control.h"
#include "stm32l4xx_ll_bus.h"         // For enabling the clock for the GPIO port
#include "stm32l4xx_ll_gpio.h"        // For GPIO operations like setting/resetting pins
#include "stm32l4xx_ll_utils.h"       // For utility functions like delay (LL_mDelay)
#include "stm32l4xx_ll_system.h"      // For system-related utilities (optional, depending on usage)



void LED_Init(void) {
    // Enable clock for GPIOA (or the appropriate port)
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    // Configure the LED pin as output
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void LED_On(void) {
    // Set the LED pin to a high level (turn on the LED)
    LL_GPIO_SetOutputPin(LED_PORT, LED_PIN);
}

void LED_Off(void) {
    // Set the LED pin to a low level (turn off the LED)
    LL_GPIO_ResetOutputPin(LED_PORT, LED_PIN);
}

void LED_Toggle(void) {
    // Toggle the LED pin state (turn on if off, and off if on)
    LL_GPIO_TogglePin(LED_PORT, LED_PIN);
}