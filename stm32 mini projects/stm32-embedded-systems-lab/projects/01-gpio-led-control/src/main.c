/*
 * main.c
 * Project 01: GPIO LED Control
 *
 * Cycles red, yellow and green LEDs in sequence. While the button
 * is held, all LEDs blink together instead.
 *
 * Target: STM32F103C8T6 (Blue Pill), 72 MHz from 8 MHz HSE + PLL.
 *
 * Pins:
 *   PA0  red LED
 *   PA1  yellow LED
 *   PA2  green LED
 *   PC13 on-board LED (active-low)
 *   PB0  button (active-low, external pull-up)
 *
 * When importing into CubeIDE, generate the project from the .ioc
 * file first to get the startup code, linker script and HAL sources.
 */

#include "main.h"
#include "gpio_control.h"

static void SystemClock_Config(void);
static void Error_Handler(void);


int main(void)
{
    HAL_Init();
    SystemClock_Config();

    gpio_led_init();
    gpio_button_init();

    while (1)
    {
        if (gpio_button_is_pressed())
        {
            gpio_led_pattern_all_blink(250);
        }
        else
        {
            gpio_led_pattern_sequential(500);
        }
    }
}


/* 8 MHz HSE x 9 = 72 MHz SYSCLK. APB1 is halved to 36 MHz (its max). */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef rcc_osc = {0};
    RCC_ClkInitTypeDef rcc_clk = {0};

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

    /* 2 flash wait states are required at 72 MHz */
    if (HAL_RCC_ClockConfig(&rcc_clk, FLASH_LATENCY_2) != HAL_OK)
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