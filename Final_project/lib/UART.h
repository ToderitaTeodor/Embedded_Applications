#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_init(uint32_t baud_rate);
void UART_sendByte(uint8_t data);
void printString(const char *str);
void printFloat(float value, uint8_t decimals);
void printInt(uint32_t value);

extern volatile char uart_buffer[];
extern volatile uint8_t uart_index;
extern volatile uint8_t data_ready;

extern volatile uint8_t transmit_enabled;

#endif