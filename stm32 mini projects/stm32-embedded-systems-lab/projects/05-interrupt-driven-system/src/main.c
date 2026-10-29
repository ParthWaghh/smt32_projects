/*
 * main.c
 * Project 05: Interrupt-Driven System
 *
 * Two buttons raise EXTI interrupts that post events to a queue. The main
 * loop pulls events off the queue and feeds them to a state machine, blinks
 * a heartbeat LED on PC13, and sleeps between interrupts.
 *
 * Target: STM32F103C8T6, 72 MHz from 8 MHz HSE + PLL.
 */

#include "stm32f1xx_hal.h"
#include "interrupt.h"
#include "state_machine.h"

#define HEARTBEAT_MS    500U

static void SystemClock_Config(void);
static void Error_Handler(void);


int main(void)
{
    fsm_t system_fsm;
    uint32_t last_heartbeat = 0;

    HAL_Init();
    SystemClock_Config();

    interrupt_gpio_init();
    fsm_init(&system_fsm);

    while (1)
    {
        while (interrupt_has_pending_event())
        {
            fsm_dispatch(&system_fsm, interrupt_get_next_event());
        }

        uint32_t now = HAL_GetTick();

        if ((now - last_heartbeat) >= HEARTBEAT_MS)
        {
            last_heartbeat = now;
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }

        /* Sleep until the next interrupt (a button or the 1 ms SysTick) */
        __WFI();
    }
}


/* 8 MHz HSE x 9 = 72 MHz. APB1 is halved to 36 MHz (its max). */
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