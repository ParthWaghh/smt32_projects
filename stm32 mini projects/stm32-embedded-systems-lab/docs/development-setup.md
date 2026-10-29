# Development Setup

## Prerequisites

| Tool | Version | Purpose |
|------|---------|---------|
| **STM32CubeIDE** | 1.14+ | IDE, compiler (arm-none-eabi-gcc), debugger |
| **STM32CubeMX** | 6.10+ | Peripheral initialisation code generator (integrated in CubeIDE) |
| **KiCad** | 8.0+ | Schematic capture and PCB layout |
| **Git** | 2.40+ | Version control |
| **ST-Link V2** | — | SWD debug probe (for hardware testing) |
| **USB-to-Serial adapter** | — | UART monitoring (CP2102 / CH340 / FTDI) |

## Installing STM32CubeIDE

1. Download from [st.com/stm32cubeide](https://www.st.com/en/development-tools/stm32cubeide.html).
2. Install with default options. The embedded GCC toolchain is bundled.
3. On first launch, select a workspace directory (can be this repo root).

## Installing KiCad

1. Download from [kicad.org](https://www.kicad.org/download/).
2. Install the default symbol and footprint libraries.
3. Schematics in this repo use the built-in STM32 symbols where available, plus custom symbols documented in each project.

## Cloning the Repository

```bash
git clone https://github.com/YOUR_USERNAME/stm32-embedded-systems-lab.git
cd stm32-embedded-systems-lab
```

## Opening a Project in STM32CubeIDE

1. **File → Import → General → Existing Projects into Workspace**.
2. Set root directory to `projects/0X-project-name/`.
3. Check the project checkbox and click **Finish**.
4. If prompted, let CubeIDE generate missing HAL drivers from the `.ioc` file.

> **Note:** The `.ioc` file is the STM32CubeMX configuration. It defines the MCU, clock tree, pin assignments, and enabled peripherals. CubeIDE regenerates `main.c`, startup code, and the linker script from this file.

## Hardware Connections

### Blue Pill Pinout Reference

```
         ┌──────────────┐
    PB12 │1           40│ PA0
    PB13 │2           39│ PA1
    PB14 │3           38│ PA2
    PB15 │4           37│ PA3
    PA8  │5           36│ PA4
    PA9  │6           35│ PA5
    PA10 │7           34│ PA6
    PA11 │8           33│ PA7
    PA12 │9           32│ PB0
    PA15 │10          31│ PB1
    PB3  │11          30│ PB10
    PB4  │12          29│ PB11
    PB5  │13          28│ RESET
    PB6  │14          27│ 3.3V
    PB7  │15          26│ GND
    PB8  │16          25│ GND
    PB9  │17          24│ 5V
    5V   │18          23│ 3.3V
    GND  │19          22│ PA13 (SWDIO)
    3.3V │20          21│ PA14 (SWCLK)
         └──────────────┘
```

### ST-Link V2 Wiring

| ST-Link Pin | Blue Pill Pin |
|-------------|---------------|
| SWDIO       | PA13          |
| SWCLK       | PA14          |
| GND         | GND           |
| 3.3V        | 3.3V          |

## Compiler Settings

- **Optimisation:** `-O0` during development/debug, `-Os` for release.
- **Warnings:** `-Wall -Wextra -Wpedantic`.
- **Standard:** `-std=c11`.
- **Target:** `-mcpu=cortex-m3 -mthumb`.

These are configured in STM32CubeIDE project properties and should not need manual editing unless customising the build.
