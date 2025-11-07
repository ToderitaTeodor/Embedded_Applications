#include "LCD.h"
#include "UART.h"
#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

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

void updateMenuDisplay(void) 
{
    char buf[16]; 

    switch(menu) {
        case 0:
            LCD_gotoxy(0, 1);
            dtostrf(temperature, 2, 1, buf);
            LCD_print(buf);
            LCD_print(" C   "); 
            LCD_print("          ");
            break;
        case 1:
            LCD_gotoxy(0, 1);
            itoa(ldrValue, buf, 10);
            LCD_print(buf);
            LCD_print(" Lux  ");
            LCD_print("          ");
            break;
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