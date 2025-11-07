#include <avr/io.h>
#include <avr/interrupt.h>
#include "UI.h"
#include "global.h"
#include <avr/delay.h>
#include "UART.h"
#include "TimerUtil.h"

void ISR_init(void)
{
    // NEXT (PE4 - INT4), PREV (PE5 - INT5)
    DDRE &= ~((1 << PE4) | (1 << PE5)); 

    PORTE |= (1 << PE4) | (1 << PE5);  

    // Falling edge
    EICRB |= (1 << ISC41) | (1 << ISC51);
    EICRB &= ~((1 << ISC40) | (1 << ISC50));

    EIMSK |= (1 << INT4) | (1 << INT5);
}

// ISR NEXT - PE4 / INT4
ISR(INT4_vect)
{
    uint32_t now = sysTime();

    if ((now - lastButtonPressTime) > DEBOUNCE_DELAY_MS)
    {       
        if (!(PINE & (1 << PE4))) 
        {
            lastButtonPressTime = now; 
            printString("NEXT\n\r");

            
            EIMSK &= ~((1 << INT4) | (1 << INT5)); 
            
            if(!selected)
            {
                if(menu < totalMenus - 1)
                    menu++;
                else
                    menu = 0; 
                
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
            
            EIMSK |= (1 << INT4) | (1 << INT5);
        }
    }
}

// ISR PREV - PE5 / INT5
ISR(INT5_vect)
{
    uint32_t now = sysTime(); 

    
    if ((now - lastButtonPressTime) > DEBOUNCE_DELAY_MS)
    { 
        if (!(PINE & (1 << PE5))) 
        {
            lastButtonPressTime = now; 

            EIMSK &= ~((1 << INT4) | (1 << INT5)); 

            if(!selected)
            {
                if(menu > 0)
                    menu--;
                else
                    menu = totalMenus - 1; 
                
                displayMenu(menu);
                updateMenuDisplay();
            }
            else
            {
                switch(menu)
                {
                    case 0: 
                        temperatureSetValue--; 
                      
                        break;
                    case 1: 
                        ldrSetValue--; 
                        
                        break;
                }
                displaySubmenu(menu);
            }
            
            EIMSK |= (1 << INT4) | (1 << INT5);
        }
    }
}