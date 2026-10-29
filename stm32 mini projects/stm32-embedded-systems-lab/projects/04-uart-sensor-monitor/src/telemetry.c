/*
 * telemetry.c
 * Formats one telemetry line, e.g. "[T:00012345] ADC0=2048 V=1650mV STATUS=OK".
 */

#include "telemetry.h"
#include <stdio.h>


/* buf needs at least 48 bytes. STATUS is always OK for now. */
void telemetry_format(char *buf, uint16_t buf_size,
                      uint32_t tick_ms, uint16_t adc_raw, uint32_t voltage_mv)
{
    snprintf(buf, buf_size,
             "[T:%08lu] ADC0=%04u V=%04lumV STATUS=OK\r\n",
             (unsigned long)tick_ms,
             (unsigned)adc_raw,
             (unsigned long)voltage_mv);
}