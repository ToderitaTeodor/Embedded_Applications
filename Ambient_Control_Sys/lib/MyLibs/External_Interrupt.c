#include <avr/io.h>
#include <avr/interrupt.h>
#include "UI.h"
#include "global.h"
#include <avr/delay.h>
#include "UART.h"
#include "TimerUtil.h"

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

// ISR NEXT - PE4 / INT4
ISR(INT2_vect)
{
    uint32_t now = sysTime();

    if ((now - lastButtonPressTime) > DEBOUNCE_DELAY_MS)
    {       
        if (!(PIND & (1 << PD2))) 
        {
            lastButtonPressTime = now; 
            printString("NEXT\n\r");

            
            EIMSK &= ~((1 << INT2) | (1 << INT3) | (1 << INT4)); 
            
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

// ISR PREV - PE5 / INT5
ISR(INT3_vect)
{
    uint32_t now = sysTime(); 

    
    if ((now - lastButtonPressTime) > DEBOUNCE_DELAY_MS)
    { 
        if (!(PIND & (1 << PD3))) 
        {
            lastButtonPressTime = now; 

            EIMSK &= ~((1 << INT2) | (1 << INT3) | (1 << INT4)); 

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

ISR(INT4_vect)
{
    uint32_t now = sysTime(); 

    
    if ((now - lastButtonPressTime) > DEBOUNCE_DELAY_MS)
    { 
        if (!(PINE & (1 << PE4))) 
        {
            lastButtonPressTime = now; 

            EIMSK &= ~((1 << INT2) | (1 << INT3) | (1 << INT4));

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