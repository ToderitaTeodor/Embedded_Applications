#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void ADC_init(void);
uint16_t ADC_read(uint8_t);
float ADC_to_voltage(uint16_t adcValue);

#endif