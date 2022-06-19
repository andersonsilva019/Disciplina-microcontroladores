#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#define RS 0x04 /* PTA2 mask */
#define RW 0x10 /* PTA4 mask */
#define EN 0x20 /* PTA5 mask */

void delayMs(int n);

void LCD_init();

void LCD_command(unsigned char command);

void LCD_data(unsigned char data);


int main(void){

	LCD_init();

	for(;;){
		LCD_command(1); /* clear display */
		delayMs(500);
		LCD_command(0x80); 	/* set cursor at first line */
		LCD_data('H'); 		/* write the word */
		LCD_data('e');
		LCD_data('l');
		LCD_data('l');
		LCD_data('o');
		delayMs(500);
	}
}


void delayMs(int n) {
	int i;
	int j;
	for(i = 0 ; i < n; i++)
		for(j = 0 ; j < 7000; j++) { }
}

void LCD_init(){

	SIM->SCGC5 |= (1 << 12); // Habilitando o clock do portD

	PORTD->PCR[0] |= (1 << 8); // Definindo como GPIO (lIGAR NO D0)
	PORTD->PCR[1] |= (1 << 8); // Definindo como GPIO (lIGAR NO D1)
	PORTD->PCR[2] |= (1 << 8); // Definindo como GPIO (lIGAR NO D2)
	PORTD->PCR[3] |= (1 << 8); // Definindo como GPIO (lIGAR NO D3)
	PORTD->PCR[4] |= (1 << 8); // Definindo como GPIO (lIGAR NO D4)
	PORTD->PCR[5] |= (1 << 8); // Definindo como GPIO (lIGAR NO D5)
	PORTD->PCR[6] |= (1 << 8); // Definindo como GPIO (lIGAR NO D6)
	PORTD->PCR[7] |= (1 << 8); // Definindo como GPIO (lIGAR NO D7)

	GPIOD->PDDR = (1U << 0) | (1U << 1) | (1U << 2) | (1U << 3) | (1U << 4) | (1U << 5) | (1U << 6) | (1U << 7);

	// PINOS DE CONTROLE
	PORTE->PCR[20] |= (1 << 8); // RS
	PORTE->PCR[21] |= (1 << 8); // R/W
	PORTE->PCR[22] |= (1 << 8); // E

	GPIOE->PDDR = (1U << 20) | (1U << 21) | (1U << 22);

	delayMs(30); /* initialization sequence */
	LCD_command(0x30);
	delayMs(10);
	LCD_command(0x30);
	delayMs(1);
	LCD_command(0x30);
	LCD_command(0x38); /* set 8-bit data, 2-line, 5x7 font */
	LCD_command(0x06); /* move cursor right */
	LCD_command(0x01); /* clear screen, move cursor to home */
	LCD_command(0x0F); /* turn on display, cursor blinking */
}

void LCD_command(unsigned char command){
	PTA->PCOR = RS | RW; /* RS = 0, R/W = 0 */
	PTD->PDOR = command;
	PTA->PSOR = EN; /* pulse E */
	delayMs(0);
	PTA->PCOR = EN;
	if (command < 4)
		delayMs(4); /* command 1 and 2 needs up to 1.64ms */
	else
		delayMs(1); /* all others 40 us */
}

void LCD_data(unsigned char data){
	PTA->PSOR = RS; /* RS = 1, R/W = 0 */
	PTA->PCOR = RW;
	PTD->PDOR = data;
	PTA->PSOR = EN; /* pulse E */
	delayMs(0);
	PTA->PCOR = EN;
	delayMs(1);
}


