/*
 * main.c
 * Project 02: ADC Sensor Interfacing
 *
 * Reads a potentiometer on PA0 and an LM35 on PA1 every 100 ms.
 * The on-board LED (PC13, active-low) turns on when the temperature
 * is above 30 C.
 *
 * Target: STM32F103C8T6, 72 MHz from 8 MHz HSE + PLL.
 */

#include "main.h"
#include "adc.h"
#include "sensor.h"

#define TEMP_THRESHOLD_C    30
#define SAMPLE_PERIOD_MS    100U

#define LED_PORT            GPIOC
#define LED_PIN             GPIO_PIN_13

static void SystemClock_Config(void);
static void led_init(void);
static void Error_Handler(void);


int main(void)
{
    HAL_Init();
    SystemClock_Config();

    led_init();
    adc_init();
    sensor_init();

    while (1)
    {
        uint16_t raw_pot  = adc_read_channel(ADC_CHANNEL_POT);
        uint16_t raw_temp = adc_read_channel(ADC_CHANNEL_TEMP);

        uint32_t voltage_mv = sensor_convert_voltage(raw_pot);
        int32_t  temp_c     = sensor_convert_temperature(raw_temp);

        (void)voltage_mv;   /* not used yet, handy to watch in the debugger */

        if (temp_c > TEMP_THRESHOLD_C)
        {
            HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
        }

        HAL_Delay(SAMPLE_PERIOD_MS);
    }
}


static void led_init(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* High = off, so the LED doesn't flash on startup */
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    gpio_cfg.Pin   = LED_PIN;
    gpio_cfg.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_cfg.Pull  = GPIO_NOPULL;
    gpio_cfg.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &gpio_cfg);
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