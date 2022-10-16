#include <avr/io.h>
#include <avr/interrupt.h>

#include "encoder.h"
#include "logger.h"




/**
 * @brief All Rotary encoder signals will be handled by PCINT2
 *  SW -> PK3 (PCINT19)
 *  DT -> PK4 (PCINT20)
 *  CLK -> PK5 (PCINT21)
 */

/**
 * @brief Implement table encoding method.　Below are the table state encodings:
 * 
 * INVALID = 0
 * CW = 1
 * CCW = 2
 * 
 * [PCLK PDT CLK DT | STATE | CODE ]
 *  0 0 0 0 | PASSIVE | 0
 *  0 0 0 1 | CCW     | 2
 *  0 0 1 0 | CW      | 1
 *  0 0 1 1 | INVALID | 0 
 *  0 1 0 0 | CW      | 1
 *  0 1 0 1 | PASSIVE | 0
 *  0 1 1 0 | INVALID | 0
 *  0 1 1 1 | CCW     | 2
 *  1 0 0 0 | CCW     | 2
 *  1 0 0 1 | INVALID | 0
 *  1 0 1 0 | PASSIVE | 0
 *  1 0 1 1 | CW      | 1
 *  1 1 0 0 | INVALID | 0
 *  1 1 0 1 | CW      | 1
 *  1 1 1 0 | CCW     | 2
 *  1 1 1 1 | PASSIVE | 0
 */
static uint8_t encoder_map[] = { 0, 2, 1, 0, 1, 0, 0, 2, 2, 0, 0, 1, 0, 1, 2, 0 };

static volatile uint8_t pre_state = 0x0;
// static volatile enum encoder_event *event_buf[ENCODER_EVENT_BUF_LEN] = {};

/**
 * @brief Hardware debouncing still required!
 * 
 */
ISR(PCINT2_vect) {
  cli();
  if (PINK & (1 << PK3)) {
    info("release");
  } else {
    info("press");
  }
  uint8_t re_state = PINK & ((1 << PK4) | (1 << PK5));
  // translate for concat
  re_state >>= 4;
  // filter for stable state
  info("=============================");
  info("re_state: %x", re_state);
  info("pre_state: %x", pre_state);
  // concat for idx
  uint8_t idx = ((pre_state << 2) | re_state) & 0x0f;
  info("idx: %x", idx);
  uint8_t state = encoder_map[idx];
  info("state: %u", state);
  switch (state) {
    case 1:
      info("CW");
      // save previous state
      pre_state = re_state;
      break;
    case 2:
      info("CCW");
      // save previous state
      pre_state = re_state;
      break;
    default:
      info("INVALID");
      break;
  }
  sei();
}

void encoder_init(void) {
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
  // wake scan on PCINT23:16
  PCICR |= (1 << PCIE2);
}

void encoder_wake(void) {
  // wake debounce timer
  PRR0 &= ~(1 << PRTIM0);
}

void encoder_sleep(void) {
  // disable timer interrupts
  PRR0 |= (1 << PRTIM0);
  // re-enable wake scan on PCINT23:16
  PCICR |= (1 << PCIE2);
}