# STM32F401RE-Bare-Metal-Drivers
# STM32F401RE Bare Metal Peripheral Drivers

Register-level peripheral drivers written in C for STM32F401RE 
without HAL library. All configurations done using RM0368 
reference manual.

## Peripherals Covered
- GPIO — LED blink, button input, BSRR atomic access
- Interrupts — EXTI button interrupt, flag pattern, NVIC config
- Timers + PWM — PSC/ARR calculation, timer ISR, LED dimming
- UART — Polling transmit, interrupt-driven receive

## Hardware
- STM32 Nucleo F401RE
- Reference Manual: RM0368

## Tools
- STM32CubeIDE
- No HAL library used
