#include <avr/io.h>

#define LED0_PORT PORTC
#define LED0_PIN PC0

#define LED1_PORT PORTC
#define LED1_PIN PC2

#define BUTTON1_PIN PD0

unsigned long counter = 0;
unsigned long toggle_delay = 900000UL;
unsigned long last_toggle = 0;

void init_devices(void)
{
	DDRC |= (1 << PC0) | (1 << PC2);
	
	DDRD &= ~(1 << PD0);
		
	PORTD |= (1 << PD0);
}

void pinToggle(volatile uint8_t *port, uint8_t pin)
{
	*port ^= (1 << pin);
}

void setPin(volatile uint8_t *port, uint8_t pin)
{
	*port |= (1 << pin);
}

void resetPin(volatile uint8_t *port, uint8_t pin)
{
	*port &= ~(1 << pin);
}


int main(void)
{
	unsigned long counter = 0;
		
    while (1) 
    {
		counter++;
		
		if(counter - last_toggle >= toggle_delay)
		{
			last_toggle = counter;
			pinToggle(&LED0_PORT, LED0_PIN);
		}
		
		if( (PIND & (1 << BUTTON1_PIN)) == 0)
		{		
			setPin(&LED1_PORT, LED1_PIN);
		}
		else
		{
			resetPin(&LED1_PORT, LED1_PIN);
		}
    }
}

