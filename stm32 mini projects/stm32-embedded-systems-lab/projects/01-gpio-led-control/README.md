# Project 01: GPIO LED Control

## 1. Objective

Control external LEDs from GPIO pins on the STM32F103C8T6 and run them in a couple of simple patterns. This is the first project in the series, so it also sets up the firmware layout, build workflow and test approach that the later projects reuse.

## 2. Hardware and Tools

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 (Blue Pill), 72 MHz, 64 KB Flash, 20 KB SRAM |
| LEDs | 3x 5 mm (red, yellow, green), each with a 330 ohm resistor |
| Button | Tactile switch with a 10 k pull-up (active-low) |
| On-board LED | PC13 (active-low) |
| Power | 3.3 V from the ST-Link or USB |
| IDE | STM32CubeIDE 1.14+ |
| Schematics | KiCad 8.x |
| Debugger | ST-Link V2 (SWD) |

## 3. Concepts Covered

- GPIO output as push-pull with no pull resistor
- GPIO input with a pull-up for an active-low button
- HAL calls used: `HAL_GPIO_Init()`, `HAL_GPIO_WritePin()`, `HAL_GPIO_ReadPin()`
- Blocking delays with `HAL_Delay()` (SysTick based)
- Polling the button (no debounce yet)
- Splitting the code into `main.c` and `gpio_control.c`

## 4. Circuit

The KiCad schematic is in [`kicad/`](kicad/).

| STM32 Pin | Mode | Connected to |
|-----------|------|--------------|
| PA0 | Output | Red LED (330 ohm to GND) |
| PA1 | Output | Yellow LED (330 ohm to GND) |
| PA2 | Output | Green LED (330 ohm to GND) |
| PB0 | Input | Button (10 k pull-up to 3.3 V, switch to GND) |
| PC13 | Output | On-board LED (active-low) |

> TODO: export the schematic from KiCad and put it in `kicad/screenshots/`.

## 5. Firmware Layout