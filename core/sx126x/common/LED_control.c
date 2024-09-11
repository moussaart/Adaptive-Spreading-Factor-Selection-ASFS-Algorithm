#include "LED_control.h"
#include "stm32l4xx_ll_bus.h"         // For enabling the clock for the GPIO port
#include "stm32l4xx_ll_gpio.h"        // For GPIO operations like setting/resetting pins
#include "stm32l4xx_ll_utils.h"       // For utility functions like delay (LL_mDelay)
#include "stm32l4xx_ll_system.h"      // For system-related utilities (optional, depending on usage)





void LED_Init(void) {
    // Activer les horloges pour les ports GPIOA et GPIOB
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

    // Configurer la broche de la LED rouge (PA5) en sortie
    GPIO_InitStruct.Pin = LED_RED_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);

    // Configurer la broche de la LED bleue (PB6) en sortie
    GPIO_InitStruct.Pin = LED_BLUE_PIN;
    LL_GPIO_Init(LED_BLUE_PORT, &GPIO_InitStruct);
}

void LED_Red_On(void) {
    LL_GPIO_SetOutputPin(LED_RED_PORT, LED_RED_PIN);
}

void LED_Red_Off(void) {
    LL_GPIO_ResetOutputPin(LED_RED_PORT, LED_RED_PIN);
}

void LED_Blue_On(void) {
    LL_GPIO_SetOutputPin(LED_BLUE_PORT, LED_BLUE_PIN);
}

void LED_Blue_Off(void) {
    LL_GPIO_ResetOutputPin(LED_BLUE_PORT, LED_BLUE_PIN);
}
