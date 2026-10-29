/*
 * sensor.h
 * Sensor conversion public API.
 */

#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

void     sensor_init(void);
uint32_t sensor_convert_voltage(uint16_t raw);      /* raw count to mV */
int32_t  sensor_convert_temperature(uint16_t raw);  /* raw count to whole degrees C (LM35) */

#endif