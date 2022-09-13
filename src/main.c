#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "usart.h"
#include "logger.h"
#include "eeprom.h"
#include "pwm.h"

void eeprom_test(void) {
  uint8_t buffer = 0;
  eeprom_status_t result = EEPROM_SUCCESS;
  warning("======= EEPROM TEST =========");
  for (int i = 0; i < EEPROM_MAX_ADDR; i++) {
    result = eeprom_write_byte(i, (uint8_t)i);
    if (result == EEPROM_FAILURE) {
      error("eeprom write failed");
    }
    _delay_ms(20);
    result = eeprom_read_byte(i, &buffer);
    if (result == EEPROM_FAILURE) {
      error("eeprom read failed");
    }
    info("EEPROM [%x]: %u", i, buffer);
    _delay_ms(20);
  }
  warning("======= TEST COMPLETE =========");
}

int main(void){
  logger_set_level(LOG_TRACE);
  usart_init();
  pwm_init();
  pwm_wake();
  DDRB |= (1 << DDB1);
  for(;;){
    // when SCK ON do not pwr off
    PORTB = PORTB ^ (1 << PB1);
    _delay_ms(1000);
  }
  return 0;
}
