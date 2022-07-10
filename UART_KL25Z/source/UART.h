#ifndef UART_H_
#define UART_H_

extern void UART0_init(uint32_t baud);

extern void sendDataString(char *string);

extern void sysClockConfig(void);

extern void uartConfigBoundRate(uint32_t baud);

#endif /* UART_H_ */
