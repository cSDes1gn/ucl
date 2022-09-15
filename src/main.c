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

void pwm_sweep(void) {
  info("========= PWM SWEEP ===========");
  trace("waking pwm timer module");
  pwm_wake();
  PORTB |= (1 << PB1);
  for (uint16_t i = 0; i <= 255; i++) {
    _delay_ms(10);
    pwm_set_duty((uint8_t)i);
  }
  PORTB &= ~(1 << PB1);
  trace("powering down pwm timer module");
  pwm_sleep();
  info("========= PWM SWEEP COMPLETE ===========");
}

int main(void){
  logger_set_level(LOG_TRACE);
  usart_init();
  pwm_init();
  DDRB |= (1 << DDB1);
  for(;;){
    pwm_sweep();
    _delay_ms(1000);
  }
  return 0;
}
