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

#define LM35_CHANNEL 0
#define LDR_CHANNEL 1
#define LM35_READING_INTERVAL  1500
#define LDR_READING_INTERVAL  500


void init_peripherals(void)
{
    systemTime_init();
    
    ADC_init();
    UART_init(9600);
    LM35_init(LM35_CHANNEL);
    sei();
}

uint32_t lasTimeLM35 = 0;
uint32_t lasTimeLDR = 0;


int main(void)
{
    init_peripherals();

    DDRC |= (1 << PC0);

    printString("System ready. Type 'help' for commands.\r\n");

    while (1)
    {
        uint32_t currentTime = sysTime();

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
            if(currentTime - lasTimeLM35 >= LM35_READING_INTERVAL)
            {
                uint16_t temperature = LM35_ReadTempC();

                printString("Temoperature: ");
                printFloat(temperature, 2);
                printString(" C\r\n");
                printString("--------\r\n");

                lasTimeLM35 = currentTime;
            }

            if(currentTime - lasTimeLDR >= LDR_READING_INTERVAL)
            {
                uint16_t ldrValue = ADC_read(LDR_CHANNEL);

                if(ldrValue < 100)
                {
                    PORTC |= (1 << PC0);
                }
                else
                {
                    PORTC &= ~(1 << PC0);    
                }

                printString("LDR analog value: ");
                printInt(ldrValue);
                printString("\r\n");
                printString("--------\r\n");

                lasTimeLDR = currentTime;
            }
        }

        _delay_ms(500);
    }
}