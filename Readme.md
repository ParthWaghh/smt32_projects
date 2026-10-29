# STM32 Embedded Systems Lab

A structured collection of embedded systems mini-projects built on the **STM32F103C8T6 (Blue Pill)** platform. Each project targets a specific peripheral subsystem and follows a disciplined firmware engineering workflow: requirements → schematic → implementation → test → documentation.

## Objectives

- Develop proficiency in **Embedded C** and the **STM32 Cortex-M3** architecture.
- Interface analog and digital peripherals using **HAL** and, where instructive, direct register access.
- Implement timer-based control (PWM) and interrupt-driven firmware.
- Explore **UART** and **CAN** communication protocols.
- Practice **modular firmware design**, systematic testing, and engineering documentation.

## Technology Stack

| Category            | Tool / Component                                      |
|---------------------|-------------------------------------------------------|
| **MCU**             | STM32F103C8T6 (Cortex-M3, 72 MHz, 64 KB Flash)       |
| **Language**        | Embedded C (C11)                                      |
| **IDE**             | STM32CubeIDE 1.x                                      |
| **Code generator**  | STM32CubeMX (integrated)                              |
| **Schematics**      | KiCad 8.x                                             |
| **Version control** | Git + GitHub                                          |
| **Testing**         | Manual test plans, serial-log analysis, oscilloscope   |
| **Debugger**        | ST-Link V2 / SWD                                      |

## Projects

| # | Project                      | Key Concepts                          | Status  |
|---|------------------------------|---------------------------------------|---------|
| 01 | [GPIO LED Control](projects/01-gpio-led-control/)           | GPIO, digital outputs, polling        | Completed |
| 02 | [ADC Sensor Interfacing](projects/02-adc-sensor-interfacing/) | ADC, voltage scaling, DMA             | Completed |
| 03 | [PWM LED Dimmer](projects/03-pwm-led-dimmer/)               | Timers, PWM, duty-cycle control       | Completed |
| 04 | [UART Sensor Monitor](projects/04-uart-sensor-monitor/)     | UART, serial debugging, telemetry     | Completed |
| 05 | [Interrupt-Driven System](projects/05-interrupt-driven-system/) | EXTI, NVIC, state machines        | Ongoing |
| 06 | [CAN Communication](projects/06-can-communication/)         | bxCAN, CAN frames, filtering         | Ongoing |


Each project is **self-contained**: independent source tree, schematic, test plan, and results. When STM32CubeIDE project files (`.ioc`, startup, linker script, HAL drivers) are generated, they live inside the respective project directory.

## Validation Approach

Every project documents:

1. **Test cases** — input conditions and expected peripheral behaviour.
2. **Actual results** — screenshots, logic-analyser captures, serial logs, or oscilloscope readings.
3. **Known limitations** — anything not validated or not implemented.

Hardware test results and simulation results are labelled separately so reviewers know the evidence source.

## Current Limitations

- KiCad schematics are provided for documentation and PCB review. Physical builds use the off-the-shelf Blue Pill module.
- CAN peripheral testing requires either two physical nodes or a virtual-CAN bridge; see [Project 06](projects/06-can-communication/) for the test strategy.
- No RTOS is used in these projects; all firmware is bare-metal with HAL.

## Build & Flash (General)

```bash
# Open any project in STM32CubeIDE:
#   File → Import → Existing Projects into Workspace
#   Browse to projects/0X-project-name/
#
# Build:  Project → Build All   (or Ctrl+B)
# Flash:  Run → Debug As → STM32 C/C++ Application
```

Detailed per-project instructions are in each project's own `README.md`.

## Progress

Project status is tracked in the table above. Each project's README is updated with implementation details, test evidence, and lessons learned as work progresses.

## Author

**Parth Wagh**

Electronics and Telecommunication Engineering · Embedded Systems · Firmware Development

---

*This repository is maintained as a learning portfolio. Contributions, suggestions, and code reviews are welcome — see [CONTRIBUTING.md](CONTRIBUTING.md).*
