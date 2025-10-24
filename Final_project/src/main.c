#include "../lib/ADC.h"
#include "../lib/LM35.h"
#include "../lib/UART.h"
#include "../lib/TimerUtil.h"
#include "../lib/PWM.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#define LM35_CHANNEL 0
#define LDR_CHANNEL 1
#define LM35_READING_INTERVAL  1500
#define LDR_READING_INTERVAL  500

volatile uint8_t menu = 0;
const uint8_t totalMenus = 2;
volatile uint8_t selected = 0;

uint32_t lasTimeLM35 = 0;
uint32_t lasTimeLDR = 0;

uint8_t fanStart = 0;

volatile float temperature = 0;
volatile uint8_t temperatureSetValue = 50;

volatile uint16_t ldrValue = 0;
volatile uint16_t ldrSetValue = 100;

void init_peripherals(void)
{
    //  NEXT (PD2 - INT2), PREV (PD3 - INT3), SELECT (PE4 - INT4)
    DDRD &= ~((1 << PD2) | (1 << PD3));
    DDRE &= ~(1 << PE4);

    PORTD |= (1 << PD2) | (1 << PD3); 
    PORTE |= (1 << PE4);             

    EIMSK |= (1 << INT2) | (1 << INT3) | (1 << INT4);

    EICRA |= (1 << ISC21) | (1 << ISC31);
    EICRB |= (1 << ISC41);

    systemTime_init();
    PWM_init();
    ADC_init();
    UART_init(9600);
    I2C_init();
    LCD_init();
    LM35_init(LM35_CHANNEL);
    sei();
}

void displayMenu(uint8_t menuIndex)
{
    LCD_clear();
    switch(menuIndex)
    {
        case 0:
            LCD_print("1. Temperature");
            break;
        case 1:
            LCD_print("2. Light");
            break;
    }
}

void displaySubmenu(uint8_t menuIndex)
{
    LCD_clear();
    switch(menuIndex)
    {
        case 0:
            LCD_print("Set temperature: ");
            LCD_gotoxy(0, 1);
            LCD_printInt(temperatureSetValue);
            LCD_print(" C");
            break;
        case 1:
            LCD_print("Set light value: ");
            LCD_gotoxy(0, 1);
            LCD_printInt(ldrSetValue);
            break;
    }

}

void updateMenuDisplay(void) {
    switch(menu) {
        case 0:
            LCD_gotoxy(0, 1);
            char buf[16];
            dtostrf(temperature, 2, 1, buf);
            LCD_print(buf);
            LCD_print(" C   "); 
            break;
        case 1:
            LCD_gotoxy(0, 1);
            itoa(ldrValue, buf, 10);
            LCD_print(buf);
            LCD_print(" Lux  ");
            break;
    }
}


// ISR NEXT
ISR(INT2_vect)
{
    if(!selected)
    {

        if(menu < totalMenus - 1)
            menu++;
        else
            return;

        displayMenu(menu);
    }
    else
    {
        switch(menu)
        {
            case 0: temperatureSetValue++; break;
            case 1: ldrSetValue++; break;
        }
        displaySubmenu(menu);
    }
}

// ISR PREV 
ISR(INT3_vect)
{
    if(!selected)
    {
        if(menu > 0)
            menu--;
        else
            return;

        displayMenu(menu);
    }
    else
    {

        switch(menu)
        {
            case 0: temperatureSetValue--; break;
            case 1: ldrSetValue--; break;
        }
        displaySubmenu(menu);
    }
}

// ISR SELECT
ISR(INT4_vect)
{
    if(!selected)
    {
        selected = 1;
        displaySubmenu(menu);
    }
    else
    {
        selected = 0;
        displayMenu(menu);

        printString("Temperature: ");
        printInt(temperatureSetValue);
        printString("  Brightness: ");
        printInt(ldrSetValue);
    }
}


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

        if(data_ready)
        {
            data_ready = 0;

            if(strcmp(uart_buffer, "start") == 0)
            {
                adc_enabled = 1;
                printString("Reading started\r\n");
            }
            else if(strcmp(uart_buffer, "stop") == 0)
            {

                if(fanStart)
                {
                    fanStart = 0;
                    setMotorSpeed(0);
                }

                adc_enabled = 0;

                printString("Reading stopped\r\n");
            }
            else if(strcmp(uart_buffer, "status") == 0)
            {
                printString("Reading ");
                printString(adc_enabled ? "ON\r\n" : "OFF\r\n");
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
        
        if(adc_enabled)
        {
            if(currentTime - lasTimeLM35 >= LM35_READING_INTERVAL)
            {
                temperature = LM35_ReadTempC();

                printString("Temperature: ");
                printFloat(temperature, 2);
                printString(" C\r\n");
                printString("--------\r\n");

                lasTimeLM35 = currentTime;

                if((temperature > temperatureSetValue) && !fanStart)
                {
                    setMotorSpeed(230);
                    fanStart = 1;
                    printString("Fan ON\r\n");
                }
                if((temperature < temperatureSetValue - 1) && fanStart)
                {
                    setMotorSpeed(0);
                    fanStart = 0;
                    printString("Fan OFF\r\n");
                }
            }

            if(currentTime - lasTimeLDR >= LDR_READING_INTERVAL)
            {
                ldrValue = ADC_read(LDR_CHANNEL);

                
                lasTimeLDR = currentTime;
            }

            if(ldrValue < ldrSetValue)
                {
                    PORTC |= (1 << PC0);
                }
                else
                {
                    PORTC &= ~(1 << PC0);    
                }
        }

        if (!selected) 
        {
        updateMenuDisplay();
        }   

        _delay_ms(500);
    }
}