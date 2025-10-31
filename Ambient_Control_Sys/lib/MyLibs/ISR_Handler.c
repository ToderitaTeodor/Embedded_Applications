#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "UI.h"
#include "global.h"

void ISR_init(void)
{
    //  NEXT (PD2 - INT2), PREV (PD3 - INT3), SELECT (PE4 - INT4)
    DDRD &= ~((1 << PD2) | (1 << PD3));
    DDRE &= ~(1 << PE4);

    PORTD |= (1 << PD2) | (1 << PD3); 
    PORTE |= (1 << PE4);             

    EIMSK |= (1 << INT2) | (1 << INT3) | (1 << INT4);

    EICRA |= (1 << ISC21) | (1 << ISC31);
    EICRB |= (1 << ISC41);

    sei();
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
    }
}