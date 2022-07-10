#include "MKL25Z4.h"
#include "GPIO.h"

void gpioInit(){
	SIM->SCGC5 = (1 << 10) | (1 << 12); // Enable PORT B clock and PORT D.

	PORTB->PCR[18] |= (1 << 24) | (0b011 << 8); // Set port B, pin 18 as GPIO (red led). (TPM2_CH0 - alt3)
	PORTB->PCR[19] |= (1 << 24) | (0b011 << 8); // Set port B, pin 19 as GPIO (green led). (TPM2_CH1 - alt3)
	PORTD->PCR[1]  |= (1 << 24) | (0b100 << 8); // Set port D, pin 1 as GPIO (blue led). (TPM0_CH1 - alt4)

	GPIOB->PDDR |= (1 << 18); // GPIO_B pin 18 as output.
	GPIOB->PSOR |= (1 << 18); // turn off red led.

	GPIOB->PDDR |= (1 << 19); // GPIO_B pin 19 as output.
	GPIOB->PSOR |= (1 << 19);  // turn off green led.

	GPIOD->PDDR |= (1 << 1);  // GPIO_D pin 1 as output.
	GPIOD->PSOR |= (1 << 1);  // turn off blue led.
}

void ledRedOn(){
	GPIOB->PCOR |= (1 << 18); // turn on red
	GPIOB->PSOR |= (1 << 19); // turn off green
	GPIOD->PSOR |= (1 << 1);  // turn off blue
}

void ledGreenOn(){
	GPIOB->PSOR |= (1 << 18); // turn off red
	GPIOB->PCOR |= (1 << 19); // turn on green
	GPIOD->PSOR |= (1 << 1);  // turn off blue
}

void ledBlueOn(){
	GPIOB->PSOR |= (1 << 18); // turn off red
	GPIOB->PSOR |= (1 << 19); // turn off green
	GPIOD->PCOR |= (1 << 1);  // turn on blue
}

void ledsOff(){
	GPIOB->PSOR |= (1 << 18); // turn off red
	GPIOB->PSOR |= (1 << 19); // turn on green
	GPIOD->PSOR |= (1 << 1);  // turn off blue
}
