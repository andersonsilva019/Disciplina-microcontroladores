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

#define PIT_CH_0_REG ((PIT_Chnl_Regs_t *)0x40037100)

#define PIT_CH_1_REG ((PIT_Chnl_Regs_t *)0x40037110)

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

/*
 * Função que realiza o toggle no nível lógico do LED vermelho
 * */
void highLedRed(){
	GPIO_B->PDDR |= (1 << 18);
	GPIO_B->PTOR |= (1 << 18);
}

/*
 * Função que realiza o toggle no nível lógico do LED verde
 * */
void highLedGreen(){
	GPIO_B->PDDR |= (1 << 19);
	GPIO_B->PTOR |= (1 << 19);
}

/*
 * Rotina de interrupção relacionada ao módulo PIT
 * */
void PIT_IRQHandler(void){

	// Verificando se o canal 0 gerou uma interrupção
	if(PIT_CH_0_REG->TFLAG & (1 << 0)){
		// Realizando o processo de toogle do LED verde
		highLedGreen();
		// Limpando a flag de interrupção do canal 0
		PIT_CH_0_REG->TFLAG |= (1 << 0);
	}else{
		// Realizando o processo de toogle do LED verde
		highLedRed();
		// Limpando a flag de interrupção do canal 1
		PIT_CH_1_REG->TFLAG |= (1 << 0);
	}
}


int main(void) {

    uint32_t clock = CLOCK_GetBusClkFreq();

	SIM->SCGC5 = (1 << 10); // Habilitando o clock do portB

	// PORT_B_18 como GPIO
	PORT_B->PCR[18] |= (1 << 8);
	// PORT_B_19 como GPIO
	PORT_B->PCR[19] |= (1 << 8);

	// GPIO_B_18 como Output
	GPIO_B->PDDR |= (1 << 18);

	 // turn off red led.
	GPIO_B->PSOR |= (1 << 18);

	// GPIO_B_19 como Output
	GPIO_B->PDDR |= (1 << 19);

	// turn off green led.
	GPIO_B->PSOR |= (1 << 19);

	SIM->SCGC6 = (1 << 23); // Habilitando o clock do PIT

	// Ativar PIT Module
	PIT_REG->MCR = 0;

	// Configurando o tempo do período que corresponde a 2 segundos
	PIT_CH_0_REG->LDVAL = GET_SEC_COUNT(2);

	// Configurando o tempo do período que corresponde a 6 segundos
	PIT_CH_1_REG->LDVAL = GET_SEC_COUNT(6);

	// Ativando o timer interrupt & timer
	PIT_CH_0_REG->TCTRL = (1 << 1) | (1 << 0); // Isso aqui faz mágica
	PIT_CH_1_REG->TCTRL = (1 << 1) | (1 << 0);

	// Habilitando a interrupção do PIT no NVIC
	NVIC_REG->iser[0] = (1 << 22);

	while(1){}

    return 0 ;
}
