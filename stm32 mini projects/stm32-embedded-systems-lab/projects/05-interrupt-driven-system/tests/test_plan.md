# Test Plan - Project 05: Interrupt-Driven System

## Test Environment

| Item | Detail |
|------|--------|
| MCU | STM32F103C8T6 (Blue Pill), 72 MHz |
| Inputs | PB0 trigger button (EXTI0, falling edge), PB1 emergency button (EXTI1, falling edge) |
| Outputs | PA0 green (IDLE), PA1 yellow (RUNNING), PA2 red (ALERT), PC13 heartbeat |
| Tools | Two-channel oscilloscope, multimeter, ST-Link V2 |

## Test Cases

### TC-01: Interrupt response time

- **Setup:** Scope channel 1 on PB0, channel 2 on a spare pin. The callback only posts an event and doesn't toggle a pin, so for this test add a temporary pin write at the top of `HAL_GPIO_EXTI_Callback` and remove it afterwards. Trigger on the falling edge of PB0.
- **Expected:** The Cortex-M3 needs about 12 cycles (about 170 ns at 72 MHz) to enter the interrupt. The HAL handler and callback add more before the pin changes, so expect somewhere around 0.5 to 2 us, not 350 ns.
- **Pass if:** The delay from the PB0 edge to the debug pin is under 2 us. Write down which pin was used and how it was toggled.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-02: Debounce

- **Setup:** Press the trigger button 50 times, firmly and at a steady pace. Watch the state LEDs, or print the state over UART. Also capture one press on the scope to see the real bounce length.
- **Expected:** Each press moves the state forward exactly once (IDLE, RUNNING, ALERT, IDLE...). Bounce shorter than 50 ms is ignored. The firmware does not count presses, so "50 increments" is checked by watching the cycle, not by reading a counter.
- **Pass if:** After 50 presses the state is where 50 steps of the cycle should leave it (50 mod 3 = 2 steps from IDLE, so ALERT), with no skipped or doubled steps.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-03: Emergency stop priority

- **Setup:** Put the system in RUNNING or ALERT, then press the emergency button on PB1. Repeat from IDLE and from EMERGENCY_STOP itself.
- **Expected:** The state goes to EMERGENCY_STOP from any state, and the yellow and red LEDs light together and stay steady (no blinking). The LED changes in the main loop, so it happens a few microseconds after the interrupt, not inside it.
- **Pass if:** The LEDs show yellow plus red every time, with no visible delay.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-04: Emergency stop preempts the trigger handler

- **Setup:** Temporarily add a busy-wait of a few milliseconds inside the trigger branch of the callback, and toggle a debug pin at the start and end of it. Press PB0, then press PB1 while the debug pin is high.
- **Expected:** The PB1 handler runs inside the PB0 handler, since priority 1 beats priority 2. Both events end up in the queue, and the emergency one is processed last, so the final state is EMERGENCY_STOP.
- **Pass if:** The PB1 handler runs before the PB0 handler finishes, and the final state is EMERGENCY_STOP. Remove the busy-wait when done.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-05: Sleep current

- **Setup:** Put the multimeter in series with the 3.3 V supply. Measure with `__WFI()` in the loop, then with `__WFI()` removed so the loop runs flat out.
- **Expected:** Lower current with `__WFI()`. SysTick still wakes the core every 1 ms, so this is light sleep, and the saving is partial. The LEDs, regulator and pull-ups are included in the reading.
- **Pass if:** The `__WFI()` build draws clearly less, and every button press is still handled, with none missed.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

### TC-06: Queue overflow

- **Setup:** Pause the main loop in the debugger, press the buttons more than 8 times, then resume. Optional but quick.
- **Expected:** The first 7 events are kept (a ring buffer of 8 holds 7) and the rest are dropped, with no crash.
- **Pass if:** No hard fault, and the state machine resumes normally.
- **Actual result:** *fill in after testing*
- **Status:** [ ] Pass  [ ] Fail  [ ] Not tested

## Known Risks

- **Emergency stop clears too easily:** One press of the trigger button goes from EMERGENCY_STOP back to IDLE, even if PB1 is still held.
- **No debounce on PB1:** One emergency press may post several events. That's harmless for the state, but they can fill the queue.
- **Dropped events:** A full queue drops events, including the emergency one.
- **Light sleep only:** SysTick wakes the core every millisecond, so the sleep saving is limited.