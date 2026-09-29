/**
 * @file  charger.c
 * @brief Charger stat line functions
 *
**/
#include "main.h"

/* STAT input is PB9 */
#define STAT_BIT         9
#define STAT_BIT_MASK   (1 << STAT_BIT)


/**
 * @brief Initialise the charger STAT line
 *
**/
void charger_init(void)
{
  MODIFY_REG(GPIOB->MODER, (3 << 18), 0);  		      /* Stat line is an input */
  MODIFY_REG(GPIOB->PUPDR, (3 << 18), (2 << 18));   /* Enable a pull down on the input to stop it floating */ 
}


/**
 * @brief Is the charger STAT line active
 *
**/
bool charger_stat(void)
{
  MODIFY_REG(GPIOB->PUPDR, (3 << 18), (1 << 18));   /* Enable the pullup on the input */
  delay_us(100);                                    /* Time to charge the input capacitance */
  int port = GPIOB->IDR;                            /* Read the port */
  MODIFY_REG(GPIOB->PUPDR, (3 << 18), (2 << 18));   /* Enable the pull down on the input to stop it floating */  
  return((port & STAT_BIT_MASK) == 0);              /* STAT is active low */
}

