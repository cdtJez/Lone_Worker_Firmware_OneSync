/*
 * @file		led.c
 * @brief   Activity LED functions
 *
 *
**/
#include "main.h"

uint32_t led_counter = 0;


/**
 * @brief  Initialise the LED GPIO pin
 *
 *  Blue LED on PC13, active high
**/
void led_init(void)
{
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN;          /* Enable clock to GPIOC */
  MODIFY_REG (GPIOC->MODER, (3UL << 26), (1UL << 26));  /* Set PC13 to be an output */
  GPIOC->BRR = (1 << 13);                     /* PC13 low: LED off */
}


/**
 * @brief  Turn the LED off
 *
**/
void led_off(void)
{
  GPIOC->BRR = (1 << 13);                     /* PC13 low: LED off */
  led_counter = 0;
}


/**
 * @brief  Turn the LED on
 *
 * @param count LED on time in 10ms units, a value of 0
 *        			turns the LED on until manually turned off
 *
 * Only use manually before the main loop is running.
 *
**/
void led_on(uint32_t count)
{
  led_counter = count;
  GPIOC->BSRR = (1 << 13);                    /* PC13 high: LED on */
}


/**
 * @brief  Test the LED counter and turn the LED off if zero
 *
 * Called from the main loop every 10ms
 *
**/
void led_sequencer(void)
{
  if (led_counter == 0)
    return;

  led_counter--;
  if (led_counter == 0)
    led_off ();
}
