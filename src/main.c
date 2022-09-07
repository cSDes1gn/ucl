#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usart.h"

int main(void){
  usart_init();
  // SCK of PORTB for outpu
  DDRB |= (1 << DDB1);
  for(;;){
    PORTB = PORTB ^ (1 << PB1);
    _delay_ms(100);
  }
  return 0;
}
