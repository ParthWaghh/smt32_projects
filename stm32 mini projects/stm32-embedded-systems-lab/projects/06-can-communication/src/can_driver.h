/**
 * @file    can_driver.h
 * @brief   Hardware abstraction layer for STM32 bxCAN peripheral.
 * @author  Parth Wagh
 */

#ifndef _CAN_DRIVER_H_
#define _CAN_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

/* Operational Modes */
typedef enum {
    CAN_OPERATING_MODE_NORMAL = 0,
    CAN_OPERATING_MODE_LOOPBACK,
    CAN_OPERATING_MODE_SILENT,
    CAN_OPERATING_MODE_SILENT_LOOPBACK
} can_mode_t;

/* Generic CAN Message Container */
typedef struct {
    uint32_t id;         /* 11-bit standard or 29-bit extended ID */
    bool     is_extended;/* True if 29-bit identifier */
    bool     is_rtr;     /* Remote Transmission Request */
    uint8_t  dlc;        /* Data Length Code (0 to 8 bytes) */
    uint8_t  data[8];    /* Payload data */
} can_frame_t;

/* Public API */
HAL_StatusTypeDef can_driver_init(can_mode_t mode);
HAL_StatusTypeDef can_driver_configure_filter(uint32_t filter_id, uint32_t filter_mask);
HAL_StatusTypeDef can_driver_transmit(const can_frame_t *frame);
bool              can_driver_receive(can_frame_t *frame);
uint32_t          can_driver_get_error_count(void);

#endif /* _CAN_DRIVER_H_ */
