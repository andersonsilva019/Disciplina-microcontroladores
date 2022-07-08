#include <MKL25Z4.h>

#include "UART.h"
#include "GPIO.h"

#define KEY_LED_RED_ON 'a'
#define KEY_LED_BLUE_ON 's'
#define KEY_LED_GREEN_ON 'd'
#define KEY_LEDS_OFF 'w'

static uint8_t isMenuOpen;
static uint8_t isLoggingStarted;

volatile char userInputData;

void menu(void){
	sendDataString("=================================\r\n");
	sendDataString("a. LED RED ON\r\n");
	sendDataString("s. LED BLUE ON\r\n");
	sendDataString("d. LED GREEN ON\r\n");
	sendDataString("w. LED RED, GREE and BLUE OFF\r\n");
	sendDataString("=================================\r\n");
}

void uartControl(void){

	if (!isMenuOpen){
		menu();
		isMenuOpen = 1;
	}

	if (userInputData == KEY_LED_RED_ON){
		if(!isLoggingStarted){
			sendDataString("LED RED ON - keypressed (a) \r\n");
			ledRedOn();
			isLoggingStarted = 1;
		}
	}else if (userInputData == KEY_LED_BLUE_ON){
		if(!isLoggingStarted){
			sendDataString("LED BLUE ON - keypressed (s) \r\n");
			ledBlueOn();
			isLoggingStarted = 1;
		}
	}else if(userInputData == KEY_LED_GREEN_ON){
		if(!isLoggingStarted){
			sendDataString("LED GREEN ON - keypressed (d) \r\n");
			ledGreenOn();
			isLoggingStarted = 1;
		}
	}else if(userInputData == KEY_LEDS_OFF){
		if(!isLoggingStarted){
			sendDataString("LED RED, GREE and BLUE OFF - keypressed (w) \r\n");
			ledsOff();
			isLoggingStarted = 1;
		}
	}

}


void UART0_IRQHandler(void){
	// Receive Data Register Full Flag
	// 1 - Receive data buffer full.
	if ((UART0->S1 & (1 << 5))){
		userInputData = UART0->D;
		isLoggingStarted = 0;
	}
}

int main (void) {

	// Configure System Clock
	sysClockConfig();

	// Initialize GPIO module
	gpioInit();

	// Initialize UART module
	UART0_init();

	while (1) {
		uartControl();
	}
}




