# Observations - Project 05: Interrupt-Driven System

## Test Setup

- **Board:** STM32F103C8T6 Blue Pill clone (Cortex-M3)
- **Clock:** 72 MHz from the 8 MHz crystal, PLL x9
- **Supply:** 3.31 V measured at the VDD pins
- **Room temperature:** 24.2 C

## Results

| Metric | Target | Measured | Status |
|--------|--------|----------|--------|
| Interrupt response (EXTI edge to GPIO toggle) | < 1.0 us | 420 ns (about 30 CPU cycles) | PASS |
| Debounce lockout window | 50 ms | 50.2 ms (from SysTick) | PASS |
| Emergency stop latency | < 1.0 us | 380 ns | PASS |
| Current, busy-polling loop | about 28 mA | 27.8 mA | Recorded |
| Current, `__WFI()` sleep | < 15 mA | 11.4 mA (about 59% lower) | PASS |

## Scope Notes

- **Ch1 (yellow):** PB0 falling edge when the button is pressed. The contacts bounced for about 3.4 ms.
- **Ch2 (blue):** PA1, the RUNNING LED. The state changed cleanly on the first falling edge, and the bounce after it was ignored during the 50 ms lockout.
- **Preemption:** With PB0 held down, pressing PB1 switched straight to `STATE_EMERGENCY_STOP` with no visible delay.