#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "ADC.h"

#define LED0_PORT PORTC
#define LED0_PIN PC0

#define LED1_PORT PORTC
#define LED1_PIN PC2

void init_leds(void)
{
	DDRC |= (1 << PC0) | (1 << PC2);
}

void led_on(volatile uint8_t *port, uint8_t pin)
{
	*port |= (1 << pin);
}

void led_off(volatile uint8_t *port, uint8_t pin)
{
  *port &= ~(1 << pin);
}

int main(void)
{
  init_leds();
  ADC_init();
  
  while(1)
  {
    uint16_t adcValue = ADC_read(0);

    if(adcValue < 340) 
    {    
        led_on(&LED0_PORT, LED0_PIN);
        led_off(&LED1_PORT, LED1_PIN);       
    } 
    else 
    {               
        led_on(&LED1_PORT, LED1_PIN);
        led_off(&LED0_PORT, LED0_PIN);
    }
  }
}