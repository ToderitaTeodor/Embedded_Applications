#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_init(uint32_t baud_rate);
void UART_sendByte(uint8_t data);
void UART_sendString(const char *str);
uint8_t UART_receiveByte(void);

#endif // UART_H