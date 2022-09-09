#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "usart.h"
#include "eeprom.h"

void eeprom_test(void) {
  uint8_t buffer = 0;
  char buf = 0;
  eeprom_status_t result = EEPROM_SUCCESS;
  for (int i = 0; i <= 9; i++) {
    result = eeprom_write_byte(i, (uint8_t)i);
    if (result == EEPROM_FAILURE) {
      usart_blocking_send('W');
      continue;
    }
    _delay_ms(200);
    result = eeprom_read_byte(i, &buffer);
    if (result == EEPROM_FAILURE) {
      usart_blocking_send('R');
      continue;
    }
    usart_blocking_send(*itoa(buffer, &buf, 10));
    usart_blocking_send('\r');
    usart_blocking_send('\n');
    _delay_ms(200);
  }
}

int main(void){
  usart_init();
  // eeprom_test();
  // SCK of PORTB for outpu
  DDRB |= (1 << DDB1);
  for(;;){
    eeprom_test();
    PORTB = PORTB ^ (1 << PB1);
  }
  return 0;
}
