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

#define PORT_A ((PORTRegs_t *)0x40049000)
#define PORT_B ((PORTRegs_t *)0x4004A000)

typedef struct{
	uint32_t sc;
	uint32_t cnt;
	uint32_t mod;
	struct {
	    uint32_t CnSC;
	    uint32_t CnV;
	} CONTROLS[6];
	uint8_t RESERVED_0[20];
	uint32_t status;
	uint8_t RESERVED_1[48];
	uint32_t conf;
}TPMR_Regs_t;

#define TPM0_REG ((TPMR_Regs_t *) 0x40038000)
#define TPM1_REG ((TPMR_Regs_t *) 0x40039000)
#define TPM2_REG ((TPMR_Regs_t *) 0x4003A000)

typedef struct{
	uint32_t sopt2;
}SOPT2R_Regs_t;

#define SOPT2_REG ((SOPT2R_Regs_t *) 0x40048004)


#define PTB0_PIN 0 // TMP1-CH0

/*
 * Configuração do canal 1 do módulo TPM1 com a função "PWM".
 *
 */

void delay(){
	for(int i = 0; i < 50000; i++){}
}

void InitTPMPWM(void){

	SIM->SCGC5 = (1 << 10); // Ativar clock da porta B

	PORT_B->PCR[PTB0_PIN] |= (1 << 24);		// ISF=PORTB_PCR18[24]: w1c (limpa a pendência)
	PORT_B->PCR[PTB0_PIN] |= (0b011 << 8);   // Selecionando a alternativa do (TPM1_CH0)


	SIM->SCGC6 = (1 << 25); // Habilitando o TPM1

	//PLL/FLL clock select
	SOPT2_REG->sopt2 &= ~(1 << 16);	//0 MCGFLLCLK clock

	// 01 MCGFLLCLK clock or MCGPLLCLK/2
	SOPT2_REG->sopt2 = (0b01 << 24);	//(TPMSRC) clock source select
										//Selects the clock source for the TPM counter clock
										//01  MCGFLLCLK clock or MCGPLLCLK/2

	//valor do modulo = 20971520 / 128 = 163840 / 50Hz = 3276
	TPM1_REG->mod = 3276;    		// MOD=TPM2_MOD[15:0]=3276


	TPM1_REG->sc = 	(0 << 5)	|			// CPWMS=TPM2_SC[5]=0 (modo de contagem crescente)
					(0b01 << 3)	|           // CMOD=TPM2_SC[4:3]=0b01 (incrementa a cada pulso do LPTPM)
					(0b111 << 0);           		// PS=TPM2_SC[2:0]=0b101 (Fator Prescale = 32)

	/* Desativar o PWM no modulo TPM1 canal 0 --> PTB0 */
	TPM1_REG->CONTROLS[0].CnSC = (0 << 5) | // MSB =TPM2_C0SC[5]=0
								 (0 << 4) | // MSA =TPM2_C0SC[4]=0
								 (0 << 3) | // ELSB=TPM2_C0SC[3]=0
								 (0 << 2);  // ELSA=TPM2_C0SC[2]=0

	/* Ativar o PWM no modulo TPM1 canal 0 --> PTB0 */
	TPM1_REG->CONTROLS[0].CnSC = (1 << 5) | // MSB =TPM2_C0SC[5]=0
								 (0 << 4) | // MSA =TPM2_C0SC[4]=0
								 (1 << 3) | // ELSB=TPM2_C0SC[3]=0
								 (0 << 2);  // ELSA=TPM2_C0SC[2]=0
}


int main(void) {

	// Inicialização do módulo TPM para gerar sinais PWM
	InitTPMPWM();

	while(1){
		// Controla o giro do motor da direção x para y
		for(int i = 58; i < 376; ++i){
			TPM1_REG->CONTROLS[0].CnV = i;
			delay();
		}

		// Controla o giro do motor da direção y para x
		for(int i = 376; i >= 58; --i){
			TPM1_REG->CONTROLS[0].CnV = i;
			delay();
		}
	}

    return 0 ;
}

