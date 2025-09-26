#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t digit_index = 0;

const uint8_t digits[10] = {
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111  // 9
};

void init_devices(void)
{
	// interrupt pin
	
	DDRD &= ~(1 << PD0);
	
	PORTD |= (1 << PD0);
	
	EIMSK |= (1 << INT0) | (1 << INT1);
	
	EICRA |= (1 << ISC01) | (1 << ISC11);
	
	sei();
	
	// GPIO pin
	
	DDRC = 0xFF;
	PORTC = 0x00;
}

ISR(INT0_vect)
{
	// cycle digits
	
	PORTC = digits[digit_index];
	
	digit_index++;
	
	if(digit_index > 9)
	{
		digit_index = 0;
	}
}

ISR(INT1_vect)
{
	// reset display
	
	PORTC = 0x00;
	
	digit_index = 0;
}


int main(void)
{
	init_devices();
	
	while (1)
	{
		
	}
}

