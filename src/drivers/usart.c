
#include "usart.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

/**
 * @brief Print a line of characters to USART channel
 *
 * @param line character array to print
 */
void usart_println(const char *line) {
  for (size_t i = 0; i < strlen(line); i++) {
    usart_send_byte(line[i]);
  }
}

void usart_init(void) {
  /* Load upper 8-bits into the high byte of the UBRR register
      Default frame format is 8 data bits, no parity, 1 stop bit
      to change use UCSRC, see AVR datasheet */
  cli();
  // Enable receiver and transmitter and receive complete interrupt
  UCSR0B = (1 << TXEN1) | (1 << RXEN1) | (0 << UCSZ12) |
           (1 << RXCIE1); //|(1<<TXCIE1);
  // 8-bit data, 1 stop bit, Aynchronous USART, no parity
  UCSR0C = (1 << UCSZ11) | (1 << UCSZ10) | (0 << USBS1) | (0 << UMSEL11) |
           (0 << UMSEL10) | (0 << UPM11) | (0 << UPM10);
  UBRR0H = (uint8_t)(BAUD_PRESCALE >> 8);
  UBRR0L = (uint8_t)(BAUD_PRESCALE); // Load lower 8-bits into the low byte of
                                     // the UBRR register
  sei();
}

void usart_send_byte(unsigned char value) {
  // wait until buffer is ready to receive new data
  while ((UCSR0A & (1 << UDRE0)) == 0)
    ;
  // write to USART data register TXD
  UDR0 = value;
}

unsigned char usart_poll_byte() {
  while ((UCSR0A & (1 << RXC0)) == 0)
    ;
  // read from USART data register RXD
  return UDR0;
}