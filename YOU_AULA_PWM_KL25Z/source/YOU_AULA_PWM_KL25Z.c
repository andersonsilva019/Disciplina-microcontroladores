/*
 * Fazer com que um LED acenda o seu brilho de um valor mínimo
 * até o valor máximo, e em seguida apague de seu valor máximo até
 * um valor mínimo
 * */

/**
 * @file    YOU_AULA_PWM_KL25Z.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_tpm.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

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

    tpm_config_t tpm2_config;

    TPM_GetDefaultConfig(&tpm2_config);

    tpm2_config.prescale = kTPM_Prescale_Divide_128;

    TPM_Init(TPM2, &tpm2_config);

    CLOCK_SetTpmClock(1);




    while(true) {}
    return 0 ;
}
