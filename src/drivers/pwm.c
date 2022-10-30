
#include "pwm.h"

#include <avr/cpufunc.h>
#include <avr/io.h>

/**
 * @brief Configure non-inverting PWM waveform generation for high resolution
 * pwm control on PL3 (OC5A) 16.7.4 Phase Correct PWM Mode.
 */
pwm_status_t pwm_init(void) {
  // non-inverting pwm mode
  TCCR5A |= (1 << COM5A1) | (0 << COM5A0);
  // select fclk/8 prescaler
  TCCR5B |= (0 << CS52) | (1 << CS51) | (0 << CS50);
  // configure waveform generation mode for 8-bit phase correct PWM (TOP=0xff)
  TCCR5A |= (0 << WGM51) | (1 << WGM50);
  TCCR5B |= (0 << WGM53) | (0 << WGM52);
  // set OC5A DDR to output for waveform stimulus
  DDRL |= (1 << DDL3);
  // set OCRA to PWM default duty cycle
  OCR5AH = 0x00;
  OCR5AL = PWM_DEFAULT_DUTY_CYCLE;
  // for dd synchronization
  _NOP();
  return PWM_SUCCESS;
}

void pwm_wake(void) {
  // wake timer 5
  PRR1 &= ~(1 << PRTIM5);
}

void pwm_sleep(void) {
  // shutdown timer 5
  PRR1 |= (1 << PRTIM5);
}

pwm_status_t pwm_set_duty(uint8_t duty) {
  OCR5AL = duty;
  return PWM_SUCCESS;
}