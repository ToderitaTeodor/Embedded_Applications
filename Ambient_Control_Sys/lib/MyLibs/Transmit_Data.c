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
<<<<<<< Updated upstream:Ambient_Control_Sys/lib/MyLibs/Transmit_Data.c
                transmit_enabled = 1;
                printString("Reading started\r\n");
=======
        
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
>>>>>>> Stashed changes:Ambient_Control_Sys/lib/MyLibs/TransmitData.c
            }
            else if (strcmp(uart_buffer, "stop") == 0)
            {
                transmit_enabled = 0;
<<<<<<< Updated upstream:Ambient_Control_Sys/lib/MyLibs/Transmit_Data.c
                printString("Reading stopped\r\n");
=======
                debug_interface_active = 0;
                setLCDDisplayMode(0);
                printString("Debug interface deactivated\r\n");
>>>>>>> Stashed changes:Ambient_Control_Sys/lib/MyLibs/TransmitData.c
            }
            else if (strcmp(uart_buffer, "status") == 0)
            {
                printString("Reading ");
                printString(transmit_enabled ? "ON\r\n" : "OFF\r\n");
            }
            else if (strcmp(uart_buffer, "help") == 0)
            {
                printString("Available commands (NORMAL MODE):\r\n");
<<<<<<< Updated upstream:Ambient_Control_Sys/lib/MyLibs/Transmit_Data.c
                printString("start\r\nstop\r\nstatus\r\nhelp\r\ntest_mode\r\n");
=======
                printString("start\r\n -> read\r\nstop\r\nstatus\r\nhelp\r\nnext\r\nprev\r\ntest_mode\r\n");
>>>>>>> Stashed changes:Ambient_Control_Sys/lib/MyLibs/TransmitData.c
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
                // for (int i = 0; i < 3; i++)
                // {
                //     PORTC ^= (1 << PC0);
                //     _delay_ms(300);
                // }
                // printString("LED test completed\r\n");
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
                LCD_print("OK");
            }
            else if (strcmp(uart_buffer, "test_motor") == 0)
            {
                // setMotorSpeed(200);
                // _delay_ms(1000);
                // setMotorSpeed(0);
                // printString("Motor test completed\r\n");
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
