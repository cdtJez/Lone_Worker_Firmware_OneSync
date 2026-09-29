/*
 * @file		ht12e.c
 * @brief   HT12E encoder software simulation
 *
 *
**/
#include "main.h"

/* PB7 is the output pin - a FET inverter follows */
#define OUT_PIN   7


/**
 * @brief  Initialise the HT12E encoder system
 *
 *  Output on PB7
**/
void ht12e_init(void)
{
  RCC->IOPENR |= RCC_IOPENR_GPIOBEN;            /* Enable clock to GPIOB */
  MODIFY_REGISTER(GPIOB->MODER, 3 << (OUT_PIN << 1), 1 << (OUT_PIN << 1));
  GPIOB->BSRR = (1 << OUT_PIN);                 /* Set PB7 high - output will be low */
}


/**
 * @brief  Set the transmit line high
 *
**/
void ht12e_line_high(void)
{
  GPIOB->BRR = (1 << OUT_PIN);          /* Set PB7 low - inverter follows */
}


/**
 * @brief  Set the transmit line low
 *
**/
void ht12e_line_low(void)
{
  GPIOB->BSRR = (1 << OUT_PIN);                 /* Set PB7 high - inverter follows */
}


/**
 * @brief   Transmit an address and data packet
 *
 * @param   data  12 bit address and data
 *
 * Transmits a single address/data packet
 * Time taken = 24.333 ms
 *
 * Bits are transmitted in 1/3ms segments:
 *  0 - 1 low segment followed by 2 high segments
 *  1 - 2 low segments followed by 1 high segment
 *
 * There is a 12 bit preamble with the line low then
 * a 1/3ms high, A0-A7-AD0-AD3 follows.
 *
 * HT12E transmits each message a minimum of 4 times
 * and continuously if the transmit enable line is
 * held low.
 *
**/
void ht12e_tx_single(uint32_t data)
{
/* 12 bit long (12ms) zero preamble */
  ht12e_line_low();
  delay_us(12000);

/* 1/3 bit high to sync the receiver */
  ht12e_line_high();
  delay_us(333);
  ht12e_line_low();

/* 12 bits: A0-A7, AD0-AD3 */
  for (uint32_t bit_count = 12; bit_count; bit_count--)
  {
    delay_us(333);
    if (data & 0x800)
      ht12e_line_high ();

    delay_us(333);
    ht12e_line_high();
    delay_us(333);
    ht12e_line_low();

    data <<= 1;
  }
}


/**
 * @brief   Transmit an address and data packet 4 times
 *
 * @param   data  12 bit address and data
 *
 *  HT12D needs to see data a minimum of 4 times for a valid decode
 *
 *  Time taken = 97.3333ms
 *
 **/
void ht12e_tx(uint32_t data)
{
  ht12e_tx_single(data);
  ht12e_tx_single(data);
  ht12e_tx_single(data);
  ht12e_tx_single(data);
}
