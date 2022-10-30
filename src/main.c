#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "encoder.h"
#include "logger.h"
#include "tests.h"
#include "usart.h"

void spinlock(void) {
  while (1) {
    _NOP();
  }
}

int main(void) {
  logger_set_level(LOG_TRACE);
  usart_init();
  encoder_init();
  sei();
  // test_run("EEPROM", test_eeprom);
  // test_run("PWM", test_pwm);
  for (;;) {
    info("Encoder Event: %u", encoder_next_event());
  }
  info("Task complete, entering spinlock.");
  spinlock();
  return 0;
}
