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
#include "Storage.h"

MonitoringState currentMonitor = MONITOR_NONE;
TestState currentTest = TEST_NONE;

static void handle_normal_commands(void);
static void handle_test_commands(void);
static void handle_eeprom_commands(void);

static uint32_t test_timer = 0;
static uint8_t test_step = 0;

void printIntAsFloat(int16_t value)
{
    printInt(value / 10);
    printString(".");
    printInt(abs(value % 10));
}

void UART_debugging(void)
{
    if (data_ready)
    {
        data_ready = 0;

        if (strcmp(uart_buffer, "test_mode") == 0)
        {
            currentMode = MODE_TEST;
            currentMonitor = MONITOR_NONE;

            LCD_clear();
            LCD_print("   TEST  MODE     ");

            printString("Switched to TEST MODE. Type help for commands!\r\n");
            return;
        }
        else if (strcmp(uart_buffer, "normal_mode") == 0)
        {
            currentMode = MODE_NORMAL;
            currentTest = TEST_NONE;

            PORTD &= ~(1 << PD7);
            DDRC &= ~(1 << PC1);
            PORTC &= ~(1 << PC1);

            LCD_clear();
            last_display_state = 99;

            printString("Switched to NORMAL MODE. Type help for commands!\r\n");
            return;
        }

        if(currentMode == MODE_NORMAL)
        {
            handle_normal_commands();
        }
        else
        {
            handle_test_commands();
        }
    }
}   
 
static void handle_normal_commands(void)
{
    if(strcmp(uart_buffer, "temp_read") == 0)
    {
        currentMonitor = MONITOR_TEMP;
        printString("Monitoring: Temperature\r\n");
    }
    else if(strcmp(uart_buffer, "ldr_read") == 0)
    {
        currentMonitor = MONITOR_LDR;
        printString("Monitoring: LDR\n\r");
    }
    else if(strcmp(uart_buffer, "stop") == 0)
    {
        currentMonitor = MONITOR_NONE;
        printString("Monitoring stopped.\r\n");
    }
    else if (strncmp(uart_buffer, "date:", 5) == 0 || strcmp(uart_buffer, "stats") == 0)
    {
        handle_eeprom_commands();
    }
    else if (strcmp(uart_buffer, "help") == 0)
    {
        printString("Normal Cmds: temp_read, ldr_read, stop, date:DD.MM.YYYY, stats, test_mode\r\n");
    }
    else
    {
        printString("Unknown Command. Type help for commands!\r\n");
    }
}

static void handle_eeprom_commands(void)
{
    if (strncmp(uart_buffer, "date:", 5) == 0)
    {
        char *p = uart_buffer + 5;
        int d = atoi(p);
        while(*p && *p != '.') p++; if(*p) p++;
        int m = atoi(p);
        while(*p && *p != '.') p++; if(*p) p++;
        int y = atoi(p);
        
        Storage_SetDate((uint8_t)d, (uint8_t)m, (uint16_t)y);
        printString("Date saved in EEPROM!\r\n");
    }
    else if (strcmp(uart_buffer, "stats") == 0)
    {
        printString("--- SYSTEM STATS ---\r\n");
        
        printString("Install Date: ");
        printInt(g_install_date.day); 
        printString(".");
        printInt(g_install_date.month); 
        printString(".");
        printInt(g_install_date.year); 
        printString("\r\n");
        
        printString("Motor Starts: ");
        
        printInt((uint16_t)g_motor_starts); 
        printString("\r\n");
        
        printString("Max Temp: ");
        printIntAsFloat(g_max_temp); 
        printString(" C\r\n");
        
        printString("Min Temp: ");
        if(g_min_temp == 1000) 
        {
            printString("N/A");
        }
        else
        {
            printIntAsFloat(g_min_temp);
        }
        printString(" C\r\n");

        printString("Temp Offset: ");
        printInt(temperature_offset); 
        printString(" C\r\n");

        printString("--------------------\r\n");
    }
}

static void handle_test_commands(void)
{
    if (strcmp(uart_buffer, "test_lcd") == 0)
    {
        currentTest = TEST_LCD;
        test_step = 0;
        printString("Starting LCD Test...\r\n");
    }

    else if (strcmp(uart_buffer, "test_led") == 0)
    {
        currentTest = TEST_LED_SEQUENCE;
        printString("Starting LED Test...\r\n");
    }
    else if (strcmp(uart_buffer, "test_motor") == 0)
    {
        currentTest = TEST_MOTOR_RUN;
        printString("Starting Motor Test...\r\n");
    }
    else if (strcmp(uart_buffer, "help") == 0)
    {
        printString("Test Cmds: test_led, test_motor, test_lcd, normal_mode\r\n");
    }
    else
    {
        printString("Unknown Test Command. Type help for commands!\r\n");
    }
}

void handle_test_logic(uint32_t currentTime)
{
    if (currentMode != MODE_TEST)
    {
        return;
    } 

    LCD_backlight_ON();
        
    switch (currentTest)
    {
        case TEST_NONE:
            break;

        case TEST_LCD:
            if (test_step == 0) 
            {
                LCD_clear();
                LCD_print("LCD Test OK");
                LCD_gotoxy(0, 1);
                LCD_print("Wait 3s...");
                
                printString("LCD Text Shown. Waiting 3s...\r\n");
                
                test_timer = currentTime; 
                test_step = 1;           
            }
            else if (currentTime - test_timer >= 3000) 
            {
                LCD_clear();

                LCD_print("   TEST  MODE     ");

                currentTest = TEST_NONE; 
                test_step = 0;           
                printString("LCD Test Completed. Menu restored.\r\n");
            }
            break;

        case TEST_LED_SEQUENCE:
        
            if (test_step == 0) {
                
                DDRD |= (1 << PD7);
                PORTD &= ~(1 << PD7);

                test_timer = currentTime;
                test_step = 1;
                PORTD ^= (1 << PD7); 
            }
            else if (currentTime - test_timer >= 300)
            {
                test_timer = currentTime;
                PORTD ^= (1 << PD7); 
                test_step++;
                
                if (test_step >= 6) {
                    currentTest = TEST_NONE; 
                    test_step = 0;
                    PORTD &= ~(1 << PD7);
                    printString("LED test completed\r\n");
                }
            }
            break;

        case TEST_MOTOR_RUN:
           
            if (test_step == 0) {
                DDRC |= (1 << PC1);
                PORTC |= (1 << PC1); 
                test_timer = currentTime;
                test_step = 1;
            }
            else if (currentTime - test_timer >= 1000)
            {
                PORTC &= ~(1 << PC1); 
                DDRC &= ~(1 << PC1);
                currentTest = TEST_NONE; 
                test_step = 0;
                printString("Motor test completed\r\n");
            }
            break;
    }
}
        
void temperatureTransmit(uint32_t currentTime)
{
    if(currentTime - lasTimeLM35 >= LM35_READING_INTERVAL)
    {
        temperature = LM35_ReadTempC() - temperature_offset;
        lasTimeLM35 = currentTime;

        Storage_UpdateMinMax(temperature);

        if (currentMode == MODE_NORMAL) 
        {
            if((temperature > temperatureSetValue) && !fanStart)
            {
                DDRC |= (1 << PC1);  PORTC |= (1 << PC1); 
                DDRD |= (1 << PD7);  PORTD |= (1 << PD7); 
                fanStart = 1;
                Storage_IncrementMotorCount();
    
            }
            else if((temperature < temperatureSetValue) && fanStart)
            {
                DDRC &=  ~(1 << PC1); PORTC &= ~(1 << PC1);
                DDRD &=  ~(1 << PD7); PORTD &= ~(1 << PD7);
                fanStart = 0;
            }
        }

        if(currentMode == MODE_NORMAL && currentMonitor == MONITOR_TEMP)
        {
            printString("Temperature: ");
            printFloat(temperature, 2);
            printString(" C\r\n");
        }
    }
}
        
void ldrTransmit(uint32_t currentTime)
{
    if (currentTime - lasTimeLDR >= LDR_READING_INTERVAL)
    {
        ldrValue = ADC_read(LDR_CHANNEL);
        lasTimeLDR = currentTime;

        if (currentMode == MODE_NORMAL)
        {
            if (ldrValue < LDR_NIGHT_THRESHOLD && !is_night_mode) {
                is_night_mode = 1;            
            }
            else if(ldrValue > LDR_DAY_THRESHOLD && is_night_mode) {
                is_night_mode = 0;
            }
            
            if(!is_night_mode && !is_idle && !selected) {
                if((currentTime - lastButtonPressTime) > IDLE_TIMEOUT_MS) {
                    is_idle = 1;
                }
            }
        }

        if(currentMode == MODE_NORMAL && currentMonitor == MONITOR_LDR)
        {
            printString("LDR value: ");
            printInt(ldrValue);
            printString("\r\n");
        }
   
    }
}
