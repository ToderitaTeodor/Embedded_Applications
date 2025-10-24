#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "PWM.h"

uint8_t fanSpeed = 0;
void buttons_interrupt_init(void)
{
	DDRD &= ~((1 << PD0) | (1 << PD1));
	
	PORTD |= (1 << PD0) | (1 << PD1);
	
	EIMSK |= (1 << INT0) | (1 << INT1);

	EICRA |= (1 << ISC01) | (1 << ISC11);
}

ISR(INT0_vect)
{
	fanSpeed = 100;
}

ISR(INT1_vect)
{
 fanSpeed = 255;
}

uint8_t brightness = 0;

int main(void)
{

  PWM_init();
  DDRB |= (1 << PB5);
  Fan_speed(255);
  while (1)
  {
      
  }

  return 0;
}