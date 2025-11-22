#include "Storage.h"
#include "global.h"
#include <avr/eeprom.h>

// Non - volatile
uint8_t  EEMEM nv_magic_byte;      
uint32_t EEMEM nv_motor_starts;    
int16_t  EEMEM nv_max_temp;        
int16_t  EEMEM nv_min_temp;   
int16_t  EEMEM nv_temp_offset;     
uint8_t  EEMEM nv_day;            
uint8_t  EEMEM nv_month;
uint16_t EEMEM nv_year;

uint32_t g_motor_starts = 0;
int16_t g_max_temp = -1000; 
int16_t g_min_temp = 1000;
SystemDate g_install_date = {0, 0, 0};

void Storage_Init(void)
{
    uint8_t magic = eeprom_read_byte(&nv_magic_byte);

    if (magic != 0xA6) 
    {       
        g_motor_starts = 0;
        g_max_temp = -1000; 
        g_min_temp = 1000;  
        temperature_offset = 0;
        
        eeprom_update_byte(&nv_magic_byte, 0xA6); 
        eeprom_update_dword(&nv_motor_starts, g_motor_starts);
        eeprom_update_word((uint16_t*)&nv_max_temp, g_max_temp);
        eeprom_update_word((uint16_t*)&nv_min_temp, g_min_temp);
        eeprom_update_word((uint16_t*)&nv_temp_offset, 0);

        eeprom_update_byte(&nv_day, 0);
        eeprom_update_byte(&nv_month, 0);
        eeprom_update_word(&nv_year, 0);
    }
    else
    {
        
        g_motor_starts = eeprom_read_dword(&nv_motor_starts);
        g_max_temp = (int16_t)eeprom_read_word((uint16_t*)&nv_max_temp);
        g_min_temp = (int16_t)eeprom_read_word((uint16_t*)&nv_min_temp);

        temperature_offset = (int16_t)eeprom_read_word((uint16_t*)&nv_temp_offset);
        
        g_install_date.day = eeprom_read_byte(&nv_day);
        g_install_date.month = eeprom_read_byte(&nv_month);
        g_install_date.year = eeprom_read_word(&nv_year);
    }
}

void Storage_IncrementMotorCount(void)
{
    g_motor_starts++;
    
    eeprom_update_dword(&nv_motor_starts, g_motor_starts);
}

void Storage_UpdateMinMax(float current_temp)
{
    int16_t current_temp_to_int = (int16_t)(current_temp * 10);

    uint8_t changed = 0;

    if (current_temp_to_int > g_max_temp)
    {
        g_max_temp = current_temp_to_int;
        eeprom_update_word((uint16_t*)&nv_max_temp, g_max_temp);
    }

    if (current_temp_to_int < g_min_temp || g_min_temp == 1000)
    {
        g_min_temp = current_temp_to_int;
        eeprom_update_word((uint16_t*)&nv_min_temp, g_min_temp);
    }
}

void Storage_SetDate(uint8_t day, uint8_t month, uint16_t year)
{
    g_install_date.day = day;
    g_install_date.month = month;
    g_install_date.year = year;

    eeprom_update_byte(&nv_day, day);
    eeprom_update_byte(&nv_month, month);
    eeprom_update_word(&nv_year, year);
}

void Storage_SaveOffset(int16_t new_offset)
{
    eeprom_update_word((uint16_t*)&nv_temp_offset, new_offset);
}