/*
 * sensor.c
 * Converts raw 12-bit ADC readings to millivolts and LM35 temperature.
 */

#include "sensor.h"

#define ADC_MAX_COUNT       4095U
#define VREF_MV             3300U
#define LM35_MV_PER_C       10U


void sensor_init(void)
{
    /* Nothing to set up yet. Could later measure VREF and adjust VREF_MV. */
}


uint32_t sensor_convert_voltage(uint16_t raw)
{
    return ((uint32_t)raw * VREF_MV) / ADC_MAX_COUNT;
}


/* LM35 outputs 10 mV per degree C, so T = mV / 10 (whole degrees). */
int32_t sensor_convert_temperature(uint16_t raw)
{
    return (int32_t)(sensor_convert_voltage(raw) / LM35_MV_PER_C);
}