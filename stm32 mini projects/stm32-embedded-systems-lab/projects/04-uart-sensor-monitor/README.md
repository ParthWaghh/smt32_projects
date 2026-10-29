# Project 04: UART Sensor Monitor

## 1. Objective

Send sensor readings from the STM32 to a PC terminal over a serial link. The board reads a potentiometer with the ADC, formats the result as a line of text, and sends it over USART1 once per second at 115200 baud.

## 2. Hardware and Tools

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 (Blue Pill) |
| UART | USART1 on PA9 (TX) and PA10 (RX) |
| USB-serial adapter | CP2102, CH340 or FTDI (3.3 V logic) |
| Terminal | PuTTY, Tera Term or minicom (115200 8N1) |
| Sensor | Potentiometer on PA0 |
| IDE | STM32CubeIDE 1.14+ |

## 3. Concepts Covered

- USART setup: baud rate, word length, stop bits and parity
- HAL calls used: `HAL_UART_Transmit()` and `HAL_UART_Receive()`
- Formatting text with `snprintf()`, which is bounded by the buffer size
- Designing a line format that is easy to read and easy to parse
- Baud rate maths: USARTDIV = f_PCLK2 / (16 x baud) = 72 MHz / (16 x 115200) = 39.0625, which the baud register holds exactly

## 4. Circuit

The KiCad schematic is in [`kicad/`](kicad/).

| STM32 Pin | Function | Connected to |
|-----------|----------|--------------|
| PA9 | USART1_TX | USB-serial RX |
| PA10 | USART1_RX | USB-serial TX |
| GND | Ground | USB-serial GND |
| PA0 | ADC1_IN0 | Potentiometer wiper |

> Do not connect the adapter's 5 V pin to the Blue Pill's 3.3 V pin. Power the board from the ST-Link or USB, and connect only TX, RX and GND to the adapter.

## 5. Firmware Layout

```
main.c
  SystemClock_Config()        72 MHz, ADC clock 12 MHz
  adc_init()                  ADC1 on PA0, set up in main.c
  uart_init()                 USART1, 115200 8N1
  banner                      two lines sent once at startup
  main loop (every 1000 ms)
    adc_read()                raw value, 0 to 4095
    mv = raw * 3300 / 4095
    telemetry_format()        build the text line
    uart_transmit_string()    send it
```

### Line Format

```
[T:00012345] ADC0=2048 V=1650mV STATUS=OK
```

Each line ends in `\r\n`.

| Field | Meaning |
|-------|---------|
| `T:` | Milliseconds since reset, from `HAL_GetTick()` (8 digits) |
| `ADC0=` | Raw 12-bit reading (4 digits) |
| `V=` | Voltage in millivolts, rounded down (4 digits) |
| `STATUS=` | Currently always `OK` |

## 6. Implementation

### Design Notes

- **Polling UART:** Fine at one line per second. Interrupt or DMA transmit can come later.
- **`snprintf()` instead of `printf()`:** No retargeting of `printf` is needed, and the output can't run past the buffer.
- **Integer millivolts:** Avoids float formatting, which would pull a lot of extra code into flash.
- **ADC code lives in `main.c`:** This project has its own small single-channel routine and does not reuse the Project 02 driver.
- **Delay after send:** The loop reads, prints and then waits 1000 ms, so the first timestamp is near 0 and each line is a few ms later than a perfect 1 s step.

## 7. Testing

The full test plan is in [`tests/test_plan.md`](tests/test_plan.md).

## 8. Results

A serial capture is saved to [`results/uart-log.txt`](results/uart-log.txt).

## 9. Limitations

- Sending blocks the main loop. A line of about 46 characters takes roughly 4 ms at 115200.
- `STATUS` is hard-coded to `OK`, so the firmware never reports a warning.
- RX is set up (`uart_receive_byte()` exists) but nothing uses it yet, so there are no commands.
- The USART error flags are never read, so there is no frame error count.
- No flow control (CTS/RTS).

## 10. Ideas for Later

- Interrupt or DMA based transmit
- A real status field, such as `WARN_HI` when the reading hits full scale
- A command parser on RX, for example to change the report interval
- A checksum on each line
- Adding the LM35 channel from Project 02 to the line

## 11. References

- [RM0008 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf), Section 27 (USART)