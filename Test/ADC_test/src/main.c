#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "ADC.h"



int main(void)
{
  DDRC |= (1 << PC0);

  ADC_init();
  
  PORTF &= !(1 << PF0);

  while(1)
  {
    uint16_t adcValue = ADC_read(0);

    if(adcValue > 605)
    {
      PORTC |= (1 << PC0);
    }
    else
    {
      PORTC &= !(1 << PC0);
    }
  }
}