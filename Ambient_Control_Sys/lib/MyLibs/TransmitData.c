#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ADC.h"
#include "LM35.h"
#include "UART.h"
#include "TimerUtil.h"
#include "PWM.h"
#include "global.h"
#include "LCD.h"
#include "UI.h"
#include "SystemInit.h"
#include "LCD.h"

static uint8_t is_night_mode = 0;

typedef enum {
    MODE_NORMAL,
    MODE_TEST
} SystemMode;

SystemMode currentMode = MODE_NORMAL;

void UART_debugging(void)
{
    if (data_ready)
    {
        data_ready = 0;

        if (strcmp(uart_buffer, "test_mode") == 0)
        {
            currentMode = MODE_TEST;
            printString("Switched to TEST MODE\r\n");
        }
        else if (strcmp(uart_buffer, "normal_mode") == 0)
        {
            currentMode = MODE_NORMAL;
            printString("Switched to NORMAL MODE\r\n");
        }
        else if (currentMode == MODE_NORMAL)
        {
            if (strcmp(uart_buffer, "start") == 0)
            {
                debug_interface_active = 1;
                transmit_enabled = 0; 
                setLCDDisplayMode(1);
                printString("Debug interface activated (Ready for 'read')\r\n");
            }
            else if (strcmp(uart_buffer, "read") == 0)
            {
                if (debug_interface_active) 
                {
                    transmit_enabled = 1;
                    setLCDDisplayMode(1);
                    printString("Reading started\r\n");
                }
                else
                {
                    printString("Error: Interface not started. Use 'start' first.\r\n");
                }
            }
            else if (strcmp(uart_buffer, "stop") == 0)
            {
                transmit_enabled = 0;
                debug_interface_active = 0;
                setLCDDisplayMode(0);
                printString("Debug interface deactivated\r\n");
            }
            else if (strcmp(uart_buffer, "status") == 0)
            {
                printString("Reading ");
                printString(transmit_enabled ? "ON\r\n" : "OFF\r\n");
            }
            else if (strcmp(uart_buffer, "help") == 0)
            {
                printString("Available commands (NORMAL MODE):\r\n");
                printString("start\r\n -> read\r\nstop\r\nstatus\r\nhelp\r\nnext\r\nprev\r\ntest_mode\r\n");
            }
            else
            {
                printString("Unknown command. Type 'help' for list.\r\n");
            }
        }
        else if (currentMode == MODE_TEST)
        {
            if (strcmp(uart_buffer, "test_led") == 0)
            {
                for (int i = 0; i < 3; i++)
                {
                    PORTC ^= (1 << PC0);
                    _delay_ms(300);
                }
                printString("LED test completed\r\n");
            }
            else if (strcmp(uart_buffer, "test_temp") == 0)
            {
                printString("Temperature: ");
                printFloat(temperature, 2);
                printString(" C\r\n");
            }
            else if (strcmp(uart_buffer, "test_lcd") == 0)
            {
                LCD_clear();
                LCD_print("LCD Test");
                LCD_gotoxy(0, 1);
                LCD_print("OK   ");
                LCD_print("       ");
                
            }
            else if (strcmp(uart_buffer, "test_motor") == 0)
            {
                DDRB |= (1 << PB4);
                PORTB |= (1 << PB4);
                _delay_ms(1000);
                
                printString("Motor test completed\r\n");
                DDRB &= ~(1 << PB4);
            }
            else if (strcmp(uart_buffer, "help") == 0)
            {
                printString("Available commands (TEST MODE):\r\n");
                printString("test_led\r\ntest_temp\r\ntest_lcd\r\ntest_motor\r\nnormal_mode\r\n");
            }
            else
            {
                printString("Unknown command. Type 'help' for list.\r\n");
            }
        }
    }
}
        
void temperatureTransmit(uint32_t currentTime)
{
    if(currentTime - lasTimeLM35 >= LM35_READING_INTERVAL)
    {
        temperature = LM35_ReadTempC();
        lasTimeLM35 = currentTime;

        // if(transmit_enabled)
        // {
        //     printString("Temperature: ");
        //     printFloat(temperature, 2);
        //     printString(" C\r\n");
        //     printString("--------\r\n");
        // }

        if((temperature > temperatureSetValue) && !fanStart)
        {
            DDRB |= (1 << PB5);
            setMotorSpeed(180);
            fanStart = 1;
            if (transmit_enabled) printString("Fan ON\r\n");
        }
        else if((temperature < temperatureSetValue) && fanStart)
        {
            setMotorSpeed(0);
            fanStart = 0;
            if (transmit_enabled) printString("Fan OFF\r\n");
            DDRB &= ~(1 << PB5);
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

        if (ldrValue < LDR_NIGHT_THRESHOLD && !is_night_mode)
        {
            is_night_mode = 1;           
        }
        else if(ldrValue > LDR_DAY_THRESHOLD && is_night_mode)
        {
            is_night_mode = 0;
            
            LCD_backlight_ON();

            if(transmit_enabled)
            {
                printString("BACK LIGHT IS: ON");
                printString("\r\n");
            }
        }
        
        if(is_night_mode)
        {

            is_idle = 0;

            if((currentTime - lastButtonPressTime) > BACKLIGHT_TIMEOUT_MS)
            {
                LCD_backlight_OFF();

                if(transmit_enabled)
                {
                    printString("BACK LIGHT IS: OFF");
                    printString("\r\n");
                }
            }
        }
       else 
        {
            LCD_backlight_ON(); 

            if((currentTime - lastButtonPressTime) > IDLE_TIMEOUT_MS && !is_idle)
            {
                is_idle = 1;

                LCD_print("Temp: ");
            LCD_gotoxy(6, 0);
            
            
            LCD_print(" C   "); 
            LCD_print("          ");
            }
        }
    }
}
