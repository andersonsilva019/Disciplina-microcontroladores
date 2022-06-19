/**
 * @file    LPTimer.c
 * @brief   Application entry point.
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

typedef struct{
	uint32_t PCR[32];
}PORTRegs_t;

#define PORT_B ((PORTRegs_t *)0x4004A000)

typedef struct{
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

typedef struct{
	uint32_t tcsr;
	uint32_t prescale;
	uint32_t compare;
	uint32_t count;
}LPTMR_Regs_t;

#define LPTMR_REG ((LPTMR_Regs_t *) 0x40040000)

void LPTMR0_IRQHandler(void){
	GPIO_B->PTOR = (1 << 18);

	// Timer Compare Flag
	//0 The value of CNR is not equal to CMR and increments --
	//1 The value of CNR is equal to CMR and increments.
	LPTMR_REG->tcsr = LPTMR_REG->tcsr | (1 << 7);
}

/*
 * @brief   Application entry point.
 */
int main(void) {
	/*1 - Ativar o clock para LPTMR e para porta B*/
	SIM->SCGC5 = (1 << 0) | (1 << 10);

	//Port_B_18 como GPIO
	PORT_B->PCR[18] = (1 << 8);

	//GPIO_B_18 como saída
	GPIO_B->PDDR = (1 << 18);

	//Bypass é ativado e clock do LPTMR é LPO
	LPTMR_REG->prescale = (1 << 2) | (1 << 0);

	//LPO é 1kHz -> T = 1/f = 1/1000 = 1ms. Queremos o tempo de 1s -> compare register = 1/1ms = 1000s
	LPTMR_REG->compare = 999;

	//Ativar Timer Enable -- 0 LPTMR is disabled and internal logic is reset -- 1 LPTMR is enabled
	//Timer Interrupt Enable -- 0 Timer interrupt disabled -- 1 Timer interrupt enabled
	LPTMR_REG->tcsr = (1 << 0) | (1 << 6);

	//NVIC_EnableIRQ(LPTMR0_IRQn);
	NVIC->REG->iser[0] = (1 << 28);

	while(1){
	}

    return 0 ;
}
