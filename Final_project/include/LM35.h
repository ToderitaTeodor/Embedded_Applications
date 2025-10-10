#ifndef LM35_H
#define LM35_H

#include <stdint.h>

void LM35_init(uint8_t channel);
float LM35_ReadTempC(void);

#endif