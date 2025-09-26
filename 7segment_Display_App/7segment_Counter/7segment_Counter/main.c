#include <avr/io.h>
#include <avr/interrupt.h>

void init_devices(void)
{
	// interrupt pin
	
	DDRD &= ~(1 << PD0);
	
	PORTD |= (1 << PD0);
	
	EIMSK |= (1 << INT0);
	
	EICRA |= (1 << ISC01);
	
	// GPIO pin
	
	DDRC |= 0xFF;
}


int main(void)
{
	init_devices();
	
	PORTC |= (1 << PC3) | (1 << PC2);
    
    while (1) 
    {
    }
}

