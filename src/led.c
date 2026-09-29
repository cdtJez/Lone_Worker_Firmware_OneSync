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
 *  LED on PA1
**/
void led_init(void)
{
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;          /* Enable clock to GPIOA */
  MODIFY_REGISTER (GPIOA->MODER, 0x000C, 0x0004);  /* Set PA1 to be an output */
  GPIOA->BSRR = (1 << 1);                     /* Set PA1 high */
}


/**
 * @brief  Turn the LED off
 *
**/
void led_off(void)
{
  GPIOA->BSRR = (1 << 1);                     /* Set PA1 high */
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
  GPIOA->BRR = (1 << 1);                    /* Set PA1 low */
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
