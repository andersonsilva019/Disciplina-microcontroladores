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

#define TPM_REG ((TPMR_Regs_t *) 0x40038000)

typedef struct{
	uint32_t status;
	uint32_t conf;
}TPM_CONF_Regs_t;

#define TPM_CONF_REG ((TPM_CONF_Regs_t *) 0x40038050)

typedef struct{
	uint32_t sopt2;
}SOPT2R_Regs_t;

#define SOPT2_REG ((SOPT2R_Regs_t *) 0x40048004)

void TPM0_IRQHandler(void){
	// Alterando o nível lógico do LED
	GPIO_B->PTOR = (1 << 18);

	// Limpando a flag de interrupção
	TPM_REG->sc |= (1 << 7);

	// Iniciando o contador
	TPM_REG->cnt = 0x0000;
}

int main(void) {

	// Habilitando o clock de 32kHz
	MCG->C1 |= (1 << 1);

	//Selects the clock source for the TPM counter clock
	// 01  MCGFLLCLK clock or MCGPLLCLK/2
	SOPT2_REG->sopt2 |= (0b11 << 24);//clock MCGFLLCLK é de 20,97

	//PLL/FLL clock select
	//Selects the MCGPLLCLK or MCGFLLCLK clock for various peripheral clocking options.
	// 0  MCGFLLCLK clock
	SIM->SCGC6 |= (1 << 24);

	// 0  Disable TOF interrupts. Use software polling or DMA request.
	// 1  Enable TOF interrupts. An interrupt is generated when TOF equals one.
	TPM_REG->sc |= (1 << 6);//ativando a interrupção para TOF

	//(SC)  Prescaler
	TPM_REG->sc |= (0b101 << 0);//ativado o prescaler para 32


	TPM_REG->mod = 8184; // Mod responsável para o contador realizar o overflow em um tempo de 8 segundos.

	TPM_REG->cnt = 0x0000; // resetando o contado.

	//(CMOD) Clock Mode Selection -- 01 : LPTPM counter increments on every LPTPM counter clock
	TPM_REG->sc |= (0b01 << 3);//incrementa a cada pulso do LPTPM

	SIM->SCGC5 |= (1 << 10); // ativar clock porta B

	//Port_B_18 como GPIO
	PORT_B->PCR[18] |= (1 << 8);

	//GPIO_B_18 como saída
	GPIO_B->PDDR |= (1 << 18);

	// Habilitando a interrupção do TPM0 no NVIC
	NVIC_REG->iser[0] |= (1 << 17);

	while(1){}


	return 0 ;
}
