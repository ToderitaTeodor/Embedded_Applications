#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

void GPIO_init(volatile uint8_t *ddr, uint8_t mask, uint8_t dir)
{
    if (dir == 1)
        *ddr |= mask;
    else
        *ddr &= ~mask;
}