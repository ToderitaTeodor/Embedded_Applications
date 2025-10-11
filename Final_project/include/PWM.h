#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void PWM_init(void);
void setMotorSpeed(uint8_t speed);

#endif