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

float LM32_ReadTempC(void)
{
    uint16_t adcValue = ADC_read(lm35_channel);
    float voltage = ADC_to_voltage(adcValue);

    return voltage * 100.0; // temperature
}