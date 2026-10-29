/*
 * gpio_control.c
 * GPIO LED and button driver for Project 01.
 *
 * External LEDs: PA0 (red), PA1 (yellow), PA2 (green)
 * On-board LED:  PC13 (active-low)
 * Button:        PB0 (active-low, external pull-up)
 */

#include "gpio_control.h"
#include "main.h"

#define LED_RED_PIN         GPIO_PIN_0
#define LED_RED_PORT        GPIOA

#define LED_YELLOW_PIN      GPIO_PIN_1
#define LED_YELLOW_PORT     GPIOA

#define LED_GREEN_PIN       GPIO_PIN_2
#define LED_GREEN_PORT      GPIOA

#define LED_ONBOARD_PIN     GPIO_PIN_13
#define LED_ONBOARD_PORT    GPIOC

#define BUTTON_PIN          GPIO_PIN_0
#define BUTTON_PORT         GPIOB


void gpio_led_init(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Set the output levels first so the LEDs don't flash on startup */
    HAL_GPIO_WritePin(LED_RED_PORT,     LED_RED_PIN,     GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_YELLOW_PORT,  LED_YELLOW_PIN,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_PORT,   LED_GREEN_PIN,   GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_ONBOARD_PORT, LED_ONBOARD_PIN, GPIO_PIN_SET);

    gpio_cfg.Pin   = LED_RED_PIN | LED_YELLOW_PIN | LED_GREEN_PIN;
    gpio_cfg.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_cfg.Pull  = GPIO_NOPULL;
    gpio_cfg.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio_cfg);

    gpio_cfg.Pin = LED_ONBOARD_PIN;
    HAL_GPIO_Init(LED_ONBOARD_PORT, &gpio_cfg);
}


void gpio_button_init(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_cfg.Pin  = BUTTON_PIN;
    gpio_cfg.Mode = GPIO_MODE_INPUT;
    gpio_cfg.Pull = GPIO_PULLUP;    /* backup for the external pull-up */
    HAL_GPIO_Init(BUTTON_PORT, &gpio_cfg);
}


/* Returns 1 while the button is held down. No debouncing here. */
uint8_t gpio_button_is_pressed(void)
{
    return (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_RESET) ? 1U : 0U;
}


void gpio_led_all_off(void)
{
    HAL_GPIO_WritePin(LED_RED_PORT,     LED_RED_PIN,     GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_YELLOW_PORT,  LED_YELLOW_PIN,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_PORT,   LED_GREEN_PIN,   GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_ONBOARD_PORT, LED_ONBOARD_PIN, GPIO_PIN_SET);
}


/* Red, yellow, green, each on for delay_ms. PC13 follows the red LED. */
void gpio_led_pattern_sequential(uint32_t delay_ms)
{
    gpio_led_all_off();
    HAL_GPIO_WritePin(LED_RED_PORT,     LED_RED_PIN,     GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_ONBOARD_PORT, LED_ONBOARD_PIN, GPIO_PIN_RESET);
    HAL_Delay(delay_ms);

    gpio_led_all_off();
    HAL_GPIO_WritePin(LED_YELLOW_PORT, LED_YELLOW_PIN, GPIO_PIN_SET);
    HAL_Delay(delay_ms);

    gpio_led_all_off();
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
    HAL_Delay(delay_ms);
}


/* One on/off cycle of all LEDs together, delay_ms each way. */
void gpio_led_pattern_all_blink(uint32_t delay_ms)
{
    HAL_GPIO_WritePin(LED_RED_PORT,     LED_RED_PIN,     GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_YELLOW_PORT,  LED_YELLOW_PIN,  GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_GREEN_PORT,   LED_GREEN_PIN,   GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_ONBOARD_PORT, LED_ONBOARD_PIN, GPIO_PIN_RESET);
    HAL_Delay(delay_ms);

    gpio_led_all_off();
    HAL_Delay(delay_ms);
}