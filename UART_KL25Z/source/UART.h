#ifndef UART_H_
#define UART_H_

extern void UART0_init(void);

extern void sendDataString(char *string);

extern void sysClockConfig(void);

#endif /* UART_H_ */
