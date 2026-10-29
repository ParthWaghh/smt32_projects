/*
 * telemetry.h
 * Telemetry line formatter public API.
 */

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

/* Writes one line like "[T:00012345] ADC0=2048 V=1650mV STATUS=OK\r\n" into buf. */
void telemetry_format(char *buf, uint16_t buf_size,
                      uint32_t tick_ms, uint16_t adc_raw, uint32_t voltage_mv);

#endif