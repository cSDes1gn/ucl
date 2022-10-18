#include <avr/io.h>
#include <avr/interrupt.h>

#include "cbuf.h"
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

static volatile uint8_t pre_state = 0;
static volatile enum encoder_event events[ENCODER_EVENT_BUF_LEN];
static cbuf_t cbuffer;

static enum encoder_event encoder_map[] = { 
  ENCODER_NULL,
  ENCODER_CCW,
  ENCODER_CW,
  ENCODER_NULL,
  ENCODER_CW,
  ENCODER_NULL,
  ENCODER_NULL,
  ENCODER_CCW,
  ENCODER_CCW,
  ENCODER_NULL,
  ENCODER_NULL,
  ENCODER_CW,
  ENCODER_NULL,
  ENCODER_CW,
  ENCODER_CCW,
  ENCODER_NULL
};

/**
 * @brief 
 * 
 */
ISR(PCINT2_vect) {
  cli();
  if (!(PINK & (1 << PK3))) {
    trace("PUSH");
    cbuf_put(&cbuffer, ENCODER_SW);
  }
  uint8_t re_state = PINK & ((1 << PK4) | (1 << PK5));
  // translate for concat
  re_state >>= 4;
  uint8_t idx = ((pre_state << 2) | re_state) & 0x0f;
  enum encoder_event state = encoder_map[idx];
  switch (state) {
    case ENCODER_CW:
      trace("CW");
      cbuf_put(&cbuffer, ENCODER_CW);
      break;
    case ENCODER_CCW:
      trace("CCW");
      cbuf_put(&cbuffer, ENCODER_CCW);
      break;
    case ENCODER_NULL:
      break;
    default:
      break;
  }
  // save previous state
  pre_state = re_state;
  sei();
}

void encoder_init(void) {
  // set rotary encoder inputs
  DDRK &= ~((1 << PK5) | (1 << PK4) | (1 << PK3));
  // enable pullups
  PORTK |= (1 << PK5) | (1 << PK4) | (1 << PK3);
  // setup interrupt mask for PCINT2 vector
  PCMSK2 |= (1 << PCINT21) | (1 << PCINT20) | (1 << PCINT19);
  // wake scan on PCINT23:16
  PCICR |= (1 << PCIE2);
  // setup event circular buffer
  cbuffer = cbuf_init((uint8_t *)events, ENCODER_EVENT_BUF_LEN);
  trace("encoder driver init success.");
}

void encoder_clear_events(void) {
  cli();
  cbuf_reset(&cbuffer);
  sei();
  trace("reset encoder event queue");
}

enum encoder_event encoder_next_event(void) {
  enum encoder_event event;
  cli();
  if (cbuf_get(&cbuffer, (uint8_t *)&event) == CBUF_EMPTY) {
    event = ENCODER_NULL;
  }
  sei();
  return event;
}

