#include <MKL25Z4.h>

#include "UART.h"
#include "GPIO.h"
#include "pwm.h"

#define KEY_LED_RED_ON 'a'
#define KEY_LED_BLUE_ON 's'
#define KEY_LED_GREEN_ON 'd'
#define KEY_LEDS_OFF 'w'
#define KEY_LED_SMOOTHING_MODE 'q'

static uint8_t isMenuOpen;
static uint8_t isLoggingStarted;

volatile char userInputData;
volatile char isLedRedOn;
volatile char isLedGreenOn;
volatile char isLedBlueOn;
volatile uint16_t pwm;

void menu(void){
	sendDataString("================================= \r\n");
	sendDataString("a. LED RED ON \r\n");
	sendDataString("s. LED BLUE ON \r\n");
	sendDataString("d. LED GREEN ON \r\n");
	sendDataString("w. LED RED, GREE and BLUE OFF \r\n");
	sendDataString("q. SELECT SMOOTHING MODE \r\n");
	sendDataString("================================= \r\n");
}

void sleep(int n) {
	for (int i =  0; i < n; ++i)
		for (int j = 0; j < 4800; ++j);
}

void setFlagLedOn(led_handler_status_t led_status){
	switch(led_status){
	case ledRed_ON:
		isLedRedOn = 1;
		isLedGreenOn = 0;
		isLedBlueOn = 0;
		break;
	case ledBlue_ON:
		isLedRedOn = 0;
		isLedGreenOn = 0;
		isLedBlueOn = 1;
		break;
	case ledGreen_ON:
		isLedRedOn = 0;
		isLedGreenOn = 1;
		isLedBlueOn = 0;
		break;
	case ledsOFF:
		isLedRedOn = 0;
		isLedGreenOn = 0;
		isLedBlueOn = 0;
		break;
	}
}


void uartControl(void){

	if (!isMenuOpen){
		menu();
		isMenuOpen = 1;
	}

	switch(userInputData){
	case KEY_LED_RED_ON: {
		if(!isLoggingStarted){
			sendDataString("LED RED ON - keypressed (a) \r\n");
			//ledRedOn();
			ledRedOnWithPWM();

			setFlagLedOn(ledRed_ON);

			isLoggingStarted = 1;


		}
		break;
	}

	case KEY_LED_BLUE_ON: {
		if(!isLoggingStarted){
			sendDataString("LED BLUE ON - keypressed (s) \r\n");
			//ledBlueOn();
			ledBlueOnWithPWM();

			setFlagLedOn(ledBlue_ON);

			isLoggingStarted = 1;

		}
		break;
	}

	case KEY_LED_GREEN_ON: {

		if(!isLoggingStarted){
			sendDataString("LED GREEN ON - keypressed (d) \r\n");
			//ledGreenOn();
			ledGreenOnWithPWM();

			setFlagLedOn(ledGreen_ON);

			isLoggingStarted = 1;

		}
		break;
	}

	case KEY_LEDS_OFF: {
		if(!isLoggingStarted){
			sendDataString("LED RED, GREE and BLUE OFF - keypressed (w) \r\n");
			//ledsOff();
			ledsOffPWM();

			setFlagLedOn(ledsOFF);

			isLoggingStarted = 1;

		}
		break;
	}

	case KEY_LED_SMOOTHING_MODE: {
		if(!isLoggingStarted){
			sendDataString("SMOOTHING MODE- keypressed (q) \r\n");

			if(isLedRedOn){
				while(userInputData == KEY_LED_SMOOTHING_MODE){

					// Decrement PWM
					for(pwm = 0; pwm < TPM2_CH0_MOD; pwm += 81){
						TPM2->CONTROLS[0].CnV = pwm;
						sleep(100);
					}

					// Increment PWM
					for(pwm = 819; pwm > 90; pwm -= 81){
						TPM2->CONTROLS[0].CnV = pwm;
						sleep(100);
					}

					pwm = 0;
				}
			}else if(isLedGreenOn){
				while(userInputData == KEY_LED_SMOOTHING_MODE){

					// Decrement PWM
					for(pwm = 0; pwm < TPM2_CH1_MOD; pwm += 81){
						TPM2->CONTROLS[1].CnV = pwm;
						sleep(100);
					}

					// Increment PWM
					for(pwm = 819; pwm > 90; pwm -= 81){
						TPM2->CONTROLS[1].CnV = pwm;
						sleep(100);
					}

					pwm = 0;
				}
			}else if(isLedBlueOn){
				while(userInputData == KEY_LED_SMOOTHING_MODE){

					// Decrement PWM
					for(pwm = 0; pwm < TPM0_CH1_MOD; pwm += 81){
						TPM0->CONTROLS[1].CnV = pwm;
					}

					// Increment PWM
					for(pwm = 819; pwm > 90; pwm -= 81){
						TPM0->CONTROLS[1].CnV = pwm;
						sleep(100);
					}

					pwm = 0;
				}
			}

			isLoggingStarted = 1;
		}

		break;
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

	// Initialize UART module
	gpioInit();

	//PWM init
	pwmInitModule();

	// Initialize GPIO module
	UART0_init(9600);

	while (1) {
		uartControl();
	}
}




