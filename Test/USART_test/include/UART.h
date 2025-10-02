#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_init(uint32_t baud);
void UART_sendByte(uint8_t data);
void printString(const char* str);
void printInt(uint32_t value);

#endif