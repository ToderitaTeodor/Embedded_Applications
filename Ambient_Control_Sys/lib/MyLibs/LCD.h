#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define LCD_ADDR 0x27 // or 0x3F

void I2C_init(void);
void I2C_start(uint8_t address);
void I2C_write(uint8_t data);
void I2C_stop(void);

void LCD_init(void);
void LCD_sendCommand(uint8_t cmd);
void LCD_sendData(uint8_t data);
void LCD_clear(void);
void LCD_gotoxy(uint8_t x, uint8_t y);
void LCD_print(char *str);

#endif
