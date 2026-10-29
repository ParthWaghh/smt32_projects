# Project 02: ADC Sensor Interfacing

## 1. Objective

Read a potentiometer and an LM35 temperature sensor with the STM32's 12-bit ADC, convert the raw readings to millivolts and degrees C, and use the on-board LED as a temperature threshold indicator.

## 2. Hardware and Tools

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 (Blue Pill) |
| Sensors | 10 k potentiometer, LM35 temperature sensor |
| Indicator | On-board LED (PC13, active-low), on when temperature is above 30 C |
| Power | 3.3 V from the ST-Link |
| IDE | STM32CubeIDE 1.14+ |
| Schematics | KiCad 8.x |

## 3. Concepts Covered

- ADC polling reads on two channels
- 12-bit resolution: 0 to 4095 maps to 0 to 3.3 V
- Linear conversion: `voltage_mv = raw * 3300 / 4095`
- LM35 output is 10 mV per degree C, so `temp_c = voltage_mv / 10`
- Averaging 8 samples to reduce noise
- Splitting the code into an ADC driver and a sensor module

## 4. Circuit

The KiCad schematic is in [`kicad/`](kicad/).

| STM32 Pin | Function | Connected to |
|-----------|----------|--------------|
| PA0 | ADC1_IN0 | Potentiometer wiper |
| PA1 | ADC1_IN1 | LM35 output |
| PC13 | GPIO output | Threshold LED |

## 5. Firmware Layout

```
main.c
  SystemClock_Config()            72 MHz, ADC clock 12 MHz
  adc_init()                      ADC1 setup and calibration
  sensor_init()                   placeholder for calibration
  main loop (every 100 ms)
    adc_read_channel(POT)         raw pot value
    adc_read_channel(TEMP)        raw LM35 value
    sensor_convert_voltage()      raw to mV
    sensor_convert_temperature()  raw to whole degrees C
    threshold check               LED on above 30 C
```

## 6. Implementation

### Design Notes

- **Polling mode:** Fine for reading two channels at 10 Hz. DMA is something to try later.
- **Oversampling:** Averaging 8 samples cuts random noise by about 2.8x (the square root of 8). That is not the same as gaining 3 extra bits, which would need 64 samples and a proper decimation step.
- **Separate `adc.c` and `sensor.c`:** The ADC driver only deals with hardware. The sensor module handles unit conversion.
- **Whole degrees:** The temperature is truncated to an integer, so the LED switches on at about 31 C, not 30 C.

## 7. Testing

The full test plan is in [`tests/test_plan.md`](tests/test_plan.md).

## 8. Results

ADC readings are logged to [`results/adc-readings.csv`](results/adc-readings.csv).

## 9. Limitations

- Blocking ADC reads would limit throughput if more channels were added.
- The LM35 datasheet gives a minimum supply of 4 V, but it runs at 3.3 V here, so readings may be off, especially at low temperatures.
- The LM35 is rated about +/-0.5 C at room temperature, and nothing has been checked against a reference thermometer.
- The ADC reference is assumed to be exactly 3.300 V. It is really the supply, so it can drift.
- The LED threshold has no hysteresis, so it may flicker right at 30 C.

## 10. Ideas for Later

- DMA-driven continuous conversion
- A moving-average filter in software
- Sending readings over UART (Project 04)
- Using the pot to set the temperature threshold

## 11. References

- [RM0008 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf), Section 11 (ADC)
- [LM35 Datasheet](https://www.ti.com/lit/ds/symlink/lm35.pdf)