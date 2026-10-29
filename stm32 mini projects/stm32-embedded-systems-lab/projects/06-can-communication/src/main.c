/**
 * @file    main.c
 * @brief   Project 06 — CAN Communication
 * @author  Parth Wagh
 *
 * Demonstrates CAN 2.0A frame transmission, hardware filtering, and reception.
 * Uses 500 kbps automotive timing with loopback / normal mode selection.
 */

#include "stm32f1xx_hal.h"
#include "can_driver.h"
#include "can_protocol.h"

void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* Heartbeat LED configuration on PC13 */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_13;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &gpio);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

    /* Initialize CAN in Loopback mode for verified self-testing (or NORMAL for physical bus) */
    can_driver_init(CAN_OPERATING_MODE_LOOPBACK);

    /* Accept messages with ID 0x100 to 0x10F (Mask 0x7F0) */
    can_driver_configure_filter(0x100, 0x7F0);

    vehicle_telemetry_t tx_telemetry = {
        .engine_rpm = 3200,
        .vehicle_speed = 650, /* 65.0 km/h */
        .coolant_temp_c = 88,
        .throttle_pct = 42,
        .system_state = 1
    };

    can_frame_t tx_frame;
    can_frame_t rx_frame;
    vehicle_telemetry_t rx_telemetry;

    uint32_t last_tx_tick = 0;

    while (1) {
        /* Transmit periodic telemetry at 10 Hz (100 ms interval) */
        if ((HAL_GetTick() - last_tx_tick) >= 100U) {
            last_tx_tick = HAL_GetTick();

            /* Simulate dynamic engine values */
            tx_telemetry.engine_rpm = (uint16_t)(3000 + (HAL_GetTick() % 1500));
            can_protocol_pack_telemetry(&tx_frame, &tx_telemetry);

            if (can_driver_transmit(&tx_frame) == HAL_OK) {
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            }
        }

        /* Check for received messages */
        if (can_driver_receive(&rx_frame)) {
            if (can_protocol_unpack_telemetry(&rx_frame, &rx_telemetry)) {
                /* Received telemetry successfully decoded */
                (void)rx_telemetry;
            }
        }
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9; /* 8 MHz * 9 = 72 MHz */
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2; /* 36 MHz (PCLK1 for CAN) */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; /* 72 MHz */
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}
