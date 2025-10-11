#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>


void PWM_init(void)
{     
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS11); 
}

void setMotorSpeed(uint8_t speed)
{
    OCR1A = speed;
}