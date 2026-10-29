/*
 * pwm_control.c
 * PWM driver: TIM3 channel 1 on PA6, 1 kHz.
 *
 * Timer clock is 72 MHz (APB1 runs at 36 MHz, and the timers get double
 * that when the APB1 prescaler is not 1). Prescaler 71 gives a 1 MHz
 * counter, and ARR 999 gives 1000 ticks per period, so 1 kHz.
 */

#include "pwm_control.h"
#include "main.h"

#define PWM_PRESCALER   (72U - 1U)
#define PWM_PERIOD      (1000U - 1U)

static TIM_HandleTypeDef htim3;


void pwm_init(void)
{
    GPIO_InitTypeDef  gpio_cfg = {0};
    TIM_OC_InitTypeDef oc_cfg  = {0};

    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_cfg.Pin   = GPIO_PIN_6;
    gpio_cfg.Mode  = GPIO_MODE_AF_PP;
    gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_cfg);

    htim3.Instance               = TIM3;
    htim3.Init.Prescaler         = PWM_PRESCALER;
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = PWM_PERIOD;
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
        while (1) { }
    }

    oc_cfg.OCMode     = TIM_OCMODE_PWM1;
    oc_cfg.Pulse      = 0;      /* start at 0% duty */
    oc_cfg.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc_cfg.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim3, &oc_cfg, TIM_CHANNEL_1) != HAL_OK)
    {
        while (1) { }
    }
}


void pwm_start(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}


void pwm_stop(void)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}


/* duty is the compare value, 0 to 999 (0 = always low, 999 = about 99.9%). */
void pwm_set_duty(uint16_t duty)
{
    if (duty > PWM_PERIOD)
    {
        duty = PWM_PERIOD;
    }

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
}