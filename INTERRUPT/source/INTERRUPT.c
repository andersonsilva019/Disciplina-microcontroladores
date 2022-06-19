///*
//
///*
// * Copyright 2016-2022 NXP
// * All rights reserved.
// *
// * Redistribution and use in source and binary forms, with or without modification,
// * are permitted provided that the following conditions are met:
// *
// * o Redistributions of source code must retain the above copyright notice, this list
// *   of conditions and the following disclaimer.
// *
// * o Redistributions in binary form must reproduce the above copyright notice, this
// *   list of conditions and the following disclaimer in the documentation and/or
// *   other materials provided with the distribution.
// *
// * o Neither the name of NXP Semiconductor, Inc. nor the names of its
// *   contributors may be used to endorse or promote products derived from this
// *   software without specific prior written permission.
// *
// * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// */
//
///**
// * @file    INTERRUPT.c
// * @brief   Application entry point.
// */
//
//
//#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MKL25Z4.h"
//#include "fsl_debug_console.h"
//
//
///* TODO: insert other include files here. */
//
///* TODO: insert other definitions and declarations here. */
//
//typedef struct {
//	unint32_t PCR[32];
//	// (ISRF) Each bit in the field indicates the detection of the configured interrupt of the same number as the field.
//	unitn32_t ISRF;
//}PORTRegs_t;
//
//#define pinIn 2
//
//#define PORT_A ((PORTRegs_t *) 0x40049000)
//#define PORT_B ((PORTRegs_t *) 0x4004A000)
//#define PORT_C ((PORTRegs_t *) 0x4004B000)
//#define PORT_D ((PORTRegs_t *) 0x4004C000)
//#define PORT_E ((PORTRegs_t *) 0x4004D000)
//
//
//void PORTD_IRQHandler(void){
//	if(PORT_D->ISFR & (1 << 2)){ // PTD2 causou interrupção?
//
//		// Limpa a flag  de ISR do pino 2 da PORTA D
//		PORT_D->ISFR |= (1 << 2);
//
//		// Faz algo
//	}
//}
//
///*
// * @brief   Application entry point.
// */
//int main(void) {
//
//    /* Init board hardware. */
//    BOARD_InitBootPins();
//    BOARD_InitBootClocks();
//    BOARD_InitBootPeripherals();
//#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
//    /* Init FSL debug console. */
//    BOARD_InitDebugConsole();
//#endif
//
//
//
//    for(;;){
//
//    }
//
//    return 0 ;
//}
