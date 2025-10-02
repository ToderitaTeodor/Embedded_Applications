#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART.h"


int main(void)
{
    UART_init(9600);

    printString("Hello world!\r\n");
    printInt(10);

    while(1)
    {

    }
}