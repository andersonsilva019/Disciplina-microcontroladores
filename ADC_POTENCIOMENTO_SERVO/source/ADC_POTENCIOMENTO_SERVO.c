#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#define MIN_VALUE_SERVO 58
#define MAX_VALUE_SERVO 327
#define SERVO_PIN 20 // TMP1-CH0 PTE20

void delay(void);

void initTPMPWM(void);

void adcInitModule(void);

int main(void) {

	uint32_t result = 0;
	uint32_t step_motor = 0;

	BOARD_InitBootPeripherals();
	BOARD_InitBootPins();
	BOARD_InitBootPins();
	BOARD_InitDebugConsole();

	initTPMPWM();

	adcInitModule();

	while(1){

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
		result = ADC0->R[0];

		// 2^16 = 65535 step size
		step_motor = (result * MAX_VALUE_SERVO) / 65535;

		TPM1->CONTROLS[0].CnV = step_motor + MIN_VALUE_SERVO;

		PRINTF("STEP MOTOR %d\r\n", step_motor + MIN_VALUE_SERVO);
	}

	return 0 ;
}

void delay(void){
	for(int i = 0; i < 50000; i++){}
}

void initTPMPWM(void){

	SIM->SCGC5 |= (1 << 13); // Ativar clock da porta B

	PORTE->PCR[SERVO_PIN] |= (1 << 24);		// ISF=PORTB_PCR18[24]: w1c (limpa a pendência)

	PORTE->PCR[SERVO_PIN] |= (0b011 << 8);   // Selecionando a alternativa do (TPM1_CH0)


	SIM->SCGC6 |= (1 << 25); // Habilitando o TPM1

	//PLL/FLL clock select
	SIM->SOPT2 &= ~(1 << 16);	//0 MCGFLLCLK clock

	// 01 MCGFLLCLK clock or MCGPLLCLK/2
	SIM->SOPT2 |= (0b01 << 24);	//(TPMSRC) clock source select
	//Selects the clock source for the TPM counter clock
	//01  MCGFLLCLK clock or MCGPLLCLK/2

	//valor do modulo = 20971520 / 128 = 163840 / 50Hz = 3276
	TPM1->MOD |= 3276;    		// MOD=TPM2_MOD[15:0]=3276


	TPM1->SC |= 	(0 << 5)	|			// CPWMS=TPM2_SC[5]=0 (modo de contagem crescente)
			(0b01 << 3)	|           // CMOD=TPM2_SC[4:3]=0b01 (incrementa a cada pulso do LPTPM)
			(0b111 << 0);           		// PS=TPM2_SC[2:0]=0b101 (Fator Prescale = 32)

	/* Desativar o PWM no modulo TPM1 canal 0 --> PTB0 */
	TPM1->CONTROLS[0].CnSC |= (0 << 5) | // MSB =TPM2_C0SC[5]=0
			(0 << 4) | // MSA =TPM2_C0SC[4]=0
			(0 << 3) | // ELSB=TPM2_C0SC[3]=0
			(0 << 2);  // ELSA=TPM2_C0SC[2]=0

	/* Ativar o PWM no modulo TPM1 canal 0 --> PTB0 */
	TPM1->CONTROLS[0].CnSC |= (1 << 5) | // MSB =TPM2_C0SC[5]=0
			(0 << 4) | // MSA =TPM2_C0SC[4]=0
			(1 << 3) | // ELSB=TPM2_C0SC[3]=0
			(0 << 2);  // ELSA=TPM2_C0SC[2]=0
}


void adcInitModule(void){
	// Enable the clock to I/O pin used by the ADC channel (PORTB)
	SIM->SCGC5 |= (1 << 10);

	// Set the PORTX_PCRn MUX bit for ADC
	PORTB->PCR[1] |= (0b000 << 8);

	// Enable the clock to ADC0 modules using SIM_SCGC6 register
	SIM->SCGC6 |= (1 << 27);

	// Choose the software trigger using the ADC0_SC2 register.
	ADC0->SC2 &= ~(1 << 6);

	/**
	 * Choose clock rate and resolution using ADC0_CFG1 register
	 */
	// Input Clock Select
	ADC0->CFG1 |= (0b00 << 0); // (Bus clock)

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
	ADC0->SC3 |= 0b00;//00 4 samples averaged.
}
