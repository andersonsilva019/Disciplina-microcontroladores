/*
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    INTERRUPT.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"


/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define PIN_SW 2
#define PIN_LED 13

void PORTD_IRQHandler(void){
	if(PORT_GetPinsInterruptFlags(PORTD) & (1 << 2)){ // PTD2 causou interrupção?

		// Limpa a flag  de ISR do pino 2 da PORTA D
		PORT_ClearPinsInterruptFlags(PORTD, 1 << 2);

		GPIO_TogglePinsOutput(GPIOB, 1 << 19);
	}
}

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    // Interrupt
    CLOCK_EnableClock(kCLOCK_PortD); // pino 4(SW) -> D2
    CLOCK_EnableClock(kCLOCK_PortA); // pino 13(LED) -> D8

    PORT_SetPinMux(PORTD, PIN_SW, kPORT_MuxAsGpio);
    PORT_SetPinMux(PORTA, PIN_LED, kPORT_MuxAsGpio);

    gpio_pin_config_t sw_config = {
    		kGPIO_DigitalInput,
			0
    };

    gpio_pin_config_t led_config = {
        	kGPIO_DigitalOutput,
    		1
    };

    GPIO_PinInit(GPIOD, PIN_SW, &sw_config);
    GPIO_PinInit(GPIOA, PIN_LED, &led_config);

    PORT_SetPinInterruptConfig(PORTD, PIN_SW, kPORT_InterruptFallingEdge);

    NVIC_EnableIRQ(PORTD_IRQn);

    while(1){

    }

    return 0 ;
}
