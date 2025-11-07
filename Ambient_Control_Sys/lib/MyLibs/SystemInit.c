#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "global.h"
#include "LCD.h"
#include "LM35.h"
#include "TimerUtil.h"
#include "ADC.h"
#include "UART.h"

void init_peripherals(void)
{
    UART_init(9600);
    ADC_init();
    systemTime_init();
    LM35_init(0);
    I2C_init();
    LCD_init();
    _delay_ms(100);
}