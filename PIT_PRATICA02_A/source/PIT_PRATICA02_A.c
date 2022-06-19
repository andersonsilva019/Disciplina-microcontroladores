#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

typedef struct {
	uint32_t MCR;
	uint32_t LTMR64H;
	uint32_t LTMR64L;
}PIT_Regs_t;

#define PIT_REG ((PIT_Regs_t *)0x40037000)

typedef struct {
	uint32_t LDVAL;
	uint32_t CVAL;
	uint32_t TCTRL;
	uint32_t TFLAG;
}PIT_Chnl_Regs_t;

#define PIT_CH_REG ((PIT_Chnl_Regs_t *)0x40037100)

typedef struct {
	uint32_t PCR[32];
}PORTRegs_t;

#define PORT_B ((PORTRegs_t *)0x4004A000)

typedef struct {
	uint32_t PDOR;
	uint32_t PSOR;
	uint32_t PCOR;
	uint32_t PTOR;
	uint32_t PDIR;
	uint32_t PDDR;
}GPIORegs_t;

#define GPIO_B ((GPIORegs_t *)0x400FF040)

typedef struct {
	uint32_t iser[1];
	uint32_t rsvd[31];
	uint32_t icer[1];
	uint32_t rsvd1[31];
	uint32_t ispr[1];
	uint32_t rsvd2[31];
	uint32_t icpr[1];
	uint32_t rsvd3[31];
	uint32_t rsvd4[64];
	uint32_t ipr[8];
}NVIC_Regs_t;

#define NVIC_REG ((NVIC_Regs_t *)0xE000E100)

/*
 * Timer em N segundos cnt = (CLOCK_GetBusClkFreq) x N
 *
 * Timer em N ms cnt = (CLOCK_GetBusClkFreq / 1000) X N
 *
 * Timer em N us cnt = (CLOCK_GetBusClkFreq / 1000000) x N
 */

#define GET_SEC_COUNT(x) (CLOCK_GetBusClkFreq() * x)

void PIT_IRQHandler(void)
{
	GPIO_B->PTOR = (1 << 18);

	/*
	 * 	Timer Flag Register (PIT_TFLGn)
	 *|   0 -- 30 |  31 |
	 *|      0    | TIF |
	 *|			  | wic |
	 */

	// Reconhece o bit 0 como o 31 do registrador (pág 580)
	PIT_CH_REG->TFLAG = (1 << 0);
}


int main(void) {

    uint32_t clock = CLOCK_GetBusClkFreq();

    //Ativando o Clock enable for PORTB
    /*
	 * 	System Clock Gating Control Register 5 (SIM_SCGC5)
	 *| 31  --  20| 19 | 18 -- 14 | 13 | 12 | 11 | 10 |  9 | 8  7 | 6 |   5 | 4  3  2 | 1 |   0   |
	 *|     0	  |  0 |     0    | PE | PD | PC | PB | PA |   1  | 0 | TSI	|    0    | 0 | LPTMR |
	 */
	SIM->SCGC5 = (1 << 10);

	// PORT_B_18 como GPIO
	PORT_B->PCR[18] = (1 << 8);

	// GPIO_B_18 como Output
	GPIO_B->PDDR = (1 << 18);

	//Ativar o clock do PIT
	/*
	 * 	System Clock Gating Control Register 6 (SIM_SCGC6)
	 *|   31 | 30 |  29 | 28 |  27  |  26  |  25  |  24  |  23 | 22 -- 2 |    1   |  0  |
	 *| DAC0 |  0 | RTC |  0 | ADC0 | TPM2 | TPM1 | TPM0 | PIT |    0    | DMAMUX | FTF |
	 */
	SIM->SCGC6 = (1 << 23);

	// Ativar PIT Module
	// 0 -- Clock for standard PIT timers is enabled
	/*
	 * 	PIT Module Control Register (PIT_MCR)
	 *|   0 -- 28 |     29    |  30  | 31  |
	 *|      0    |  Reserved | MDIS | FRZ |
	 */
	PIT_REG->MCR = 0;

	// Configurando o Time Period
	PIT_CH_REG->LDVAL = GET_SEC_COUNT(8);

	// Ativando o timer interrupt & timer
	/*
	 * 	Timer Control Register (PIT_TCTRLn)
	 *|   0 -- 28 |  29 |  30 |  31 |
	 *|      0    | CHN | TIE | TEN |
	 */

	// Reconhece 31 como 0 e o 30 como 1 (Registrador invertido pág 579)
	PIT_CH_REG->TCTRL = (1 << 1) | (1 << 0); // Isso aqui faz mágica


	//NVIC_EnableIRQ(PIT_IRQn);
	NVIC_REG->iser[0] = (1 << 22);


	while(1){}

    return 0 ;
}
