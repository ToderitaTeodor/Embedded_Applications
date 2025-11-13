#include "LCD.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04

static uint8_t backlight_state = LCD_BACKLIGHT;

static void LCD_send(uint8_t data, uint8_t mode);

void I2C_init(void)
{
    TWSR = 0x00;
    TWBR = 72; // ~100kHz la 16MHz
}

void I2C_start(uint8_t address)
{
    TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
    TWDR = address;
    TWCR = (1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
}

void I2C_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWEN)|(1<<TWINT);
    while(!(TWCR & (1<<TWINT)));
}

void I2C_stop(void)
{
    TWCR = (1<<TWSTO)|(1<<TWINT)|(1<<TWEN);
    _delay_ms(2);
}

static void LCD_pulseEnable(uint8_t data)
{
    I2C_write(data | LCD_ENABLE | backlight_state);
    _delay_us(1);
    I2C_write((data & ~LCD_ENABLE) | backlight_state);
    _delay_us(50);
}

static void LCD_send(uint8_t data, uint8_t mode)
{
    uint8_t highNib = data & 0xF0;
    uint8_t lowNib = (data << 4) & 0xF0;

    I2C_start(LCD_ADDR << 1);
    I2C_write(highNib | mode | backlight_state);
    LCD_pulseEnable(highNib | mode);
    I2C_write(lowNib | mode | backlight_state);
    LCD_pulseEnable(lowNib | mode);
    I2C_stop();
}

void LCD_sendCommand(uint8_t cmd)
{
    LCD_send(cmd, 0);
}

void LCD_sendData(uint8_t data)
{
    LCD_send(data, 1);
}

void LCD_init(void)
{
    _delay_ms(50);
    LCD_sendCommand(0x33);
    LCD_sendCommand(0x32);
    LCD_sendCommand(0x28);
    LCD_sendCommand(0x0C);
    LCD_sendCommand(0x06);
    LCD_clear();
}

void LCD_clear(void)
{
    LCD_sendCommand(0x01);
    _delay_ms(2);
}

void LCD_gotoxy(uint8_t x, uint8_t y)
{
    uint8_t addr[] = {0x80, 0xC0};
    LCD_sendCommand(addr[y] + x);
}

void LCD_print(char *str)
{
    while(*str) LCD_sendData(*str++);
}

void LCD_printInt(int num)
{
    char buf[7];
    itoa(num, buf, 10);
    LCD_print(buf);
}

void LCD_backlight_ON(void)
{
    backlight_state = LCD_BACKLIGHT;

    LCD_sendCommand(0x0C);
}

void LCD_backlight_OFF(void)
{
    backlight_state = 0x00;

    LCD_sendCommand(0x0C);
}