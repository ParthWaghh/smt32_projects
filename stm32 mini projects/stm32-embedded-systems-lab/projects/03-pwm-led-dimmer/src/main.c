/*
 * main.c
 * Project 03: PWM LED Dimmer
 *
 * A potentiometer on PA0 (ADC1_IN0) sets the duty cycle of a 1 kHz
 * PWM signal on PA6 (TIM3_CH1). The pot is read every 20 ms.
 *
 * Target: STM32F103C8T6, 72 MHz from 8 MHz HSE + PLL.
 */

#include "main.h"
#include "pwm_control.h"

#define ADC_MAX_COUNT   4095U
#define PWM_MAX_DUTY    999U        /* timer ARR value */
#define UPDATE_MS       20U

static ADC_HandleTypeDef hadc1;

static void SystemClock_Config(void);
static void adc_init(void);
static uint16_t adc_read(void);
static void Error_Handler(void);


int main(void)
{
    HAL_Init();
    SystemClock_Config();

    adc_init();
    pwm_init();
    pwm_start();

    while (1)
    {
        uint16_t raw  = adc_read();
        uint32_t duty = ((uint32_t)raw * PWM_MAX_DUTY) / ADC_MAX_COUNT;

        pwm_set_duty((uint16_t)duty);
        HAL_Delay(UPDATE_MS);
    }
}


static void adc_init(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};
    ADC_ChannelConfTypeDef ch = {0};

    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_cfg.Pin  = GPIO_PIN_0;
    gpio_cfg.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &gpio_cfg);

    hadc1.Instance                = ADC1;
    hadc1.Init.DataAlign          = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode       = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.NbrOfConversion    = 1;
    hadc1.Init.ExternalTrigConv   = ADC_SOFTWARE_START;

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_ADCEx_Calibration_Start(&hadc1);

    /* Only one channel is used, so set it up once here */
    ch.Channel      = ADC_CHANNEL_0;
    ch.Rank         = ADC_REGULAR_RANK_1;
    ch.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;

    if (HAL_ADC_ConfigChannel(&hadc1, &ch) != HAL_OK)
    {
        Error_Handler();
    }
}


/* Returns a 12-bit reading, or 0 if the conversion times out. */
static uint16_t adc_read(void)
{
    uint16_t val = 0;

    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        val = (uint16_t)HAL_ADC_GetValue(&hadc1);
    }

    HAL_ADC_Stop(&hadc1);
    return val;
}


/* 8 MHz HSE x 9 = 72 MHz. APB1 is halved to 36 MHz (its max). */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef       rcc_osc = {0};
    RCC_ClkInitTypeDef       rcc_clk = {0};
    RCC_PeriphCLKInitTypeDef rcc_adc = {0};

    rcc_osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    rcc_osc.HSEState       = RCC_HSE_ON;
    rcc_osc.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    rcc_osc.PLL.PLLState   = RCC_PLL_ON;
    rcc_osc.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    rcc_osc.PLL.PLLMUL     = RCC_PLL_MUL9;

    if (HAL_RCC_OscConfig(&rcc_osc) != HAL_OK)
    {
        Error_Handler();
    }

    rcc_clk.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                           | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    rcc_clk.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    rcc_clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    rcc_clk.APB1CLKDivider = RCC_HCLK_DIV2;
    rcc_clk.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&rcc_clk, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }

    /* ADC clock = 72 MHz / 6 = 12 MHz, under the 14 MHz limit */
    rcc_adc.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    rcc_adc.AdcClockSelection    = RCC_ADCPCLK2_DIV6;

    if (HAL_RCCEx_PeriphCLKConfig(&rcc_adc) != HAL_OK)
    {
        Error_Handler();
    }
}


static void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}