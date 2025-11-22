#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>

#define LM35_CHANNEL 1
#define LDR_CHANNEL 2

#define LM35_READING_INTERVAL  1500
#define LDR_READING_INTERVAL  500

#define LDR_NIGHT_THRESHOLD 200 
#define LDR_DAY_THRESHOLD 250 

#define BACKLIGHT_TIMEOUT_MS 5000
#define IDLE_TIMEOUT_MS 10000

#define DEBOUNCE_DELAY_MS 200UL

typedef enum {
    MODE_NORMAL,
    MODE_TEST
} SystemMode;

typedef enum {
    MONITOR_NONE,
    MONITOR_TEMP,
    MONITOR_LDR
} MonitoringState;

typedef enum {
    TEST_NONE,
    TEST_LCD,
    TEST_LED_SEQUENCE,
    TEST_MOTOR_RUN
} TestState;

extern MonitoringState currentMonitor;
extern TestState currentTest;
extern SystemMode currentMode;

extern volatile uint32_t lastButtonPressTime;

extern const uint8_t totalMenus;
extern volatile uint8_t menu;
extern volatile uint8_t selected;

extern uint32_t lasTimeLM35;
extern uint32_t lasTimeLDR;

extern volatile uint8_t is_idle;
extern volatile uint8_t is_night_mode;
extern volatile uint8_t last_display_state;

extern uint8_t fanStart;

extern volatile float temperature;
extern volatile uint8_t temperatureSetValue;
extern volatile uint8_t temperature_offset;

extern volatile uint16_t ldrValue;
extern volatile uint16_t ldrSetValue;

extern volatile uint8_t debug_interface_active;

#endif