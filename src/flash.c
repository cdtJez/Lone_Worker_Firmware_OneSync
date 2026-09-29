/**
 * @file  flash.c
 * @brief Read and write to flash
 *
**/
#include "main.h"

#define FLASH_UNLOCK_KEY_1  0x45670123
#define FLASH_UNLOCK_KEY_2  0xCDEF89AB

#define FLASH_SR_ERROR_FLAGS  (FLASH_SR_FASTERR | FLASH_SR_MISERR | FLASH_SR_PGSERR | \
                               FLASH_SR_SIZERR  | FLASH_SR_PGAERR | FLASH_SR_WRPERR | \
                               FLASH_SR_PROGERR | FLASH_SR_OPERR  | FLASH_SR_EOP)

/**
 * @brief Erase a page of flash then write data
 *
 * @param address   address in flash
 * @param size      number of bytes to write
 * @param data      pointer to data to write
 *
 * @return none
 *
**/
void flash_write(uint8_t *address, int size, uint8_t *data)
{
  /* Unlock flash */
  FLASH->KEYR = FLASH_UNLOCK_KEY_1;
  FLASH->KEYR = FLASH_UNLOCK_KEY_2;

/* Erase top flash page */

  /* Wait for any pending flash operations */
  while (FLASH->SR & FLASH_SR_BSY1) {};

  /* Clear any error flags */
  FLASH->SR |= FLASH_SR_ERROR_FLAGS;

  /* Set the page erase mode and page */
  MODIFY_REG(FLASH->CR, 0x1ff8, (15 << 3) | FLASH_CR_PER);

  /* Start the erase */
  FLASH->CR |= FLASH_CR_STRT;

  /* Wait for busy flag to clear */
  while (FLASH->SR & FLASH_SR_BSY1) {};
	
	/* Clear page erase mode */
	FLASH->CR &= ~FLASH_CR_PER;	

/* Write data into flash */

  /* Clear any error flags */
  FLASH->SR |= FLASH_SR_ERROR_FLAGS;

  /* Set the flash programming enable bit */
  FLASH->CR |= FLASH_CR_PG;
	
  uint32_t * address_ptr = (uint32_t *)address;
  uint32_t * data_ptr = (uint32_t *)data;

  /* Write loop */
  while (size > 0)
  {
	/* Must write 2 32bit values (8 bytes) at a time */	
    *address_ptr++ = *data_ptr++;
    *address_ptr++ = *data_ptr++;

	/* Wait for the programming to finish */
    while (FLASH->SR & FLASH_SR_BSY1) {};

  /* Check the programming succeeded flag */
    if ((FLASH->SR & FLASH_SR_EOP) != 0)
      break;

  /* Clear the programming succeeded flag */
    FLASH->SR |= FLASH_SR_EOP;

	/* 8 bytes written */
    size -= 8;
  }

  /* Clear the programming enable bit */
  FLASH->CR &= ~FLASH_CR_PG;

  /* Lock the flash */
  FLASH->CR |= FLASH_CR_LOCK;
}


/**
 * @brief Read data from flash
 *
 * @param address   address in flash
 * @param size      number of bytes to write
 * @param data      pointer where to write data to
 *
 * @return none
 *
**/
void flash_read(uint8_t *address, int size, uint8_t *data)
{
  while (size--)
    *data++ = *address++;
}

