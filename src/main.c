#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "usart.h"
#include "logger.h"
#include "eeprom.h"

void eeprom_test(void) {
  uint8_t buffer = 0;
  eeprom_status_t result = EEPROM_SUCCESS;
  result = eeprom_write_byte(0, 1);
  if (result == EEPROM_FAILURE) {
    usart_send_byte('W');
  }
  _delay_ms(200);
  result = eeprom_read_byte(0, &buffer);
  if (result == EEPROM_FAILURE) {
    usart_send_byte('R');
  }
  if (buffer != 1) {
    usart_send_byte('1');
  }
  usart_send_byte('\r');
  usart_send_byte('\n');
  _delay_ms(200);
}

int main(void){
  logger_set_level(LOG_TRACE);
  usart_init();
  // eeprom_test();
  // SCK of PORTB for outpu
  DDRB |= (1 << DDB1);
  for(;;){
    info("Hello this is a test: %i, %f", 1, (float)0.0234);
    _delay_ms(500);
    // eeprom_test();
    PORTB = PORTB ^ (1 << PB1);
  }
  return 0;
}
