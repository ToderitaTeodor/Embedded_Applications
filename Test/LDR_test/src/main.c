#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "ADC.h"
#include "UART.h"

int main(void)
{
    UART_init(9600);
    ADC_init();

    sei();

    printString("System ready. Type 'help' for commands.\r\n");

    while (1)
    {
        if(data_ready)
        {
            data_ready = 0;

            if(strcmp(uart_buffer, "adc_on") == 0)
            {
                adc_enabled = 1;
                printString("ADC ENABLED\r\n");
            }
            else if(strcmp(uart_buffer, "adc_off") == 0)
            {
                adc_enabled = 0;
                printString("ADC DISABLED\r\n");
            }
            else if(strcmp(uart_buffer, "adc_status") == 0)
            {
                printString("ADC ");
                printString(adc_enabled ? "ON\r\n" : "OFF\r\n");
            }
            else if(strcmp(uart_buffer, "help") == 0)
            {
                printString("Available commands:\r\n");
                printString("adc_on\r\nadc_off\r\nadc_status\r\nhelp\r\n");
            }
            else
            {
                printString("Unknown command. Type 'help' for list.\r\n");
            }
        }

        if(adc_enabled)
        {
            uint16_t adcValue = ADC_read(0);

            printString("ADC value: ");
            printInt((int)adcValue);
            printString("\r\n");
        }

        _delay_ms(500);
    }
}