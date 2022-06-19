//#include <stdio.h>
//#include "board.h"
//#include "peripherals.h"
//#include "pin_mux.h"
//#include "clock_config.h"
//#include "MKL25Z4.h"
//#include "fsl_debug_console.h"
//
//int num_of_toggles = 0;
//
//void PIT_IRQHandler(){
//	//Determine which channel triggered interrupt
//	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
//		GPIOB->PTOR = (1 << 18); //toggle RED
//
//		//Clear interrupt request flag for channel
//		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
//		num_of_toggles++;
//
//		if (num_of_toggles == 10){
//			PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
//			PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
//			num_of_toggles = 0;
//		}
//	}
//}
//
//void init_pit(){
//
//	SIM->SCGC5 = (1 << 10);
//
//	//PORT_B_18 como GPIO
//	PORTB->PCR[18] = (1 << 8);
//
//	// GPIO_B_18 como Output
//	GPIOB->PDDR = (1 << 18);
//
//	//Enable clock to PIT module
//	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable module
//
//	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //enable mdis
//
//	PIT->MCR |= PIT_MCR_FRZ_MASK;
//
//	//Initialize PIT0 to count down from starting_value
//
//	PIT->CHANNEL[0].LDVAL =0x3FFFFF; //every 400ms //No chaining of timers
//
//	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
//
//	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; //Let the PIT channel generate interrupt requests
//
//	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
//
//}
//
//int main (){
//
//}
