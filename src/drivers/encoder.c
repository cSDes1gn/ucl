#include <avr/io.h>
#include <avr/interrupt.h>


#define RE_PK_MASK ((1 << PK5) | (1 << PK4) | (1 << PK3))
// rotary encoder debounce upper threshold (ms)
#define RE_DEBOUNCE_THRESH 10

// All Rotary encoder signals will be handled by PCINT2
// SW -> PK3 (PCINT19)
// CLK -> PK4 (PCINT20)
// DT -> PK5 (PCINT21)

static volatile uint8_t re_state = 0x0;

/**
 * @brief Read from rotary encoder every 30 ms (~33 Hz)
 * Phase leads by ~ 60ms for fast turns by Nyquists theory we should sample at minimum twice the frequency
 * 
 */
ISR(TIMER0_OVF_vect) {
  cli();
  // uint8_t curr_re_state = (PINK & 0x30);
  // sw read
  if (PINK & (1 << PK3)) {
    info("release");
  } else {
    info("press");
  }
  sei();
}

// ISR(PCINT2_vect) {
//   cli();
//   // perform debounce verification
//   // clk dt read
//   uint8_t curr_re_state = (PINK & 0x30);
//   if (db_timer) {
//     if (db_timer > TCNT0) {
//       if ((db_timer - TCNT0) < RE_DEBOUNCE_THRESH) {
//         goto ret;
//       }
//     } else {
//       if ((TCNT0 - db_timer) < RE_DEBOUNCE_THRESH) {
//         goto ret;
//       }
//     }
//   }

//   if (((re_state == 0x0) && (curr_re_state == 0x10)) || ((re_state == 0x30) && (curr_re_state== 0x20))) {
//     info("CW");
//   } else if (((re_state == 0x0) && (curr_re_state == 0x20)) || ((re_state == 0x30) && (curr_re_state== 0x10))) {
//     info("CCW");
//   }

//   // sw read
//   // if (PINK & (1 << PK3)) {
//   //   info("release");
//   // } else {
//   //   info("press");
//   // }
// ret:
//   if ( curr_re_state == 0x00) {
//     re_state = 0x00;
//   } else if (curr_re_state == 0x30) {
//     re_state = 0x30;
//   }
//   db_timer = TCNT0;
//   sei();
// }

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
