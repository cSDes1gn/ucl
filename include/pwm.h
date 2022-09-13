#ifndef __PWM_H__
#define __PWM_H__


#include <stdint.h>
#include "configuration.h"

typedef int pwm_status_t;

#define PWM_SUCCESS (pwm_status_t)0
#define PWM_FAILURE (pwm_status_t)1

#define PWM_PRESCALER 8
/**
 * @brief Phase Correct PWM frequency calculation described by 17.9.4
 * 
 */
#define PWM_TOP 0xff
#define PWM_FREQ  F_CPU / (PWM_PRESCALER * 2 * PWM_TOP)
#define PWM_DEFAULT_DUTY_CYCLE 127

void pwm_wake(void);
void pwm_sleep(void);
pwm_status_t pwm_init(void);
pwm_status_t pwm_set_duty(uint8_t duty);

#endif // __PWM_H__ 