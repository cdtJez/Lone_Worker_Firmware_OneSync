/**
 *	Library for SPI
 *
 *
 *
**/
#include "main.h"

#define NSS_PIN   4         /* SPI NSS pin (PA4) */

/* Define the data register as an 8 bit register otherwise 16 bits will be transmitted */
#define SPI1_DR_8BIT (*(__IO uint8_t *) 0x4001300C)


/**
 * @brief SPI hardware initialiser
 *
 * @param none
 *
**/
void spi_init(void)
{
//  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;                /* Enable clock to GPIOA - done in main */

/* Set PA4 (NSS) to be an output, PA1 (SCK), PA6 (MISO) & PA2 (MOSI) to be alternative function */
  MODIFY_REG(GPIOA->MODER, ((3 << 8) | (3 << 2) | (3 << 12) | (3 << 4)), ((1 << 8) | (2 << 2) | (2 << 12) | (2 << 4)));   

/* Set PA1, PA6 & PA2 to be AF0 (SPI1) */
  MODIFY_REG(GPIOA->AFR[0], ((15 << 4) | (15 << 24) | (15 << 8)), 0);

/* Pull down on MISO (PA6) */  
  MODIFY_REG(GPIOA->PUPDR, (3 << 12), (2 << 12));

  GPIOA->BSRR = (1 << NSS_PIN);                     /* Set NSS (PA4) high */

  RCC->APBENR2 |= RCC_APBENR2_SPI1EN;               /* Turn on the clock to SPI1 */

  SPI1->CR1 &= ~SPI_CR1_SPE;                        /* SPI1 disabled */
  SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM  | SPI_CR1_SSI;       /* Set the clock to /2 and master mode */
  SPI1->CR2 = SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_FRXTH;     /* 8 bit mode */
  SPI1->CR1 |= SPI_CR1_SPE;                         /* Enable SPI1 */
}


/**
 * @brief SPI clear the enable line
 *
 * @param none
 *
**/
void spi_select(void)
{
  GPIOA->BRR = (1 << 4);      /* Set PA4 low */  
}


/**
 * @brief SPI set the enable line
 *
 * @param none
 *
**/
void spi_deselect(void)
{
  GPIOA->BSRR = (1 << 4);     /* Set PA4 high */
}


/**
 * @brief SPI transfer data
 *
 * @param   data to be transmitted
 * @return  data received
 *
 * Gotcha - When sending data to the data register (SPI1->DR)
 *          the register must be defined as 8bit, if the data
 *          is transferred as 32bits then 16 bits will be sent!
 *
**/
uint8_t spi_xfer(uint8_t data)
{
/* Wait for Tx data register to become empty */
  while (!(SPI1->SR & SPI_SR_TXE)) {};
  SPI1_DR_8BIT = data;

/* Wait for data to become available */
  while (!(SPI1->SR & SPI_SR_RXNE)) {}; 
  return (SPI1_DR_8BIT);
}



/**
 * @brief Disable the SPI interface before sleep.
 *
 *  Procedure as in page 876 RM0454 - stm32g0x0 reference manual
 *  If not followed then the SPI interface may send random data when in sleep mode
 *
**/
void spi_disable_before_sleep(void)
{
/* If not enabled then return */
  if (!(SPI1->CR1 & SPI_CR1_SPE))
    return;
  
/* Wait for any data transmission to finish */
  while (SPI1->SR & SPI_SR_FTLVL_Msk) {};
  
/* Wait until not busy */  
  while (SPI1->SR & SPI_SR_BSY_Msk) {};

/* Disable the SPI interface */  
  SPI1->CR1 &= ~(SPI_CR1_SPE); 
  
/* Read data until the buffer is empty */
  while (SPI1->SR & SPI_SR_FRLVL_Msk)
    (void)(SPI1_DR_8BIT);
}



/**
 * @brief Enable the SPI interface after sleep
 *
**/
void spi_enable_after_sleep(void)
{
  SPI1->CR1 |= SPI_CR1_SPE;  
}

