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

    
    DDRC |= (1 << PC0);

    displayMenu(menu);

    while (1)
    {
        uint32_t currentTime = sysTime();

        if(!selected && !debug_interface_active)
        {

            EIMSK &= ~((1 << INT2) | (1 << INT3) | (1 << INT4)); 

            updateMenuDisplay();

            EIMSK |= (1 << INT2) | (1 << INT3) | (1 << INT4); 
        }

        temperatureTransmit(currentTime);
        //ldrTransmit(currentTime);
        UART_debugging();
    }
}