#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

void PWM_init(void)
{
    // Non-inverting mode
    // WGM - Fast PWM mode
    // Prescaler - 8bit

    TCCR1A = (1 << COM1A1) | (1 << WGM10);

    TCCR1B = (1 << WGM12) | (1 << CS11);

}

void Fan_speed(uint8_t brightness)
{
    OCR1A = brightness;
}
