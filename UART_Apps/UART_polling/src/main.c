#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <UART.h>

int main(void)
{
  UART_init(9600);

  UART_sendString("Embedded Systems Course\r\n");

  while (1)
  {
    uint8_t c = UART_receiveByte(); 
    UART_sendByte(c);               
  }
}