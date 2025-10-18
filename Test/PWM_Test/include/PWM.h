#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void PWM_init(void);
void Fan_speed(uint8_t brightness);

#endif