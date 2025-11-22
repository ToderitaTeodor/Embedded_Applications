#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include "ADC.h"
#include "UART.h"

static uint8_t lm35_channel = 0;

void LM35_init(uint8_t channel)
{
    lm35_channel = channel;
    ADC_init();
}

float LM35_ReadTempC(void)
{
    float sum = 0.0;

    for(int i = 0; i < 10; i++)
    {
        ADC_read(lm35_channel);
        float adcValue = ADC_read(lm35_channel);

        sum += ADC_to_voltage(adcValue);
        _delay_us(10);
    }

    return (sum / 10.0) * 100; // temperature
}