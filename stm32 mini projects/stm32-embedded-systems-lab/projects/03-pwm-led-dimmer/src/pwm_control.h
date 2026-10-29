/*
 * pwm_control.h
 * PWM driver public API (TIM3 CH1 on PA6).
 */

#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H

#include <stdint.h>

void pwm_init(void);
void pwm_start(void);
void pwm_stop(void);

/* Compare value, 0 to 999 (999 is about 99.9% duty). Larger values are clamped. */
void pwm_set_duty(uint16_t duty);

#endif