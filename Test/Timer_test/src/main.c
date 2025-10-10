#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "sysTime.h"

uint32_t lastTime = 0;

int main(void)
{
  sei();

  systemTime_init();

  DDRB |= (1 << PB1);

  while (1)
  {
    uint32_t currentTime = sysTime();

    if(currentTime -lastTime >= 2500)
    {
      PORTB ^= (1 << PB1);

      lastTime = currentTime;
    }
  }

  return 0;
}