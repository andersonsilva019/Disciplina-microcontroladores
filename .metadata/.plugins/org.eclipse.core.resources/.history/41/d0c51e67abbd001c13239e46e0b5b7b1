/**
 * @file    SWITCH_LED_SDK.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

#define pin_led_1 8
#define pin_led_2 13
#define pin_sw_1 12
#define pin_sw_2 4

int main(void) {
	// Configure clocks (Port A, C and D)
	CLOCK_EnableClock(kCLOCK_PortA); // pino 13(LED2) e sw1 pino 12
	CLOCK_EnableClock(kCLOCK_PortC); // pino 8(LED1)
	CLOCK_EnableClock(kCLOCK_PortD); // sw2 pino 4

	// setta porta como GPIO
	PORT_SetMultiplePinsConfig(PORTC, pin_led_1, kPORT_MuxAsGpio); // led 1
	PORT_SetMultiplePinsConfig(PORTA, pin_led_2, kPORT_MuxAsGpio); // led 2

	//setta porta A como gpio de entrada
	port_pin_config_t config = {0};

	config.mux = kPORT_MuxAsGpio;
	config.pullSelect = kPORT_PullDown;

	// configura como entrada
	PORT_SetPinConfig(PORTA, pin_sw_1, &config); // sw1
	PORT_SetPinConfi(PORTD, pin_sw_2, &config); // sw2

	/*
	 * Configure led 1
	 * */

	gpio_pin_config_t led_config_1 = {
		kGPIO_DigitalOutput, // /*!< GPIO direction -> output */
		1
	};

	GPIO_PinInit(PORTC, pin_led_1, &led_config_1);

	/*
	 * Configure led 2
	 * */

	gpio_pin_config_t led_config_2 = {
			kGPIO_DigitalOutput, /* GPIO direction -> output */
			1
	};

	GPIO_PinInit(PORTA, pin_led_2, &led_config_2);

	/*
	 * Configure sw 1
	 * */
	gpio_pin_config_t sw_config_1 = {
		kGPIO_DigitalInput, /* GPIO direction -> input */
		0
	};

	GPIO_PinInit(PORTA, pin_sw_1, &sw_config_1);

	/*
	 * Configure sw 2
	 * */
	gpio_pin_config_t sw_config_2 = {
		kGPIO_DigitalInput, /* GPIO direction -> output */
		0
	};

	GPIO_PinInit(PORTA, pin_sw_2, &sw_config_2);

	// Logic
	while (1) {
		if (GPIO_ReadPinInput(PORTA, pin_sw_1)) {
			GPIO_SetPinsOutput(PORTC, (1 << pin_led_1));
		} else if(GPIO_ReadPinInput(PORTD, pin_sw_2)) {
			GPIO_SetPinsOutput(PORTA, (1 << pin_led_2));
		}else {
			GPIO_ClearPinsOutput(PORTC, (1 << pin_led_1));
			GPIO_ClearPinsOutput(PORTA, (1 << pin_led_2));
		}
	}

    return 0 ;
}
