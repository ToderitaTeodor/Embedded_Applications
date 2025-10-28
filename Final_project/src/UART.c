#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>

volatile char uart_buffer[32];
volatile uint8_t uart_index = 0;
volatile uint8_t data_ready = 0;

volatile uint8_t transmit_enabled = 0; // default OFF

void UART_init(uint32_t baud_rate)
{
    uint16_t ubrr_value = (F_CPU / (16 * baud_rate)) - 1;
    
    UBRR0H = (uint8_t)(ubrr_value >> 8);
    UBRR0L = (uint8_t)(ubrr_value);

    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void printByte(uint8_t data)
{
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void printString(const char *str)
{
    while(*str)
    {
        printByte(*str++);
    }
}

void printFloat(float value, uint8_t decimals)
{
    char buffer[12];
    int intPart = (int)value;
    ultoa(intPart, buffer, 10);
    printString(buffer);

    printByte('.');

    float fracPart = value - intPart;
    for(uint8_t i = 0; i < decimals; i++)
    {
        fracPart *= 10;
    }

    int fracInt = (int)(fracPart + 0.5f);
    ultoa(fracInt, buffer, 10);
    printString(buffer);
}

void printInt(uint32_t value)
{
  char buffer[12];

  ultoa(value, buffer, 10);

  printString(buffer);
}

ISR(USART0_RX_vect)
{
    char c = UDR0;

    if(c == '\r' || c == '\n')
    {
        uart_buffer[uart_index] = '\0'; 
        uart_index = 0;
        data_ready = 1;
    }
    else
    {
        if(uart_index < sizeof(uart_buffer) - 1)
            uart_buffer[uart_index++] = c;
    }
}