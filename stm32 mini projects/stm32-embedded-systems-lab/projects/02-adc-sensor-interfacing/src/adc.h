/*
 * adc.h
 * ADC driver public API.
 */

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADC_CHANNEL_POT     0U      /* potentiometer on PA0 (ADC1_IN0) */
#define ADC_CHANNEL_TEMP    1U      /* LM35 on PA1 (ADC1_IN1) */

void adc_init(void);

/* Averaged 12-bit reading (0 to 4095), or 0 for an invalid channel. */
uint16_t adc_read_channel(uint8_t channel);

#endif