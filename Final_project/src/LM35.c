#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "ADC.h"

static uint8_t lm35_channel = 0;

void LM35_init(uint8_t channel)
{
    lm35_channel = channel;
    ADC_init();
}

float LM35_ReadTempC(void)
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < 10; i++)
        sum += ADC_read(lm35_channel);
    
    float avg = sum / 10.0;
    float voltage = ADC_to_voltage(avg);

    return voltage * 100.0; // temperature
}