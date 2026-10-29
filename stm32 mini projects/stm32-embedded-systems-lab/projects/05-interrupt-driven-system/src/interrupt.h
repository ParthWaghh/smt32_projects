/*
 * interrupt.h
 * Button interrupts and the event queue for Project 05.
 */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

#define BTN_TRIGGER_PIN     GPIO_PIN_0
#define BTN_TRIGGER_PORT    GPIOB
#define BTN_EMERGENCY_PIN   GPIO_PIN_1
#define BTN_EMERGENCY_PORT  GPIOB

typedef enum {
    EVENT_NONE = 0,
    EVENT_BUTTON_PRESSED,
    EVENT_EMERGENCY_TRIGGERED,
    EVENT_TIMER_TICK,
    EVENT_RESET
} system_event_t;

void            interrupt_gpio_init(void);
void            interrupt_timer_init(void);
bool            interrupt_has_pending_event(void);

/* Returns EVENT_NONE if the queue is empty. */
system_event_t  interrupt_get_next_event(void);

/* Safe to call from an ISR. Drops the event if the queue is full. */
void            interrupt_post_event(system_event_t evt);

#endif