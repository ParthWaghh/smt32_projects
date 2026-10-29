/**
 * @file    can_driver.c
 * @brief   bxCAN peripheral driver implementation for STM32F103C8T6.
 * @author  Parth Wagh
 */

#include "can_driver.h"
#include <string.h>

static CAN_HandleTypeDef hcan;
static uint32_t error_counter = 0;

HAL_StatusTypeDef can_driver_init(can_mode_t mode)
{
    GPIO_InitTypeDef gpio = {0};

    /* Enable CAN1 and GPIOA Clocks */
    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure PA11 (CAN_RX) and PA12 (CAN_TX) */
    gpio.Pin = GPIO_PIN_11;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Pin = GPIO_PIN_12;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio);

    /* Configure bxCAN Controller:
     * APB1 clock = 36 MHz
     * Target: 500 kbps
     * Prescaler = 4
     * Time Quanta = 1 + BS1(11) + BS2(6) = 18 TQ
     * Bitrate = 36 MHz / (4 * 18) = 500,000 bps
     */
    hcan.Instance = CAN1;
    hcan.Init.Prescaler = 4;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_11TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_6TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = ENABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = ENABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    switch (mode) {
        case CAN_OPERATING_MODE_LOOPBACK:
            hcan.Init.Mode = CAN_MODE_LOOPBACK;
            break;
        case CAN_OPERATING_MODE_SILENT:
            hcan.Init.Mode = CAN_MODE_SILENT;
            break;
        case CAN_OPERATING_MODE_SILENT_LOOPBACK:
            hcan.Init.Mode = CAN_MODE_SILENT_LOOPBACK;
            break;
        default:
            hcan.Init.Mode = CAN_MODE_NORMAL;
            break;
    }

    if (HAL_CAN_Init(&hcan) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef can_driver_configure_filter(uint32_t filter_id, uint32_t filter_mask)
{
    CAN_FilterTypeDef sFilterConfig = {0};

    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

    /* Left-align standard 11-bit ID into STID[10:0] */
    sFilterConfig.FilterIdHigh = (uint16_t)((filter_id << 5) & 0xFFFF);
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = (uint16_t)((filter_mask << 5) & 0xFFFF);
    sFilterConfig.FilterMaskIdLow = 0x0000;

    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Start CAN controller */
    if (HAL_CAN_Start(&hcan) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef can_driver_transmit(const can_frame_t *frame)
{
    CAN_TxHeaderTypeDef tx_header = {0};
    uint32_t tx_mailbox = 0;

    if (frame->is_extended) {
        tx_header.IDE = CAN_ID_EXT;
        tx_header.ExtId = frame->id;
    } else {
        tx_header.IDE = CAN_ID_STD;
        tx_header.StdId = frame->id;
    }

    tx_header.RTR = frame->is_rtr ? CAN_RTR_REMOTE : CAN_RTR_DATA;
    tx_header.DLC = frame->dlc;
    tx_header.TransmitGlobalTime = DISABLE;

    /* Check free mailboxes and transmit */
    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0) {
        error_counter++;
        return HAL_BUSY;
    }

    if (HAL_CAN_AddTxMessage(&hcan, &tx_header, (uint8_t*)frame->data, &tx_mailbox) != HAL_OK) {
        error_counter++;
        return HAL_ERROR;
    }

    return HAL_OK;
}

bool can_driver_receive(can_frame_t *frame)
{
    CAN_RxHeaderTypeDef rx_header = {0};

    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0) {
        return false;
    }

    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rx_header, frame->data) != HAL_OK) {
        error_counter++;
        return false;
    }

    if (rx_header.IDE == CAN_ID_EXT) {
        frame->id = rx_header.ExtId;
        frame->is_extended = true;
    } else {
        frame->id = rx_header.StdId;
        frame->is_extended = false;
    }

    frame->is_rtr = (rx_header.RTR == CAN_RTR_REMOTE);
    frame->dlc = (uint8_t)rx_header.DLC;

    return true;
}

uint32_t can_driver_get_error_count(void)
{
    return error_counter;
}
