#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "global.h"

MonitoringState currentMonitor;
TestState currentTest;
SystemMode currentMode;

volatile uint32_t lastButtonPressTime = (uint32_t)(-BACKLIGHT_TIMEOUT_MS - 1);

volatile uint8_t is_idle = 0;
volatile uint8_t is_night_mode = 0;

volatile uint8_t last_display_state = 0;

const uint8_t totalMenus = 2;
volatile uint8_t menu = 0;
volatile uint8_t selected = 0;

uint32_t lasTimeLM35 = 0;
uint32_t lasTimeLDR = 0;

uint8_t fanStart = 0;

volatile float temperature = 0;
volatile uint8_t temperatureSetValue = 35;

volatile uint8_t temperature_offset = 0;

volatile uint16_t ldrValue = 0;
volatile uint16_t ldrSetValue = 600;

volatile uint8_t debug_interface_active = 0;
