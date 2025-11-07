#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "ADC.h"
#include "UART.h"
#include "LM35.h"
#include "TimerUtil.h"
#include "TransmitData.h"
#include "LCD.h"
#include "UI.h"
#include "global.h"
#include "ISR_Handler.h"
#include "SystemInit.h"


int main(void)
{
    init_peripherals();
    sei();
    ISR_init();

    displayMenu(menu);

    while (1)
    {
        uint32_t currentTime = sysTime();

        if(!selected && !transmit_enabled)
        {

            EIMSK &= ~((1 << INT4) | (1 << INT5)); 

            updateMenuDisplay();

            EIMSK |= (1 << INT4) | (1 << INT5); 
        }

        temperatureTransmit(currentTime);
        UART_debugging();
    }
}