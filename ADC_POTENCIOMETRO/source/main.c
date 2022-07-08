#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

void adcInitModule(void);

int main(void) {

	BOARD_InitBootPeripherals();
	BOARD_InitBootPins();
	BOARD_InitBootPins();
	BOARD_InitDebugConsole();

	adcInitModule();

	while(1){
		uint32_t data;
		uint32_t voltage;

		/**
		 * ADCH - Input channel select
		 *
		 * Select the ADC input channel using the ADC0_SC1A register
		 *
		 * AD9 is selected as input;
		 */
		ADC0->SC1[0] = (0b01001 << 0);

		// Keep monitoring the end-of-conversion COCO flag in ADC0_SC1A register.
		// When the COCO flag goes HIGH, read the ADC result from the ADC0_RA and save it.
		while((ADC0->SC1[0] & (1 << 7)) == 0);

		/* Ler dados ADC do resultado do registro */
		data = ADC0->R[0];
		// 2^16 = 65535 step size
		/* 0xFFFF => 3.3V, data = que é volt. volt = (data * 3300) / 65535 */
		voltage = (data * 3300) / 65535;

		PRINTF("data: %d voltage: %d\r\n", data, voltage);
	}
	return 0 ;
}

void adcInitModule(void){
	// Enable the clock to I/O pin used by the ADC channel (PORTB)
		SIM->SCGC5 = (1 << 10);

		// Set the PORTX_PCRn MUX bit for ADC
		PORTB->PCR[1] = (0b000 << 8);

		// Enable the clock to ADC0 modules using SIM_SCGC6 register
		SIM->SCGC6 = (1 << 27);

		// Choose the software trigger using the ADC0_SC2 register.
		ADC0->SC2 &= ~(1 << 6);

		/**
		 * Choose clock rate and resolution using ADC0_CFG1 register
		 */
		// Input Clock Select
		ADC0->CFG1 |= (0b01 << 0); // (Bus clock) / 2

		// Conversion mode selection
		ADC0->CFG1 |= (0b11 << 2); // 16-bit conversion

		// Clock Divide Select
		ADC0->CFG1 |= (0b11 << 5); // (input clock)/8

		// Sample time configuration
		ADC0->CFG1 |= (1 << 10); // Long sample time

		/* ADC0 SC3
		 *
		 * Status and Control Register 3 (ADCx_SC3)
		 *| 31  --  8 |   7  |   6  | 5  4 |  3   |   2  | 1  0 |
		 *|     0	  |  CAL | CALF |  0   | ADC0 | AVGE | AVGS |
		 */
		ADC0->SC3 = 0b00;//00 4 samples averaged.
}
