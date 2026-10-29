# Test Plan - Project 04: UART Sensor Monitor

## Test Environment

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 (Blue Pill) |
| Peripheral | USART1 (PA9 TX, PA10 RX) |
| Clock | 72 MHz (HSE + PLL), PCLK2 = 72 MHz |
| Serial settings | 115200 baud, 8N1 |
| Input | Pot on PA0 (ADC1_IN0), read once per second |
| Tools | USB-serial adapter with PuTTY or Tera Term (or a Python script), oscilloscope or logic analyser |

## Test Cases

### TC-01: Baud rate

- **Setup:** Probe PA9 with the scope or logic analyser while the board is sending lines.
- **Expected:** Bit width of about 8.68 us (1 / 115200 = 8.68 us). The USART divider works out exactly (72 MHz / 115200 = 625), so any error should come from the crystal or the scope.
- **Pass if:** Measured bit width is within 2% of 8.68 us and the terminal shows clean text with no garbage characters.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-02: Line format and timestamps

- **Setup:** Open the terminal and log at least 100 lines.
- **Expected:** The banner prints first, then one line per second like `[T:00012345] ADC0=2048 V=1650mV STATUS=OK`, ending in CRLF. Timestamps always increase. Each step is a little over 1000 ms, because the ADC read and the UART transmit (about 4 ms) are added to the 1000 ms delay. The first timestamp is near 0, not 1000.
- **Pass if:** Every line matches `^\[T:\d{8}\] ADC0=\d{4} V=\d{4}mV STATUS=\w+\r\n$`, and the gap between lines is 1000 to 1010 ms.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-03: ADC value range and buffer size

- **Setup:** Turn the pot slowly from 0 V to 3.3 V and back while logging.
- **Expected:** `ADC0` goes from about 0000 to 4095 and `V` from about 0000mV to 3300mV. The voltage matches `raw * 3300 / 4095` (rounded down). The longest line is about 46 characters, well inside the 128 byte buffer in `main.c`, so nothing gets cut off.
- **Pass if:** No line is truncated, the numbers always have four digits, and the board doesn't hang or hard fault.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-04: Cable disconnect and reconnect

- **Setup:** While it's streaming, unplug the USB-serial adapter (or pull the PA9 jumper), wait about 5 seconds, then plug it back in. Reopen the terminal if it lost the port.
- **Expected:** The firmware keeps running, since TX doesn't wait for a receiver. Lines sent while disconnected are lost, so the timestamps show a gap of about 5 seconds when output comes back.
- **Pass if:** Output resumes by itself within about a second of reconnecting, with no reset or hang. The on-board behaviour doesn't change.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

## Known Risks

- **STATUS is always OK:** `telemetry.c` hard-codes it, so no WARN_HI line can show up until that is changed.
- **No error counting:** The firmware doesn't read the USART error flags, so "no framing errors" can only be judged from garbage in the terminal, not from a counter.
- **Timestamp drift:** The delay runs after the read and transmit, so each line is a few ms late and it adds up over a long log.