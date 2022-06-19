#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL43Z4.h"
#include "fsl_debug_console.h"

typedef struct{
	uint32_t PCR[32];
}PORTRegs_t;

#define PORT_E ((PORTRegs_t *)0x4004D000)

typedef struct{
	uint32_t PDOR;
	uint32_t PSOR;
	uint32_t PCOR;
	uint32_t PTOR;
	uint32_t PDIR;
	uint32_t PDDR;
}GPIORegs_t;

#define GPIO_E ((GPIORegs_t *)0x400FF100)

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
	uint32_t CSR;
	uint32_t PSR;
	uint32_t CMR;
	uint32_t CNR;
}LPTMR_Regs_t;

#define LPTMR_REG ((LPTMR_Regs_t *) 0x40040000)

void LPTMR0_IRQHandler(void){
	GPIO_E->PTOR = (1 << 31);

	// Timer Compare Flag
	//0 The value of CNR is not equal to CMR and increments --
	//1 The value of CNR is equal to CMR and increments.
	LPTMR_REG->CSR = LPTMR_REG->CSR | (1 << 7);
}

int main(void) {
	//1 - Ativar o clock para LPTMR e para porta B/
	SIM->SCGC5 = (1 << 0) | (1 << 13);

	//Port_B_18 como GPIO
	PORT_E->PCR[31] = (1 << 8);

	//GPIO_B_18 como saída
	GPIO_E->PDDR = (1 << 31);

	//Bypass é ativado e clock do LPTMR é LPO
	LPTMR_REG->PSR = (1 << 2) | (1 << 0);

	//TFC -- Timer Free-Running Counter
	//0 -- CNR is reset whenever TCF is set.
	//1 -- CNR is reset on overflow.
	LPTMR_REG->CSR = (1 << 2);

	// Sem prescaler
	//LPO é 1kHz -> T = 1/f = 1/1000 = 1ms. Queremos o
	//tempo de 1s -> compare register = 1/1ms = 1000s
	LPTMR_REG->CMR = 999; // 1000 - 1 = 999

	//TEN -- 0 LPTMR is disabled and internal logic is reset -- 1 LPTMR is enabled
	//TIE -- Timer Interrupt Enable -- 0 Timer interrupt disabled -- 1 Timer interrupt enabled
	LPTMR_REG->CSR = (1 << 0) | (1 << 6);

	NVIC_REG->iser[0] = (1 << 28);

	while(1){}

    return 0 ;
}
