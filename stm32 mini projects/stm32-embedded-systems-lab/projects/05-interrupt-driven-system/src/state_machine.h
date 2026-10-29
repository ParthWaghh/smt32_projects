/*
 * state_machine.h
 * Four-state machine for Project 05, driven by events from interrupt.h.
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include "interrupt.h"

typedef enum {
    STATE_IDLE = 0,
    STATE_RUNNING,
    STATE_ALERT,
    STATE_EMERGENCY_STOP
} fsm_state_t;

typedef struct {
    fsm_state_t current_state;
    uint32_t    state_entry_tick;   /* HAL_GetTick() when the state was entered */
    uint32_t    cycle_counter;      /* number of IDLE to RUNNING transitions */
} fsm_t;

void        fsm_init(fsm_t *fsm);
void        fsm_dispatch(fsm_t *fsm, system_event_t event);
fsm_state_t fsm_get_current_state(const fsm_t *fsm);

/* Returns a constant string such as "IDLE" or "EMERGENCY_STOP". */
const char *fsm_state_to_str(fsm_state_t state);

#endif