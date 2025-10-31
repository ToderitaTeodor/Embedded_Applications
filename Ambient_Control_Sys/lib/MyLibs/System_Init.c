#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "global.h"

void init_peripherals(void)
{
    ISR_init();
    systemTime_init();
    PWM_init();
    ADC_init();
    UART_init(9600);
    I2C_init();
    LCD_init();
    LM35_init(LM35_CHANNEL);
}