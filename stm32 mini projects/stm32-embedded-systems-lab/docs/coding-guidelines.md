# Coding Guidelines

These conventions apply to all firmware source code in this repository.

## 1. File Organisation

- One `.c` / `.h` pair per functional module (e.g., `adc.c` + `adc.h`).
- `main.c` contains only `main()`, system-clock setup, and top-level initialisation calls.
- Module-specific ISR handlers belong in the module's `.c` file, not in `main.c`.

## 2. Naming Conventions

| Element | Convention | Example |
|---------|-----------|---------|
| Functions | `snake_case`, prefixed by module | `gpio_led_toggle()` |
| Local variables | `snake_case` | `adc_value` |
| Global variables | `g_` prefix | `g_system_tick` |
| Constants / macros | `UPPER_SNAKE_CASE` | `LED_PIN`, `ADC_MAX_VALUE` |
| Type definitions | `PascalCase_t` | `UartConfig_t` |
| File names | `lowercase_with_underscores.c` | `pwm_control.c` |

## 3. Header Guards

Use `#ifndef` / `#define` guards with the pattern `_MODULE_NAME_H_`:

```c
#ifndef _GPIO_CONTROL_H_
#define _GPIO_CONTROL_H_

/* declarations */

#endif /* _GPIO_CONTROL_H_ */
```

## 4. Comments

- Use `/* */` block comments for file headers and function descriptions.
- Use `//` inline comments sparingly for non-obvious logic.
- Every public function must have a brief description of purpose, parameters, and return value.

```c
/**
 * @brief  Toggle the on-board LED.
 * @param  port  GPIO port handle (e.g., GPIOC).
 * @param  pin   GPIO pin number (e.g., GPIO_PIN_13).
 * @retval None
 */
void gpio_led_toggle(GPIO_TypeDef *port, uint16_t pin);
```

## 5. Formatting

- **Indentation:** 4 spaces (no tabs).
- **Braces:** Allman style (opening brace on its own line).
- **Line length:** 100 characters max.
- **Blank lines:** One blank line between functions; two blank lines between sections.

```c
void gpio_led_init(void)
{
    GPIO_InitTypeDef gpio_cfg = {0};

    gpio_cfg.Pin   = LED_PIN;
    gpio_cfg.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_cfg.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED_PORT, &gpio_cfg);
}
```

## 6. Error Handling

- Always check HAL return values (`HAL_OK`, `HAL_ERROR`, `HAL_BUSY`, `HAL_TIMEOUT`).
- Use `Error_Handler()` for unrecoverable faults.
- In debug builds, use `assert_param()` for invalid parameter detection.

## 7. ISR Discipline

- ISRs must be **short**. Set a flag or write to a buffer; process in `main()` loop.
- Use `volatile` for all variables shared between ISR and main context.
- Document the interrupt source and priority in a comment above the handler.

## 8. Magic Numbers

No magic numbers in logic. Define constants:

```c
/* BAD */
if (adc_raw > 3000) { ... }

/* GOOD */
#define ADC_THRESHOLD_HIGH  3000U
if (adc_raw > ADC_THRESHOLD_HIGH) { ... }
```

## 9. Includes

Order of `#include` directives:

1. Corresponding module header (`#include "gpio_control.h"`)
2. Project headers
3. HAL / CMSIS headers
4. Standard library headers (`<stdint.h>`, `<stdbool.h>`)

## 10. Version Control

- Commit messages: imperative mood, ≤ 72 characters.  
  Example: `Add PWM duty-cycle sweep in main loop`
- One logical change per commit.
- Do not commit generated build artefacts (see `.gitignore`).
