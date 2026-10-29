/*
 * interrupt.c
 * EXTI button handling and a small event queue between the ISRs and main.
 */

#include "interrupt.h"
#include "stm32f1xx_hal.h"

#define EVENT_QUEUE_CAPACITY    8U
#define DEBOUNCE_THRESHOLD_MS   50U

static volatile system_event_t event_queue[EVENT_QUEUE_CAPACITY];
static volatile uint8_t  queue_head;
static volatile uint8_t  queue_tail;
static volatile uint32_t last_btn_press_tick;


void interrupt_gpio_init(void)
{
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Buttons: falling-edge interrupt with the internal pull-up */
    gpio.Pin  = BTN_TRIGGER_PIN | BTN_EMERGENCY_PIN;
    gpio.Mode = GPIO_MODE_IT_FALLING;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &gpio);

    /* Set the LED levels before the pins become outputs, so nothing
       flashes on startup. PC13 is active-low, so high means off. */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

    gpio.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &gpio);

    /* A lower number is a higher priority, so the emergency button
       (priority 1) can interrupt the trigger button's handler (2). */
    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}


/* Called from both ISRs. If the queue is full the event is dropped. */
void interrupt_post_event(system_event_t evt)
{
    /* The emergency ISR can preempt the trigger ISR in the middle of
       this function, so the update has to be atomic. */
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    uint8_t next_head = (uint8_t)((queue_head + 1U) % EVENT_QUEUE_CAPACITY);

    if (next_head != queue_tail)
    {
        event_queue[queue_head] = evt;
        queue_head = next_head;
    }

    __set_PRIMASK(primask);
}


bool interrupt_has_pending_event(void)
{
    return (queue_head != queue_tail);
}


system_event_t interrupt_get_next_event(void)
{
    if (queue_head == queue_tail)
    {
        return EVENT_NONE;
    }

    system_event_t evt = event_queue[queue_tail];
    queue_tail = (uint8_t)((queue_tail + 1U) % EVENT_QUEUE_CAPACITY);
    return evt;
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t now = HAL_GetTick();

    if (GPIO_Pin == BTN_TRIGGER_PIN)
    {
        if ((now - last_btn_press_tick) > DEBOUNCE_THRESHOLD_MS)
        {
            last_btn_press_tick = now;
            interrupt_post_event(EVENT_BUTTON_PRESSED);
        }
    }
    else if (GPIO_Pin == BTN_EMERGENCY_PIN)
    {
        /* No debounce here, so the stop happens on the first edge */
        interrupt_post_event(EVENT_EMERGENCY_TRIGGERED);
    }
}