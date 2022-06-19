
/**
 * @file    LPTMR_BARE_METAL.c
 * @brief   LPTMR trigger a periodic interrupt every 1 second
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

typedef struct {
	uint32_t CSR; // Control Status Register
	uint32_t PSR; // Prescale Register
	uint32_t CMR; // Compare Register
	uint32_t CNR; // Counter Register
}LPTMRRegs_t;

#define LPTMR ((LPTMRRegs_t *) 0x4004000)

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    //  Enable the LPTMR module clock;
    SIM->SCGC5 |= (1 << 0);

    /*	Configure LPTMR to Timer counter mode by default, use LPO 128K as clock source, bypass the prescaler;
     *
     *	0 Time Counter mode.
     *	1 Pulse Counter mode.
     * */
    LPTMR->CSR &= ~(1 << 1);
    LPTMR->PSR |= (1 << 2);
    LPTMR->PSR &= ~(1 << 1); // SET 0
    LPTMR->PSR |= (1 << 0); // SET 1

    // Set the compare value register to 1 second value;
    LPTMR->CMR |= 0x1F4; // 500 em decimal;

    // Enable timer interrupt;
    LPTMR->CSR |= (1 << 6);

    // Starts the timer counting after all configuration;

    // In the interrupt routine, clear the channel compare flag TCF every 1 second;


}
