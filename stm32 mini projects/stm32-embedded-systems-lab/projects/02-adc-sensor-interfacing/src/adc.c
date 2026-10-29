/*
 * adc.c
 * ADC1 driver for STM32F103C8T6. Polling reads on PA0 and PA1,
 * averaged over 8 samples to cut down noise.
 */

#include "adc.h"
#include "main.h"

#define ADC_OVERSAMPLE_COUNT    8U
#define ADC_TIMEOUT_MS          100U

static ADC_HandleTypeDef hadc1;


void adc_init(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};
    RCC_PeriphCLKInitTypeDef adc_clk = {0};

    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* The ADC clock must stay at or below 14 MHz: 72 MHz / 6 = 12 MHz */
    adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    adc_clk.AdcClockSelection    = RCC_ADCPCLK2_DIV6;
    HAL_RCCEx_PeriphCLKConfig(&adc_clk);

    gpio_cfg.Pin  = GPIO_PIN_0 | GPIO_PIN_1;
    gpio_cfg.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(GPIOA, &gpio_cfg);

    hadc1.Instance                   = ADC1;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode          = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.NbrOfConversion       = 1;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        while (1) { }
    }

    /* F1 needs a calibration run after every power-up */
    HAL_ADCEx_Calibration_Start(&hadc1);
}


/* Returns the averaged 12-bit reading (0 to 4095), or 0 for a bad channel. */
uint16_t adc_read_channel(uint8_t channel)
{
    ADC_ChannelConfTypeDef ch_cfg = {0};
    uint32_t sum = 0;
    uint8_t good = 0;

    if (channel == ADC_CHANNEL_POT)
    {
        ch_cfg.Channel = ADC_CHANNEL_0;     /* PA0 */
    }
    else if (channel == ADC_CHANNEL_TEMP)
    {
        ch_cfg.Channel = ADC_CHANNEL_1;     /* PA1 */
    }
    else
    {
        return 0;
    }

    ch_cfg.Rank         = ADC_REGULAR_RANK_1;
    ch_cfg.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;   /* slowest, most accurate */

    if (HAL_ADC_ConfigChannel(&hadc1, &ch_cfg) != HAL_OK)
    {
        return 0;
    }

    for (uint8_t i = 0; i < ADC_OVERSAMPLE_COUNT; i++)
    {
        HAL_ADC_Start(&hadc1);

        if (HAL_ADC_PollForConversion(&hadc1, ADC_TIMEOUT_MS) == HAL_OK)
        {
            sum += HAL_ADC_GetValue(&hadc1);
            good++;
        }

        HAL_ADC_Stop(&hadc1);
    }

    return (good > 0U) ? (uint16_t)(sum / good) : 0U;
}