//Copyright (c) 2017 Pratik M Tambe <enthusiasticgeek@gmail.com>
// Tested with WINGONEER USB 2.0 to TTL UART 6PIN CP2102 Module Serial Converter
// Connect RX1 pin of the AVR to TX pin of the serial converter 
// Connect TX1 pin of the AVR to RX pin of the serial converter 
// Connect GND pin of the AVR to GND pin of the serial converter 
// Open terminal. if /dev/ttyUSB0 is connected as serial device
// 'sudo minicom -s configuration ttyUSB0'
// set baudrate 115200 8N1 (no parity, 8 bit data, 1 stop bit). Then press CTRL+a. Then press x.
// 'sudo minicom configuration ttyUSB0' and press ENTER 

/*Very Important - change F_CPU to match target clock 
  Note: default AVR clock is 16MHz external crystal
  This program transmits continously on USART. Interrupt is used for 
  Receive charactor, which is then transmitted instead. LEDs are used 
  as a test. Normal RX routine is included but not used.
  Change USART_BAUDRATE constant to change Baud Rate
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"

int main(void){
  usart_init();
  /* set pin 7 of PORTB for output*/
  DDRB |= _BV(DDB7);
  for(;;){
    PORTB = PORTB ^ _BV(PORTB7);
    _delay_ms(500);
  }
  return 0;
}