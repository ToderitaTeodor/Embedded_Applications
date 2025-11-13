#include <avr/io.h>
#include <avr/interrupt.h>
#include "UI.h"
#include "global.h"
#include <avr/delay.h>
#include "UART.h"
#include "TimerUtil.h"
#include "LCD.h"

#define BUTTON_PRESSED_TIME 1500

void init_external_interrupts(void)
{
    DDRD &= ~((1 << PD2) | (1 << PD3));
    DDRE &= ~(1 << PE4);

    PORTD |= (1 << PD2) | (1 << PD3); 
    PORTE |= (1 << PE4);             

    EIMSK |= (1 << INT2) | (1 << INT3) | (1 << INT4);

    EICRA |= (1 << ISC21) | (1 << ISC31);
    EICRB |= (1 << ISC41);

}

// ISR NEXT
ISR(INT2_vect)
{
    uint32_t now = sysTime();

    if ((now - lastButtonPressTime) > DEBOUNCE_DELAY_MS)
    {       
        if (!(PIND & (1 << PD2))) 
        {
            lastButtonPressTime = now; 
            

            EIMSK &= ~((1 << INT2) | (1 << INT3) | (1 << INT4)); 

            is_idle = 0;

            LCD_backlight_ON();

            if(!selected)
            {
                if(menu < totalMenus - 1)
                    menu++;
                else
                    return; 
                
                displayMenu(menu);
                updateMenuDisplay();
            }
            else
            {
                switch(menu)
                {
                    case 0: 
                        temperatureSetValue++; 
                        
                        break;
                    case 1: 
                        ldrSetValue++; 
                        break;
                }
                displaySubmenu(menu);
            }
            
            EIMSK |= (1 << INT2) | (1 << INT3) | (1 << INT4);
        }
    }
}

// ISR PREV 
ISR(INT3_vect)
{
    uint32_t now = sysTime(); 

    
    if ((now - lastButtonPressTime) > DEBOUNCE_DELAY_MS)
    { 
        if (!(PIND & (1 << PD3))) 
        {
            lastButtonPressTime = now; 
            
            EIMSK &= ~((1 << INT2) | (1 << INT3) | (1 << INT4)); 

            is_idle = 0;

            LCD_backlight_ON();
            
            if(!selected)
            {
                if(menu > 0)
                    menu--;
                else
                    return; 
                
                displayMenu(menu);
                updateMenuDisplay();
            }
            else
            {
                switch(menu)
                {
                    case 0: 
                        temperatureSetValue--; 

                        if(temperatureSetValue < 10)
                        {
                            temperatureSetValue = 10;
                        }
                      
                        break;
                    case 1: 
                        ldrSetValue--; 
                    
                        break;
                }
                displaySubmenu(menu);
            }
            
            EIMSK |= (1 << INT2) | (1 << INT3) | (1 << INT4);
        }
    }
}

// ISR SELECT
ISR(INT4_vect)
{
    uint32_t now = sysTime(); 

    if ((now - lastButtonPressTime) > DEBOUNCE_DELAY_MS)
    { 
        if (!(PINE & (1 << PE4))) 
        {
            lastButtonPressTime = now; 

            EIMSK &= ~((1 << INT2) | (1 << INT3) | (1 << INT4));

            is_idle = 0;

            LCD_backlight_ON();
            
            if(!selected)
            {
                selected = 1;
                displaySubmenu(menu);
            }
            else
            {
                selected = 0;
                displayMenu(menu);
            }

            EIMSK |= (1 << INT2) | (1 << INT3) | (1 << INT4);
        }
    }
}