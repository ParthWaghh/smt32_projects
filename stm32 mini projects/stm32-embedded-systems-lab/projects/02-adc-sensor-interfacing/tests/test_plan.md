# Test Plan - Project 02: ADC Sensor Interfacing

## Test Environment

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 |
| ADC clock | 12 MHz (PCLK2 / 6) |
| Sample time | 239.5 cycles |
| Oversampling | 8 samples averaged |
| Tools | Multimeter, debugger (Live Expressions) |

## Test Cases

### TC-01: Potentiometer full range

- **Setup:** Pot on PA0. Turn it slowly from one end to the other.
- **Expected:** Reads close to 0 at 0 V and close to 4095 at 3.3 V, rising smoothly in between.
- **Pass if:** The voltage the firmware reports is within a few percent of what the multimeter shows on PA0 at several points along the sweep.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-02: LM35 at room temperature

- **Setup:** LM35 sitting at ambient temperature, about 25 C.
- **Expected:** About 250 mV on PA1, reported as 25 C. The firmware only gives whole degrees (it cuts off the decimals), so 25.9 C would show as 25.
- **Pass if:** Reported temperature is within 2 C of a reference thermometer.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-03: Temperature threshold LED

- **Setup:** Warm the LM35 with your fingers. A heat gun isn't needed and is easy to overdo.
- **Expected:** The PC13 LED turns on once the reported temperature goes above 30 C. Because of the whole-degree cutoff, that happens at about 31 C actual. It turns off again when the sensor cools.
- **Pass if:** LED is off below 30 C and on above it.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-04: ADC noise floor

- **Setup:** Short PA0 to GND (or turn the pot fully to the GND end).
- **Expected:** Reading stays at 5 counts or less.
- **Pass if:** No reading goes above 5.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-05: Oversampling effect

- **Setup:** Leave the pot at a fixed position. Log about 100 readings with `ADC_OVERSAMPLE_COUNT` set to 1, then another 100 with it set to 8.
- **Expected:** Single reads wander by roughly 10 counts. The 8-sample average should be about a third of that, since averaging N samples cuts random noise by about the square root of N (about 2.8 for N = 8).
- **Pass if:** The spread with 8 samples is clearly smaller than with 1.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

## Known Risks

- **LM35 supply:** The datasheet minimum is 4 V and the board runs it at 3.3 V. Readings may be off, especially below room temperature. Check TC-02 against a reference thermometer.
- **No hysteresis:** The LED threshold has none, so it may flicker if the temperature sits right at 30 C.