/** 
 * Delay in SYSCLK periods.
 *
 * @param	count	SysClk ticks to delay
 * @return none
 *
 * At 2MHz 1 SYSCLK period is 500ns.
 *
 * A call to delay_sysclk takes about 25-35 SYSCLK longer than requested (or more
 * if code memory access has wait states and caching/prefetch is disabled).
 *
 * Delay is not extended if interrupts occur, unless they take more than
 * SysTick->LOAD SYSCLK periods to process.
 *
 * Can be called from interrupt handlers.
**/
#include "main.h"


/**
 * @brief Delay function (blocking)
 *
 * @param count Ticks to count, max (2^32 - 1)
 *
**/
void delay_ticks(uint32_t count)
{
  int then = SysTick->VAL;

  for (;;)
  {
    int now     = SysTick->VAL;
    int elapsed = (then - now);

    if (elapsed < 0) /* count-down clock has wrapped */
      elapsed += (SysTick->LOAD + 1);

    if (count < (uint32_t)elapsed)
      return;

    count -= (uint32_t)elapsed;
    then = now;
  }
}


/**
 * @brief Delay function (blocking) not using SysTick
 *
 * @param us    delay time (us)
 *
 *  Can delay 1073 seconds (2^32 / 4000000)
 *  Loop takes 4 cycles = 1/4 us with a 16MHz clock
 *  Delay is extended by interrupts
 * 
**/
void delay_us(uint32_t us)
{
  asm volatile (".syntax unified\n\t"
                ".cpu cortex-m0plus\n\t"
                ".thumb\n\t"
                "lsls %1, %1, #2\n\t"
                "1:\n\t"
                "nop\n\t"
                "subs %1, %1, #1\n\t"
                "bne  1b\n\t"
                : "=r" (us)
                : "0" (us)
                : );
}

