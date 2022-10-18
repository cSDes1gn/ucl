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

static volatile uint8_t pre_state = 0;
// static volatile enum encoder_event *event_buf[ENCODER_EVENT_BUF_LEN] = {};

static enum encoder_event encoder_map[] = { 
  ENCODER_INVALID,
  ENCODER_CCW,
  ENCODER_CW,
  ENCODER_INVALID,
  ENCODER_CW,
  ENCODER_INVALID,
  ENCODER_INVALID,
  ENCODER_CCW,
  ENCODER_CCW,
  ENCODER_INVALID,
  ENCODER_INVALID,
  ENCODER_CW,
  ENCODER_INVALID,
  ENCODER_CW,
  ENCODER_CCW,
  ENCODER_INVALID
};

/**
 * @brief 
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
  uint8_t idx = ((pre_state << 2) | re_state) & 0x0f;
  enum encoder_event state = encoder_map[idx];
  switch (state) {
    case ENCODER_CW:
      info("CW");
      break;
    case ENCODER_CCW:
      info("CCW");
      break;
    case ENCODER_INVALID:
      break;
    default:
      info("INVALID");
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
}
