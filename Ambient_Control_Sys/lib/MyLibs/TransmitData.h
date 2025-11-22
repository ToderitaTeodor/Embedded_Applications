#ifndef TRANSMIT_DATA_H
#define TRANSMIT_DATA_H

#include <stdint.h>

void UART_debugging(void);
void temperatureTransmit(uint32_t currentTime);
void ldrTransmit(uint32_t currentTime);
void handle_test_logic(uint32_t currentTime);

#endif