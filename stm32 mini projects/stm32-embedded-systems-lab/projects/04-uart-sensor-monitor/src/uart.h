/*
 * uart.h
 * UART driver public API (USART1, PA9 TX, PA10 RX, 115200 8N1).
 */

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "stm32f1xx_hal.h"

void              uart_init(void);
void              uart_transmit_string(const char *str);
void              uart_transmit(const uint8_t *data, uint16_t len);

/* Waits up to timeout ms for one byte. Returns HAL_OK or HAL_TIMEOUT. */
HAL_StatusTypeDef uart_receive_byte(uint8_t *byte, uint32_t timeout);

#endif