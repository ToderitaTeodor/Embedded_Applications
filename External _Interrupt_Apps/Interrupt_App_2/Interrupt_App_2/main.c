#include <avr/io.h>

#include <avr/interrupt.h>

#define LED_MASK ((1 << PC0) | (1 << PC2) | (1 << PC4) | (1 << PC6))


void leds_on(void)
{
	
	PORTC |= LED_MASK;
}

void leds_off(void)
{
	PORTC &= ~LED_MASK;
}

void leds_init(void)
{	
	DDRC |= LED_MASK;
	leds_on();
}

void buttons_interrupt_init(void)
{
	DDRD &= ~((1 << PD0) | (1 << PD1));
	
	PORTD |= (1 << PD0) | (1 << PD1);
	
	EIMSK |= (1 << INT0) | (1 << INT1);

	EICRA |= (1 << ISC01) | (1 << ISC11);
}

ISR(INT0_vect)
{
	leds_on();
}

ISR(INT1_vect)
{
	leds_off();
}

int main(void)
{
	leds_init();
	buttons_interrupt_init();
	sei();
	
	while (1){
		
	}
}

