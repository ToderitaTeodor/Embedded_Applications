#ifndef GLOBAL_H
#define GLOPBAL_H

#include <stdint.h>

#define LM35_CHANNEL 0
#define LDR_CHANNEL 1
#define LM35_READING_INTERVAL  1500
#define LDR_READING_INTERVAL  500

extern const uint8_t totalMenus;
extern volatile uint8_t menu;
extern volatile uint8_t selected ;

extern uint32_t lasTimeLM35;
extern uint32_t lasTimeLDR;

extern uint8_t fanStart;

extern volatile float temperature;
extern volatile uint8_t temperatureSetValue;

extern volatile uint16_t ldrValue;
extern volatile uint16_t ldrSetValue;

#endif