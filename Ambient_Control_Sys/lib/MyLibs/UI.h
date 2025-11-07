#ifndef UI_H
#define UI_H

#include <stdint.h>

void displayMenu(uint8_t menuIndex);
void displaySubmenu(uint8_t menuIndex);
void updateMenuDisplay(void);
void setLCDDisplayMode(uint8_t mode);

#endif