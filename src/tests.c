
#include <avr/io.h>
#include <util/delay.h>
#include "eeprom.h"
#include "pwm.h"
#include "logger.h"
#include "tests.h"


void test_run(char *name, test_func test) {
  DDRB |= (1 << DDB1);
  warning("======= %s TEST =========", name);
  PORTB |= (1 << PB1);
  test_status_t result = test();
  if (result == TEST_FAILURE) {
    error("[ FAILED ] Test %s failed with error: %i", name, result);
  } else {
    info("[ PASSED ]");
  }
  PORTB &= ~(1 << PB1);
}

test_status_t test_eeprom(void) {
  uint8_t buffer = 0;
  eeprom_status_t result = EEPROM_SUCCESS;
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
  return TEST_SUCCESS;
}

test_status_t test_pwm(void) {
  trace("Initializing PWM peripheral");
  pwm_init();
  trace("waking pwm timer module");
  pwm_wake();
  for (uint16_t i = 0; i <= 255; i++) {
    _delay_ms(10);
    pwm_set_duty((uint8_t)i);
  }
  trace("powering down pwm timer module");
  pwm_sleep();
  return TEST_SUCCESS;
}