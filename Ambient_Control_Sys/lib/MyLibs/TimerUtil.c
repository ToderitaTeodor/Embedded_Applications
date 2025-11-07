#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

volatile uint32_t ms_counter = 0; 

void systemTime_init(void)
{
    // waveform generation mode in CTC
    // TCCR0A |= (1 << WGM01);
    // TCCR0A &= ~(1 << WGM00);
    // TCCR0B &= ~(1 << WGM02);

    TCCR0A = (1 << WGM01) | (TCCR0A & ~(1 << WGM00));

    // prescaler set to 64 => 16Mhz / 64 = 250 000 ticks => 250 ticks/ms

    // TCCR0B |= (1 << CS00);
    // TCCR0B |= (1 << CS01);
    // TCCR0B &= ~(1 << CS02);
    TCCR0B = (TCCR0B & ~(1 << WGM02)) | (1 << CS00) | (1 << CS01);

    OCR0A = 249;

    TIMSK0 |= (1 << OCIE0A);

    sei();
}

ISR(TIMER0_COMPA_vect)
{
    ms_counter++;
}

uint32_t sysTime(void)
{
    uint32_t timer;

    //cli(); // stop all interrupts
    
    timer = ms_counter;

    //sei();

    return timer;
}