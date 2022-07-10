#ifndef GPIO_H_
#define GPIO_H_

typedef enum {
	ledRed_ON,
	ledGreen_ON,
	ledBlue_ON,
	ledsOFF
}led_handler_status_t;

typedef enum {
	pinMUX_ALT_0 = 0b000,
	pinMUX_ALT_1 = 0b001,
	pinMUX_ALT_2 = 0b010,
	pinMUX_ALT_3 = 0b011,
	pinMUX_ALT_4 = 0b100,
	pinMUX_ALT_5 = 0b101,
	pinMUX_ALT_6 = 0b110,
	pinMUX_ALT_7 = 0b111
} pin_mux;

extern void gpioInit(void);

extern void ledRedOn(void);

extern void ledBlueOn(void);

extern void ledGreenOn(void);

extern void ledsOff(void);

extern void setFlagLedOn(led_handler_status_t led_status);

#endif /* GPIO_H_ */
