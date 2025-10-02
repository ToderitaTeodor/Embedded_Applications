#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void UART_init(uint32_t baud_rate)
{
  uint16_t ubrr_value = (F_CPU / (16 * baud_rate)) - 1;
  
  UBRR0H = (uint8_t)(ubrr_value >> 8);
  UBRR0L = (uint8_t)(ubrr_value);

  UCSR0B = (1 << TXEN0) | (1 << RXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 data bits
}

void UART_sendByte(uint8_t data)
{
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}

void UART_sendString(const char *str)
{
  while(*str)
  {
    UART_sendByte(*str++);
  }
}

uint8_t UART_receiveByte(void)
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}