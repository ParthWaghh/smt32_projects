# Project 05: Interrupt-Driven System

## 1. Objective

Handle two push buttons with external interrupts (EXTI) instead of polling, and feed the events into a small state machine. One button steps through three states, and the other is an emergency stop that overrides everything. The main loop sleeps between interrupts.

## 2. Hardware and Tools

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 (Blue Pill, Cortex-M3, 72 MHz) |
| Trigger button | Tactile switch on PB0 (EXTI0, active-low, internal pull-up) |
| Emergency button | Tactile switch on PB1 (EXTI1, active-low, internal pull-up) |
| State LEDs | PA0 green (IDLE), PA1 yellow (RUNNING), PA2 red (ALERT), each with a 330 ohm resistor |
| Heartbeat | On-board LED on PC13 (active-low), toggled every 500 ms |
| IDE and toolchain | STM32CubeIDE 1.14+ (arm-none-eabi-gcc) |
| Schematics | KiCad 8.x |
| Measurement | Oscilloscope, multimeter |

## 3. Concepts Covered

- NVIC priorities: PB1 (priority 1) can interrupt the PB0 handler (priority 2)
- EXTI falling-edge interrupts on two lines
- A ring-buffer event queue shared between the ISRs and the main loop
- Short critical section so one ISR can't corrupt the queue while another preempts it
- A four-state machine: IDLE, RUNNING, ALERT and EMERGENCY_STOP
- Tick-based debounce with no delays inside an ISR
- Sleeping with `__WFI()` between events

## 4. Circuit

The KiCad schematic is in [`kicad/interrupt-driven-system.kicad_sch`](kicad/interrupt-driven-system.kicad_sch).

| MCU Pin | Function | Connected to |
|---------|----------|--------------|
| PB0 | EXTI0 trigger | Button to GND (internal pull-up) |
| PB1 | EXTI1 emergency | Button to GND (internal pull-up) |
| PA0 | Output | Green LED, IDLE (330 ohm) |
| PA1 | Output | Yellow LED, RUNNING (330 ohm) |
| PA2 | Output | Red LED, ALERT (330 ohm) |
| PC13 | Output | On-board LED, heartbeat (active-low) |

## 5. Firmware Layout

The ISRs only post events. The main loop does the work.

```
button edge (EXTI0 or EXTI1)
  HAL_GPIO_EXTI_Callback()
    PB0: ignore if less than 50 ms since the last accepted press
    post an event to the queue

main loop
  while the queue has events
    fsm_dispatch()        change state, update the LEDs
  toggle PC13 every 500 ms
  __WFI()                 sleep until the next interrupt
```

Files:

```
main.c            clock setup and the main loop
interrupt.c/.h    GPIO and NVIC setup, EXTI callback, event queue
state_machine.c/.h  the state machine and LED outputs
```

### State Transitions

| State | Trigger press | Emergency press |
|-------|---------------|-----------------|
| IDLE (green) | RUNNING | EMERGENCY_STOP |
| RUNNING (yellow) | ALERT | EMERGENCY_STOP |
| ALERT (red) | IDLE | EMERGENCY_STOP |
| EMERGENCY_STOP (yellow and red together) | IDLE | stays |

## 6. Implementation

### Building

1. Open STM32CubeIDE and import this folder.
2. Build the project.
3. Connect the ST-Link V2 to the SWD header (GND, SWCLK, SWDIO, 3.3 V).
4. Run or debug from the IDE.

### Design Notes

- **ISRs stay short:** The callback only checks the debounce time and posts an event. The LEDs change in the main loop.
- **Debounce:** PB0 ignores presses within 50 ms of the last accepted one, using `HAL_GetTick()`. PB1 has no debounce, so the stop happens on the first edge.
- **Queue:** 8 slots, which holds 7 events. If it is full, new events are dropped. Posting is wrapped in a short interrupt-disabled section, because the emergency ISR can preempt the trigger ISR halfway through a post.
- **Sleep:** `__WFI()` stops the core until an interrupt. SysTick fires every 1 ms, so the core wakes about 1000 times a second. This is light sleep, not a deep power-saving mode.

## 7. Testing

The full test plan is in [`tests/test_plan.md`](tests/test_plan.md). It covers:

- Interrupt response time, measured with a temporary debug pin
- Debounce over 50 presses
- Emergency stop from every state
- Preemption of the trigger handler by the emergency handler
- Current with and without `__WFI()`
- Queue overflow

## 8. Results

Measurements are recorded in [`results/observations.md`](results/observations.md).

## 9. Limitations

- Debounce is a time check in the callback, so a press in the first 50 ms after reset is ignored.
- PB1 has no debounce, so one press can post several emergency events and crowd out others in the queue.
- A full queue drops events, including the emergency one.
- One press of the trigger button clears the emergency stop, even if PB1 is still held.
- The emergency state lights yellow and red steadily. It doesn't blink.
- Sleep is light because SysTick keeps waking the core every millisecond.
- There is no UART output yet, so the current state can only be seen on the LEDs.

## 10. Ideas for Later

- Check that PB1 is released before leaving EMERGENCY_STOP, or use a separate reset
- Debounce PB1, or use a separate flag for the emergency so it can never be dropped
- Blink the LEDs in the emergency state
- A timeout in ALERT, using `state_entry_tick`
- Report the state over UART (Project 04)
- Stop mode with an EXTI wake-up for much lower standby current

## 11. References

- [RM0008 Reference Manual](https://www.st.com/resource/en/reference_manual/cd00171190.pdf), EXTI and NVIC chapters
- [Cortex-M3 Programming Manual (PM0056)](https://www.st.com/resource/en/programming_manual/cd00228163.pdf)