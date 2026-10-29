/*
 * uart.c
 * Blocking UART driver for USART1 (PA9 TX, PA10 RX), 115200 8N1.
 */

#include "uart.h"
#include "main.h"
#include <string.h>

#define UART_TX_TIMEOUT_MS  1000U

static UART_HandleTypeDef huart1;


void uart_init(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};

    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA9: TX, alternate function push-pull */
    gpio_cfg.Pin   = GPIO_PIN_9;
    gpio_cfg.Mode  = GPIO_MODE_AF_PP;
    gpio_cfg.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_cfg);

    /* PA10: RX, floating input */
    gpio_cfg.Pin  = GPIO_PIN_10;
    gpio_cfg.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &gpio_cfg);

    huart1.Instance        = USART1;
    huart1.Init.BaudRate   = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits   = UART_STOPBITS_1;
    huart1.Init.Parity     = UART_PARITY_NONE;
    huart1.Init.Mode       = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        while (1)
        {
        }
    }
}


void uart_transmit_string(const char *str)
{
    uint16_t len = (uint16_t)strlen(str);

    HAL_UART_Transmit(&huart1, (const uint8_t *)str, len, UART_TX_TIMEOUT_MS);
}


void uart_transmit(const uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit(&huart1, data, len, UART_TX_TIMEOUT_MS);
}


/* Waits up to timeout ms for one byte. Returns HAL_OK or HAL_TIMEOUT. */
HAL_StatusTypeDef uart_receive_byte(uint8_t *byte, uint32_t timeout)
{
    return HAL_UART_Receive(&huart1, byte, 1, timeout);
}