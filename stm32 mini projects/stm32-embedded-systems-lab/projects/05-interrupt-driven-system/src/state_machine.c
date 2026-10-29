/*
 * state_machine.c
 * Four-state machine driven by button events. The LEDs on PA0 to PA2
 * show the current state.
 */

#include "state_machine.h"
#include "stm32f1xx_hal.h"

#define LED_IDLE_PIN        GPIO_PIN_0      /* green */
#define LED_RUNNING_PIN     GPIO_PIN_1      /* yellow */
#define LED_ALERT_PIN       GPIO_PIN_2      /* red */
#define LED_ALL_PINS        (LED_IDLE_PIN | LED_RUNNING_PIN | LED_ALERT_PIN)


static void update_led_outputs(fsm_state_t state)
{
    HAL_GPIO_WritePin(GPIOA, LED_ALL_PINS, GPIO_PIN_RESET);

    switch (state)
    {
        case STATE_IDLE:
            HAL_GPIO_WritePin(GPIOA, LED_IDLE_PIN, GPIO_PIN_SET);
            break;

        case STATE_RUNNING:
            HAL_GPIO_WritePin(GPIOA, LED_RUNNING_PIN, GPIO_PIN_SET);
            break;

        case STATE_ALERT:
            HAL_GPIO_WritePin(GPIOA, LED_ALERT_PIN, GPIO_PIN_SET);
            break;

        case STATE_EMERGENCY_STOP:
            /* yellow and red together, steady (no blinking yet) */
            HAL_GPIO_WritePin(GPIOA, LED_RUNNING_PIN | LED_ALERT_PIN, GPIO_PIN_SET);
            break;

        default:
            break;
    }
}


static void enter_state(fsm_t *fsm, fsm_state_t next)
{
    fsm->current_state    = next;
    fsm->state_entry_tick = HAL_GetTick();
}


void fsm_init(fsm_t *fsm)
{
    fsm->cycle_counter = 0;
    enter_state(fsm, STATE_IDLE);
    update_led_outputs(fsm->current_state);
}


void fsm_dispatch(fsm_t *fsm, system_event_t event)
{
    if (event == EVENT_NONE)
    {
        return;
    }

    /* Emergency stop wins in every state */
    if (event == EVENT_EMERGENCY_TRIGGERED)
    {
        enter_state(fsm, STATE_EMERGENCY_STOP);
        update_led_outputs(fsm->current_state);
        return;
    }

    switch (fsm->current_state)
    {
        case STATE_IDLE:
            if (event == EVENT_BUTTON_PRESSED)
            {
                enter_state(fsm, STATE_RUNNING);
                fsm->cycle_counter++;
            }
            break;

        case STATE_RUNNING:
            if (event == EVENT_BUTTON_PRESSED)
            {
                enter_state(fsm, STATE_ALERT);
            }
            break;

        case STATE_ALERT:
            if (event == EVENT_BUTTON_PRESSED)
            {
                enter_state(fsm, STATE_IDLE);
            }
            break;

        case STATE_EMERGENCY_STOP:
            /* A trigger button press clears the stop */
            if (event == EVENT_BUTTON_PRESSED)
            {
                enter_state(fsm, STATE_IDLE);
            }
            break;

        default:
            enter_state(fsm, STATE_IDLE);
            break;
    }

    update_led_outputs(fsm->current_state);
}


fsm_state_t fsm_get_current_state(const fsm_t *fsm)
{
    return fsm->current_state;
}


const char *fsm_state_to_str(fsm_state_t state)
{
    switch (state)
    {
        case STATE_IDLE:           return "IDLE";
        case STATE_RUNNING:        return "RUNNING";
        case STATE_ALERT:          return "ALERT";
        case STATE_EMERGENCY_STOP: return "EMERGENCY_STOP";
        default:                   return "UNKNOWN";
    }
}