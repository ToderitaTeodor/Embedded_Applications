#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "UART.h"

ISR(USART0_RX_vect)
{
  uint8_t c = UDR0;
  UART_sendByte(c);
}

int main(void)
{
  UART_init(9600);
  UART_sendString("Embedded Systems course\r\n");

  sei();

  while (1)
  {
    
  }
}