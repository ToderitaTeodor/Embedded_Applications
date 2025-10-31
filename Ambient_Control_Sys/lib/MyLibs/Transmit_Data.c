#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "ADC.h"
#include "LM35.h"
#include "UART.h"
#include "TimerUtil.h"
#include "PWM.h"
#include "global.h"
#include "LCD.h"
#include "UI.h"
#include "ISR_Handler.h"
#include "System_Init.h"

void UART_debugging(void)
{
    if(data_ready)
    {
        data_ready = 0;

        if(strcmp(uart_buffer, "start") == 0)
        {
            transmit_enabled = 1;
            printString("Reading started\r\n");
        }
        else if(strcmp(uart_buffer, "stop") == 0)
        {
            transmit_enabled = 0;

            printString("Reading stopped\r\n");
        }
        else if(strcmp(uart_buffer, "status") == 0)
        {
            printString("Reading ");
            printString(transmit_enabled ? "ON\r\n" : "OFF\r\n");
        }
        else if(strcmp(uart_buffer, "help") == 0)
        {
            printString("Available commands:\r\n");
            printString("start\r\nstop\r\nstatus\r\nhelp\r\n");
        }
        else
        {
            printString("Unknown command. Type 'help' for list.\r\n");
        }
    }
}

        
void temperatureTransmit(uint32_t currentTime)
{
    if(currentTime - lasTimeLM35 >= LM35_READING_INTERVAL)
    {
        temperature = LM35_ReadTempC();
        lasTimeLM35 = currentTime;

        if(transmit_enabled)
        {
            printString("Temperature: ");
            printFloat(temperature, 2);
            printString(" C\r\n");
            printString("--------\r\n");
        }

        if((temperature > temperatureSetValue) && !fanStart)
        {
            setMotorSpeed(230);
            fanStart = 1;
            if (transmit_enabled) printString("Fan ON\r\n");
        }
        else if((temperature < temperatureSetValue - 1) && fanStart)
        {
            setMotorSpeed(0);
            fanStart = 0;
            if (transmit_enabled) printString("Fan OFF\r\n");
        }
    }
}
        
void ldrTransmit(uint32_t currentTime)
{
    if (currentTime - lasTimeLDR >= LDR_READING_INTERVAL)
    {
        ldrValue = ADC_read(LDR_CHANNEL);
        lasTimeLDR = currentTime;

        if (transmit_enabled)
        {
            printString("LDR value: ");
            printInt(ldrValue);
            printString("\r\n");
        }

        if (ldrValue < ldrSetValue)
            PORTC |= (1 << PC0);
        else
            PORTC &= ~(1 << PC0);
        }
}
