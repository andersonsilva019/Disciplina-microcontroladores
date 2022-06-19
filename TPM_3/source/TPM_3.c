#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

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
	uint32_t sc;
	uint32_t cnt;
	uint32_t mod;
}TPMR_Regs_t;

#define TPM_CH_0_REG ((TPMR_Regs_t *) 0x40038000)

#define TPM_CH_1_REG ((TPMR_Regs_t *) 0x40039000)

typedef struct{
	uint32_t sopt2;
}SOPT2R_Regs_t;

#define SOPT2_REG ((SOPT2R_Regs_t *) 0x40048004)

void highLedRed(){
	GPIO_B->PDDR |= (1 << 18);
	GPIO_B->PTOR |= (1 << 18);
}

void highLedGreen(){
	GPIO_B->PDDR |= (1 << 19);
	GPIO_B->PTOR |= (1 << 19);
}

void TPM0_IRQHandler(void){
	highLedGreen();
	//TPM_CONF_REG->status = TPM_CONF_REG->status | (1 << 8);
	//TPM_CH_0_REG->cnt = 0x0000;
	//(TOF) Timer Overflow Flag -- 1 : LPTPM counter has overflowed.
	TPM_CH_0_REG->sc |= (1 << 7);

	TPM_CH_0_REG->cnt = 0x0000; // resetar
}

void TPM1_IRQHandler(void){
	highLedRed();

	TPM_CH_1_REG->sc |= (1 << 7);

	TPM_CH_1_REG->cnt = 0x0000; // resetar
}

int main(void) {

	// Habilitando o clock de 32kHz
	MCG->C1 |= (1 << 1);

	SIM->SCGC6 |= (1 << 24); // Habilita o clock do TPM0
	SIM->SCGC6 |= (1 << 25); // Habilita o clock do TPM1
	SIM->SCGC5 |= (1 << 10); // Habilita 0 clock da porta_B

	SOPT2_REG->sopt2 |= (0b11 << 24);		//clock MCGFLLCLK é de 20,97

	TPM_CH_0_REG->sc |= (1 << 6);//	Ativando a interrupção para TOF
	TPM_CH_0_REG->sc |= (0b101 << 0);//	Ativado o prescaler para 32
	TPM_CH_0_REG->mod = 2046; // (1024 - 1) * 2 Overflow do contador para 2 segundos; led verde
	TPM_CH_0_REG->cnt = 0x0000; // Resetar
	TPM_CH_0_REG->sc |= (0b01 << 3);//	Incrementa a cada pulso do LPTPM

	TPM_CH_1_REG->sc |= (1 << 6);	//	Ativando a interrupção para TOF
	TPM_CH_1_REG->sc |= (0b101 << 0);	//	Ativado o prescaler para 32
	TPM_CH_1_REG->mod = 6138; 	// (1024 - 1) * 6 Overflow do contador para 6 segundos; led green
	TPM_CH_1_REG->cnt = 0x0000; // resetar
	TPM_CH_1_REG->sc |= (0b01 << 3);

	PORT_B->PCR[18] |= (1 << 8); // Definindo o Port_B_18 como GPIO
	PORT_B->PCR[19] |= (1 << 8); // Definindo o Port_B_19 como GPIO

	GPIO_B->PDDR |= (1 << 18); // GPIO_B_18 como Output
	GPIO_B->PSOR |= (1 << 18); // turn off red led.

	GPIO_B->PDDR |= (1 << 19);
	GPIO_B->PSOR |= (1 << 19);  // turn off green led.

	NVIC_REG->iser[0] |= (1 << 17); // Habilita a interrupção do TMP0 no NVIC
	NVIC_REG->iser[0] |= (1 << 18);	// Habilita a interrupção do TMP1 no NVIC

	while(1){}

	return 0 ;
}
