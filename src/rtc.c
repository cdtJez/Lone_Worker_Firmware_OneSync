/*
 * RTC control code
 *
 *
**/
#include "main.h"


/**
 * @brief RTC interrupt handler
 *
 *  RTC wakeup timer used to wake the micro from deep sleep via this interrupt
 *
**/
void RTC_TAMP_IRQHandler(void)
{
  RTC->SCR   = RTC_SCR_CWUTF;     /* Clear the interrupt */
  RTC->MISR &= ~RTC_MISR_WUTMF;   /* Clear the wakeup timer masked flag */
}



/**
 * @brief Initialise RTC periodic auto-wake
 *
**/
void rtc_init_auto_wake(void)
{
/* Power to the RTC module */
  RCC->APBENR1 |= RCC_APBENR1_RTCAPBEN;
  
/* Enable writing to RTC registers - this only needs to happen once */  
  PWR->CR1 |= PWR_CR1_DBP;
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;

/* Turn on the RTC and select LSI clock */
	RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_1;
  
/* Turn off the wakeup feature and allow writing to the registers */
  RTC->CR &= ~RTC_CR_WUTE;   
  
/* Wait for WUTWF to be set before WUTE can be set again and WUTR can be written to */
  while (!(RTC->ICSR & RTC_ICSR_WUTWF)) {};   
  
/* Set the wake up timer to timeout 10 times/sec */
  RTC->WUTR = 200;

/* Turn on the wakeup interrupt */
  RTC->CR = (RTC->CR & ~0x7) | RTC_CR_WUTE | RTC_CR_WUTIE;
  
  NVIC_EnableIRQ(RTC_TAMP_IRQn);
}

