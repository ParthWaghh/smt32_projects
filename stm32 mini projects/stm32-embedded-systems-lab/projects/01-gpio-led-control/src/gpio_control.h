/*
 * gpio_control.h
 * Public API for the GPIO LED and button driver.
 */

#ifndef GPIO_CONTROL_H
#define GPIO_CONTROL_H

#include <stdint.h>

void gpio_led_init(void);
void gpio_button_init(void);

/* Returns 1 while the button is held down, 0 otherwise. */
uint8_t gpio_button_is_pressed(void);

void gpio_led_all_off(void);

/* One red -> yellow -> green cycle, delay_ms per LED. */
void gpio_led_pattern_sequential(uint32_t delay_ms);

/* One on/off cycle of all LEDs together, delay_ms each way. */
void gpio_led_pattern_all_blink(uint32_t delay_ms);

#endif