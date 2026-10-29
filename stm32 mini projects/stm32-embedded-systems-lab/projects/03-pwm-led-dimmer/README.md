# Project 03: PWM LED Dimmer

## 1. Objective

Use a hardware timer to generate a PWM signal that controls LED brightness. A potentiometer sets the duty cycle, so turning it dims or brightens the LED in real time.

## 2. Hardware and Tools

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 (Blue Pill) |
| LED | 1x high-brightness LED with a 330 ohm resistor on PA6 (TIM3_CH1) |
| Potentiometer | 10 k on PA0 (ADC1_IN0) for brightness control |
| IDE | STM32CubeIDE 1.14+ |
| Schematics | KiCad 8.x |
| Measurement | Oscilloscope or logic analyser on PA6 |

## 3. Concepts Covered

- Timer setup: prescaler, auto-reload register (ARR) and output compare
- PWM mode 1 on TIM3 channel 1
- Duty cycle = CCR / (ARR + 1) x 100%
- Picking 1 kHz, which is well above the point where flicker is visible
- Mapping the ADC reading to a compare value
- How the timer clock, prescaler and ARR set the PWM frequency

## 4. Circuit

The KiCad schematic is in [`kicad/`](kicad/).

| STM32 Pin | Function | Connected to |
|-----------|----------|--------------|
| PA6 | TIM3_CH1 (PWM output) | LED through 330 ohm to GND |
| PA0 | ADC1_IN0 | Potentiometer wiper |

### PWM Frequency

```
Timer clock  = 72 MHz (APB1 is 36 MHz, and timers run at double that
               when the APB1 prescaler is not 1)
Prescaler    = 72 - 1 = 71, so the counter clock is 1 MHz
ARR          = 1000 - 1 = 999
PWM freq     = 1 MHz / 1000 = 1 kHz
CCR range    = 0 to 999 (0% to 99.9% duty)
```

## 5. Firmware Layout

```
main.c
  SystemClock_Config()      72 MHz, ADC clock 12 MHz
  adc_init()                ADC1 on PA0, set up in main.c
  pwm_init(), pwm_start()   TIM3 CH1, 1 kHz, PWM mode 1
  main loop (every 20 ms)
    adc_read()              raw pot value, 0 to 4095
    duty = raw * 999 / 4095
    pwm_set_duty(duty)
```

## 6. Implementation

### Design Notes

- **1 kHz PWM:** Far above the roughly 60 Hz where flicker becomes visible. It is also low enough that the timer has 1000 steps of resolution.
- **Hardware PWM:** The timer produces the waveform on its own, so the output stays steady even while the main loop is busy reading the ADC or waiting in `HAL_Delay()`.
- **Linear mapping:** The ADC value maps straight to the compare value, with no gamma correction. That is fine for a single LED.
- **ADC code lives in `main.c`:** This project has its own small single-channel ADC routine and does not reuse the Project 02 driver.
- **Duty tops out at 99.9%:** The highest compare value is 999 with ARR at 999, so the output is never fully high.

## 7. Testing

The full test plan is in [`tests/test_plan.md`](tests/test_plan.md).

## 8. Results

PWM measurements are logged to [`results/pwm-measurements.csv`](results/pwm-measurements.csv).

## 9. Limitations

- No gamma correction, so perceived brightness is not linear with duty cycle.
- The duty never reaches a true 100%, and a 1 us low pulse remains each period at the top of the pot's travel.
- The pot is read once per update with no averaging, so the duty can jitter by a few counts.
- Only one channel is used.

## 10. Ideas for Later

- A gamma correction lookup table for more even-looking dimming
- Three channels (RGB LED) with TIM3 CH1 to CH3
- Reporting the current duty cycle over UART
- Letting the duty reach a true 100% (scale by 1000 and allow a compare value of 1000)

## 11. References

- [RM0008 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf), Section 15 (general-purpose timers)