#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "UART.h"

#define LINE_MAX 64

int main(void)
{
    UART_init(9600);
    UART_sendString("Embedded Systems Course\r\n");

    char line[LINE_MAX];
    uint8_t idx = 0;

    while (1)
    {
        char c = UART_receiveByte();
        UART_sendByte(c);
    }
}