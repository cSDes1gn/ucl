#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "usart.h"
#include "logger.h"
#include "eeprom.h"
#include "pwm.h"

#define RE_PK_MASK ((1 << PK5) | (1 << PK4) | (1 << PK3))
// rotary encoder debounce upper threshold (ms)
#define RE_DEBOUNCE_THRESH 75

static volatile uint8_t re_state = 0x0;
static volatile uint8_t db_timer = 0x0;

// All Rotary encoder signals will be handled by PCINT2
// SW -> PK3 (PCINT19)
// CLK -> PK4 (PCINT20)
// DT -> PK5 (PCINT21)

/**
 * @brief Reset pushed timer result to indicate overflow or start of RE scan sequence
 * 
 */
ISR(TIMER0_OVF_vect) {
  cli();
  db_timer = 0x0;
  sei();
}

ISR(PCINT2_vect) {
  cli();
  // perform debounce verification
  // clk dt read
  uint8_t curr_re_state = (PINK & 0x30);
  if (db_timer) {
    if (db_timer > TCNT0) {
      if ((db_timer - TCNT0) < RE_DEBOUNCE_THRESH) {
        goto ret;
      }
    } else {
      if ((TCNT0 - db_timer) < RE_DEBOUNCE_THRESH) {
        goto ret;
      }
    }
  }

  if (((re_state == 0x0) && (curr_re_state == 0x10)) || ((re_state == 0x30) && (curr_re_state== 0x20))) {
    info("CW");
  } else if (((re_state == 0x0) && (curr_re_state == 0x20)) || ((re_state == 0x30) && (curr_re_state== 0x10))) {
    info("CCW");
  }

  // sw read
  // if (PINK & (1 << PK3)) {
  //   info("release");
  // } else {
  //   info("press");
  // }
ret:
  if ( curr_re_state == 0x00) {
    re_state = 0x00;
  } else if (curr_re_state == 0x30) {
    re_state = 0x30;
  }
  db_timer = TCNT0;
  sei();
}

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

void re_init(void) {
  // set rotary encoder inputs
  DDRK &= ~((1 << PK5) | (1 << PK4) | (1 << PK3));
  // enable pullups
  PORTK |= (1 << PK5) | (1 << PK4) | (1 << PK3);
  // setup interrupt mask for PCINT2 vector
  PCMSK2 |= (1 << PCINT21) | (1 << PCINT20) | (1 << PCINT19);
  // configure a debounce timer (normal operation with output compare and wavegen disabled
  TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0) | (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00));
  TCCR0B &= ~((1 << WGM02));
  // configure timer with a 1024 prescaler (15.625KHz @ 16 MHz) and overflow interrupts
  TCCR0B |= ((1 << CS02) | (1 << CS00));
  TIMSK0 |= (1 << TOIE0);
  TCNT0 = 0x0;
  // sleep timer 0
  PRR0 |= (1 << PRTIM0);
}

void re_enable(void) {
  // push initial port k state
  re_state = PINK & 0x30;
  info("Initial RE_STATE: 0x%x", re_state);
  // enable scan on PCINT23:16
  PCICR |= (1 << PCIE2);
  // wake debounce timer
  PRR0 &= ~(1 << PRTIM0);
}

int main(void){
  logger_set_level(LOG_TRACE);
  usart_init();
  pwm_init();
  DDRB |= (1 << DDB1);
  re_init();
  re_enable();
  sei();
  for(;;){
    PORTB = PORTB ^ (1 << PB1);
    // pwm_sweep();
    // eeprom_test();
    _delay_ms(10);
  }
  return 0;
}
