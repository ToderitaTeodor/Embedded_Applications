#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#define LM35_CHANNEL 0
#define LDR_CHANNEL 1
#define LM35_READING_INTERVAL  1500
#define LDR_READING_INTERVAL  500

const uint8_t totalMenus = 2;
volatile uint8_t menu = 0;
volatile uint8_t selected = 0;

uint32_t lasTimeLM35 = 0;
uint32_t lasTimeLDR = 0;

uint8_t fanStart = 0;

volatile float temperature = 0;
volatile uint8_t temperatureSetValue = 50;

volatile uint16_t ldrValue = 0;
volatile uint16_t ldrSetValue = 100;
