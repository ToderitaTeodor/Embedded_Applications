#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>

typedef struct {
    uint8_t day;
    uint8_t month;
    uint16_t year;
} SystemDate;

extern uint32_t g_motor_starts;
extern int16_t g_max_temp;
extern int16_t g_min_temp;
extern SystemDate g_install_date;

void Storage_Init(void);
void Storage_IncrementMotorCount(void);
void Storage_UpdateMinMax(float current_temp);
void Storage_SetDate(uint8_t day, uint8_t month, uint16_t year);
void Storage_SaveOffset(int16_t new_offset);

#endif