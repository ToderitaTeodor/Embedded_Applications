#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

void ADC_init(void)
{
    ADMUX = (1 << REFS0); // reference to 5volts and ADC chanel 0
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS2); // enable and set prescaler to 64
}

uint16_t ADC_read(uint8_t channel)
{
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

    ADCSRA |= (1 << ADSC);

    while(ADCSRA & (1 << ADSC));

    return ADC;
}

float ADC_to_voltage(uint16_t adcValue)
{
    return ((float)adcValue * 5.0) / 1023.0;
}