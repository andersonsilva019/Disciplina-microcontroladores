#include "MKL25Z4.h"
#include "UART.h"

void uartConfigBoundRate(uint32_t baud){
	if(baud == 115200){
		UART0->BDL = 0x1A;
		UART0->BDH = 0x0;
	}else if(baud == 9600){
		UART0->BDL = 0x38;
		UART0->BDH = 0x1;
	}
}

void UART0_init(uint32_t baud) {

	// Select MCGFLLCLK as UART0 clock
	SIM->SOPT2 |= (0b01 << 26);

	// Enable UART0 Clock
	SIM->SCGC4 |= (1 << 10);

	// Enable PORTA clock
	SIM->SCGC5 |= (1 << 9);

	// PTA1 as ALT2 (UART0) -> UART0_RX
	PORTA->PCR[1] |=  (0b010 << 8);

	//PTA2 as ALT2 (UART0) -> UART0_TX
	PORTA->PCR[2] |=  (0b010 << 8);

	// Configure Baud Rate
	uartConfigBoundRate(baud);

	/*
	 * Configure Serial Port
	 *
	 *  Parity Type -> Even parity
	 *  Parity Enable -> No hardware parity generation or checking
	 *  9-Bit or 8-Bit Mode Select -> Receiver and transmitter use 8-bit data characters
	 *  Loop Mode Select -> Normal operation - UART _RX and UART _TX use separate pins
	 *
	 **/
	UART0->C1  = 0x00;

	// Configure Tx/Rx Interrupts
	// Transmit Interrupt
	UART0->C2  &= ~(1 << 7);  			// Tx Interrupt disabled

	// Transmission Complete Interrupt
	UART0->C2  &= ~(1 << 6); 	// Tx Complete Interrupt disabled

	// Receiver Interrupt Enable
	UART0->C2  |= (1 << 5);     // Rx Interrupt enabled

	// Configure Transmitter/Receiever
	UART0->C2  |= (1 << 3);  // Transmitter Enable (Tx)
	UART0->C2  |= (1 << 2);  // Receiver Enable (Rx)

	// Enable UART0 Interrupt
	NVIC_EnableIRQ(UART0_IRQn);
}

/* Delay n milliseconds */
/* The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit(). */
void delayMs(int n) {
	int i;
	int j;
	for(i = 0 ; i < n; i++)
		for (j = 0; j < 7000; j++) {}
}

void sendDataString(char *string){
	while (*string){
		__disable_irq();

		// Transmit Data Register Empty Flag
		while(!(UART0->S1 & (1 << 7))) { }

		// Sending caracter
		UART0->D = *string;

		__enable_irq();

		string++;
	}
}

void sysClockConfig(void){
	MCG->C1 |= (0b00 << 6);  // Select PLL/FLL as clock source

	// MCG_C1: IREFS (bit 2)  = 1
	MCG->C1 |= (1 << 2); // Select Inernal Reference clock
	// source for FLL

	// MCG_C4: DRST_DRS (bit 6-5) = 01
	MCG->C4 |= (0b01 << 5); // Select DCO range as Mid range
	// DCO generates the FLL Clock

	// MCG_C4: DMX32 (bit 7) = 1
	MCG->C4 |= (1 << 7);   // Select DCO frequency as 48Mhz
}
