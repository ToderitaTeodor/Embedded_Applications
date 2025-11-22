#include "LCD.h"
#include "UART.h"
#include "global.h"
#include "TimerUtil.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

int16_t last_displayed_temp = -9999;
int16_t last_displayed_ldr = -9999;

void displayMenu(uint8_t menuIndex)
{
    LCD_clear();

    last_displayed_temp = -9999;

    switch(menuIndex)
    {
        case 0:
            LCD_print("1. Temperature");
            break;
        case 1:
            LCD_print("2. Offset");
            break;
    }
}

void displaySubmenu(uint8_t menuIndex)
{
    LCD_clear();

    last_displayed_temp = -9999;

    switch(menuIndex)
    {
        case 0:
            LCD_print("Set temperature: ");
            LCD_gotoxy(0, 1);
            LCD_printInt(temperatureSetValue);
            LCD_print(" C");
            
            break;
        case 1:
            LCD_print("Set offset:     ");
            LCD_gotoxy(0, 1);
            LCD_printInt(temperature_offset);
            LCD_print("  C           ");
            break;
    }
}

void updateMenuDisplay(void) 
{
    int16_t temp_to_display;

    int16_t temp_to_int = (int16_t)(temperature * 10);
    temp_to_display = ((temp_to_int + 2) / 5) * 5;

    uint32_t timeSincePress = sysTime() - lastButtonPressTime;
    uint8_t is_timeout = (timeSincePress > BACKLIGHT_TIMEOUT_MS);

    printString("Temperature: ");
    printInt(temp_to_display);
    printString(" C\r\n");
    printString("--------\r\n");

    if(is_night_mode && is_timeout)
    {

        if(last_display_state != 2)
        {
            LCD_clear();
            LCD_backlight_OFF();
            last_display_state = 2;
        }
        return;
    }
    else
    {
        if(last_display_state == 2)
        {
            LCD_backlight_ON;
            last_display_state = 99;
        }
        else
        {
            LCD_backlight_ON();
        }
    }
    
    if (is_idle)
    {
        if (last_display_state != 1)
        {
            LCD_clear();
            LCD_backlight_ON();
            LCD_gotoxy(0, 0);
            LCD_print("Temp: ");
            LCD_print("       C"); 
            LCD_gotoxy(0, 1);
            LCD_print("                ");
            last_display_state = 1;
            last_displayed_temp = -9999;
        }

        if(temp_to_display != last_displayed_temp)
        {
            LCD_gotoxy(6, 0); 
            LCD_printFixed(temp_to_display);
            LCD_print(" ");
            last_displayed_temp = temp_to_display;
        }
    }

    else
    {
        if (last_display_state != 0)
        {
            displayMenu(menu);

            last_display_state = 0;
            last_displayed_temp = -9999;
        }

        switch(menu) 
        {
        case 0:
            if(temp_to_display != last_displayed_temp)
            {
                LCD_gotoxy(0, 1);
                LCD_printFixed(temp_to_display);
                LCD_print(" C   "); 
                LCD_print("          ");
                last_displayed_temp = temp_to_display;
            }
            break;
        case 1:
            LCD_gotoxy(0, 0);
            LCD_print("2. Offset       ");
            LCD_gotoxy(0, 1);
            LCD_print("                ");
            break;
        }
    }  
}

void setLCDDisplayMode(uint8_t mode)
{
    LCD_clear();
    if (mode == 1)
    {
        LCD_gotoxy(0, 0);
        LCD_print("Debugging");
        LCD_gotoxy(0, 1);
        LCD_print("...  ");
        LCD_print("          ");
    }
    else
    {
        displayMenu(menu);
    }
}