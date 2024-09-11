/*!
 * @file      apps_common.h
 *
 * @brief     Common functions shared by the examples
 *
 * @copyright
 * The Clear BSD License
 * Copyright Semtech Corporation 2022. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Semtech corporation nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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



// Define the LED pin and port (assuming LED is connected to PA5)
#define LED_PIN LL_GPIO_PIN_5
#define LED_PORT GPIOA

// GPIO Initialization structure
static LL_GPIO_InitTypeDef GPIO_InitStruct;


void LED_Init(void);

void LED_On(void) ;

void LED_Off(void) ;

void LED_Toggle(void) ;

#ifdef __cplusplus
}
#endif

#endif  // APPS_COMMON_H

/* --- EOF ------------------------------------------------------------------ */
