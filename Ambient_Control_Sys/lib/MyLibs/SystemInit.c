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
#include "PWM.h"

void init_peripherals(void)
{
    UART_init(9600);
    PWM_init();
    ADC_init();
    systemTime_init();
    LM35_init(LM35_CHANNEL);
    I2C_init();
    LCD_init();
}