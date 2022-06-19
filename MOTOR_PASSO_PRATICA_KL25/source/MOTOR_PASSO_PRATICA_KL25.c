#include "MKL25Z4.h"


void delay(int n) {
	for (int i = 0; i < n; i++);
}

int main() {


	SIM->SCGC5   |= (1 << 10); // Habilitando o clock do portB

	PORTB->PCR[0] = (1 << 8); // Definindo como GPIO
	PORTB->PCR[1] = (1 << 8); // Definindo como GPIO
	PORTB->PCR[2] = (1 << 8); // Definindo como GPIO
	PORTB->PCR[3] = (1 << 8); // Definindo como GPIO

	GPIOB->PDDR |= 0xF;

	char steps[] = {0x9, 0x3, 0x6, 0xC}; // Controle das fase do motor

	int cnt = 0, sw = 0, mod = 512; // Variaveis para auxiliar

	while (1) {
		cnt = (cnt + 1) % mod;
		if (cnt == (mod - 1)) sw ^= 1;
		if (sw) {
			for (int i = 0; i < 4; i++) {
				GPIOB->PCOR |= 0xF;
				GPIOB->PDOR |= steps[3 - i];
				delay(10000);
			}
		} else {
			for (int i = 0; i < 4; i++) {
				GPIOB->PCOR |= 0xF;
				GPIOB->PDOR |= steps[i];
				delay(10000);
			}
		}
	}
	return 0;
}
