#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "TimerUtil.h"
#include "global.h"
#include "UI.h"
#include "ISR_Handler.h"
#include "System_Init.h"
#include "Transmit_Data.h"


int main(void)
{
    init_peripherals();

    DDRC |= (1 << PC0);

    DDRB |= (1 << PB5);

    printString("System ready. Type 'help' for commands.\r\n");

    displayMenu(menu);

    while (1)
    {
        uint32_t currentTime = sysTime();

        UART_debugging();
        temperatureTransmit(currentTime);
        ldrTransmit(currentTime);

        if (!selected) 
        {
        updateMenuDisplay();
        }   
    }
}