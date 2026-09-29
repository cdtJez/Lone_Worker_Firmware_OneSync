/**
 *
 * @file		delay.h
 * @brief   Header file for delay.c
 *
 *
**/
#ifndef DELAY_H
#define DELAY_H

/* starts the SysTick counting but with the interrupt off */
void SysTick_on(void);

/* like the CMSIS function, but without off-by-one bug and doesn't mess with interrupt priorities */
void SysTick_init(uint32_t ticks);

/* Delay in SYSCLK periods.
 *
 * At 8MHz 1 SYSCLK period is 125ns.
 *
 * A call to delay_sysclk takes about 25-35 SYSCLK longer than requested (or more
 * if code memory access has wait states and caching/prefetch is disabled).
 *
 * Delay is not extended if interrupts occur, unless they take more than
 * SysTick->LOAD SYSCLK periods to process.
 *
 * Can be called from interrupt handlers.
 */
void delay_ticks(uint32_t count);

/* Delay not using SysTick */
void delay_us(uint32_t us);

/* Subtract 20 from the parameter to ensure provided delay is at least that
 * requested.
 */
#define DELAY_SYSCLK(cnt)   delay_ticks(((cnt) >= 20u) ? ((cnt) - 20u) : 0u)

/* Use these macros when the parameter is a literal only, because there is
 * no instruction for long long division, it requires a library.
 *
 * The conversion from time units to counts will occur at compile time, and
 * the compiler will warn if overflow occurs (delay more than 2**32 SYSCLK).
 */
#define DELAY_100_NS(t)     (DELAY_SYSCLK((((unsigned long long)SYSCLK * (unsigned long long)(t))  + 9999999uLL) / 10000000uLL))
#define DELAY_US(us)        (DELAY_SYSCLK((((unsigned long long)SYSCLK * (unsigned long long)(us)) +  999999uLL) /  1000000uLL))
#define DELAY_MS(ms)        (DELAY_SYSCLK((((unsigned long long)SYSCLK * (unsigned long long)(ms)) +     999uLL) /     1000uLL))
#define DELAY_S(s)          (DELAY_SYSCLK(  (unsigned long long)SYSCLK * (unsigned long long)(s)))

#endif /* DELAY_H_INCLUDED */