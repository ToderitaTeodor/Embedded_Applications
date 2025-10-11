#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "PWM.h"


uint8_t brightness = 0;

int main(void)
{

  PWM_init();
  DDRB |= (1 << PB5);

  while (1)
  {
      LED_brightness(200);
  }

  return 0;
}