/*!
 * @file      LED_control.h
 *
 * @brief     Common functions shared by the examples
 *
 */

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32l4xx_ll_bus.h"         // For enabling the clock for the GPIO port
#include "stm32l4xx_ll_gpio.h"        // For GPIO operations like setting/resetting pins
#include "stm32l4xx_ll_utils.h"       // For utility functions like delay (LL_mDelay)
#include "stm32l4xx_ll_system.h"      // For system-related utilities (optional, depending on usage)



// Définir les broches et les ports des LEDs
#define LED_RED_PIN LL_GPIO_PIN_5
#define LED_RED_PORT GPIOA

#define LED_BLUE_PIN LL_GPIO_PIN_7
#define LED_BLUE_PORT GPIOB

// Structure d'initialisation GPIO
static LL_GPIO_InitTypeDef GPIO_InitStruct;

void LED_Init(void);

void LED_Red_On(void);

void LED_Red_Off(void);

void LED_Blue_On(void);

void LED_Blue_Off(void);


#ifdef __cplusplus
}
#endif

#endif  // APPS_COMMON_H

/* --- EOF ------------------------------------------------------------------ */
