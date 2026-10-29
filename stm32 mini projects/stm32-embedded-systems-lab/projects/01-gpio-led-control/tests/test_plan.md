# Test Plan - Project 01: GPIO LED Control

## Test Environment

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 (Blue Pill) |
| Clock | 72 MHz (HSE + PLL) |
| Debugger | ST-Link V2 over SWD |
| Power | 3.3 V from the ST-Link |
| Tools | Eyes and a multimeter |

## Test Cases

### TC-01: Sequential pattern (default)

- **Setup:** Board powered, button not pressed.
- **Expected:** Red, yellow and green light up one at a time, 500 ms each, then repeat. Only one LED is on at any moment, so a full cycle takes about 1.5 s.
- **Pass if:** Each LED lights alone, in order, for roughly 500 ms.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-02: All-blink pattern (button held)

- **Setup:** Board powered. Press and hold the button on PB0.
- **Expected:** All three external LEDs blink together, 250 ms on and 250 ms off (2 Hz).
- **Pass if:** The LEDs switch on and off in sync while the button is held, and the sequential pattern comes back after release.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-03: On-board LED follows red

- **Setup:** Board powered, button not pressed. Watch the PC13 LED during the sequential pattern.
- **Expected:** PC13 is on only while the red LED is on.
- **Pass if:** The on-board LED lights up during the red phase and at no other time.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-04: Button release goes back to sequential

- **Setup:** Hold the button so the all-blink pattern is running, then let go.
- **Expected:** The sequential pattern resumes. It can take up to one full pattern cycle to switch, because the code waits out the current delay before checking the button again.
- **Pass if:** No LED gets stuck on and the sequence restarts cleanly.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-05: Current measurement

- **Setup:** One LED on (red phase of the sequential pattern). Put the multimeter in series with the LED.
- **Expected:** I = (3.3 V - 2.0 V) / 330 ohm = about 3.9 mA.
- **Pass if:** The reading is between 3 and 6 mA.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

## Known Risks

- **Button bounce:** There's no debounce, so the pattern may flip a few times right at press or release. Fine for this project.
- **PC13 current limit:** PC13 can only sink about 3 mA, so don't hang anything extra on it.