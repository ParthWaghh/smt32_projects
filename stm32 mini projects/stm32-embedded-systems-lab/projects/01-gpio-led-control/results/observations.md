# Observations — Project 01: GPIO LED Control

## Test Date

2025-10-28

## Test Setup

- **Board:** STM32F103C8T6 Blue Pill
- **Power:** 3.3 V via ST-Link V2
- **LEDs:** 5 mm Red, Yellow, Green (330 Ω series resistors)
- **Button:** Tactile switch with 10 kΩ external pull-up

## Results Summary

| Test Case | Result | Notes |
|-----------|--------|-------|
| TC-01: Sequential pattern | PASS | R → Y → G cycles cleanly, ~500 ms per step |
| TC-02: All blink | PASS | All three toggle together, no visible lag between them |
| TC-03: On-board LED | PASS | PC13 works, but it's active-low (0 = ON), which tripped me up at first |
| TC-04: Button release | PASS | Pattern resumes on release; one stray double-trigger seen before adding a delay |

## Detailed Observations

**TC-01: Sequential pattern**
Flashed and ran on the first try. Red, yellow and green light up one after another and the timing looks even by eye. Checked one full cycle against a stopwatch on my phone and it came out at about 1.5 s, as expected for 3 × 500 ms.

**TC-02: All blink**
All three LEDs switch on and off together. Since they're on the same port, I'm writing them in one BSRR operation, which probably helps. Nothing visible out of sync.

**TC-03: On-board LED**
PC13 LED behaved "backwards" at first: writing 1 turned it off. This is expected, since the Blue Pill's LED is wired to 3.3 V and sinks current through the pin. Inverted the logic in code and it behaved normally.

**TC-04: Button release**
Pressing the button pulls the pin low (external pull-up, so idle = high). Holding it pauses the pattern, and releasing it resumes. Early on I saw the state change twice on a single press, which looks like contact bounce. Adding a ~20 ms debounce delay fixed it.

**TC-05: Current measurement**
Measured with a multimeter in series with each LED:

| LED | Measured current |
|-----|------------------|
| Red | ~3.9 mA |
| Yellow | ~3.6 mA |
| Green | ~3.1 mA |

This roughly matches (3.3 V − Vf) / 330 Ω, with Vf around 2.0–2.2 V. All three are fairly dim but clearly visible indoors. I could drop the resistors to ~220 Ω for more brightness and still stay well within limits.

**Anomalies:**
- Slight drop in LED brightness when all three were on at once, but nothing measurable on the 3.3 V rail.
- ST-Link connection dropped once when I bumped the jumper wires. Reseating them fixed it.

## Screenshots / Photos

- `kicad/screenshots/breadboard_setup.jpg`: full breadboard layout
- `kicad/screenshots/button_bounce.png`: scope capture of bounce on the button pin

## Lessons Learned

- The on-board PC13 LED is active-low, so check the schematic before assuming logic levels.
- Buttons need debouncing, even for a simple project. A short delay was enough here, but a timer or interrupt-based approach would be cleaner.
- Loose jumper wires on the ST-Link header cause confusing "device not found" errors, so check the physical connections first.
- Calculate LED current before wiring. 330 Ω is safe but fairly dim at 3.3 V.
- Next time, label wires on the breadboard to make debugging faster.