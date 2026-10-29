# Test Plan - Project 03: PWM LED Dimmer

## Test Environment

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 |
| PWM | TIM3 CH1 on PA6, 1 kHz, 1000 steps (ARR = 999) |
| Input | Pot on PA0, read every 20 ms |
| Tools | Oscilloscope, multimeter |

## Test Cases

### TC-01: Duty at minimum

- **Setup:** Turn the pot fully to the GND end (about 0 V on PA0).
- **Expected:** LED is off. The scope shows PA6 sitting low.
- **Pass if:** PA6 stays at 0 V with no pulses.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-02: Duty at mid-point

- **Setup:** Set the pot to the middle (about 1.65 V on PA0).
- **Expected:** LED at medium brightness. The scope shows about 50% duty at 1 kHz (roughly 499 out of 1000 ticks, so 49.9%).
- **Pass if:** Measured duty is between 48% and 52%.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-03: Duty at maximum

- **Setup:** Turn the pot fully to the 3.3 V end.
- **Expected:** LED at full brightness. The duty tops out at 99.9%, so the scope shows a 1 kHz signal with a 1 us low pulse each period, not a flat high line.
- **Pass if:** Duty reads 99% or more. A tiny low glitch at the start of each period is expected, and you may need to zoom in to see it.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-04: PWM frequency

- **Setup:** Pot anywhere between the two ends. Measure the frequency on PA6.
- **Expected:** 1.00 kHz, which is a 1 ms period.
- **Pass if:** Within 1% of 1 kHz. The Blue Pill crystal is usually accurate to well under that, so a bigger error points to a wrong clock setting.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-05: Smooth dimming

- **Setup:** Turn the pot slowly from one end to the other while watching the LED.
- **Expected:** Brightness rises evenly with no visible jumps. Small flicker near the ends or between neighbouring values is normal, since ADC noise moves the duty by a few counts.
- **Pass if:** No visible stepping or sudden jumps over the whole sweep.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

## Known Risks

- **No true 100% duty:** The compare value tops out at 999 out of 1000, so the output is never fully high.
- **Brightness feels non-linear:** Eyes respond roughly logarithmically, so the LED may look like it reaches full brightness well before the pot is at the end. That's normal for linear duty, not a bug.
- **ADC noise:** The pot is read with a single sample and no averaging, so the duty may jitter by a few counts and cause slight flicker.