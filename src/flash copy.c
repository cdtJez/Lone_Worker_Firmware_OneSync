
/**
 * @file  flash.c
 * @brief Read and write to flash
 *
**/
#include "main.h"

#define FLASH_UNLOCK_KEY_1  0x45670123
#define FLASH_UNLOCK_KEY_2  0xCDEF89AB

#define FLASH_WAIT_LIMIT 1600000u


bool flash_wait_idle(void)
{
  uint32_t remaining = FLASH_WAIT_LIMIT;
  while (FLASH->SR & (FLASH_SR_BSY1 | FLASH_SR_CFGBSY))
  {
    if (--remaining == 0u)
      return false;
  }
  return true;
}

bool flash_unlock(void)
{
  if (FLASH->CR & FLASH_CR_LOCK)
  {
    FLASH->KEYR = FLASH_UNLOCK_KEY_1;
    FLASH->KEYR = FLASH_UNLOCK_KEY_2;
  }
  return (FLASH->CR & FLASH_CR_LOCK) == 0u;
}

void flash_lock(void)
{
  FLASH->CR |= FLASH_CR_OPTLOCK | FLASH_CR_LOCK;
}

/* Decode the captured status without reading or modifying FLASH registers. */
static void flash_log_program_error(uint32_t sr)
{
  xprintf("flash_write: programming error SR=0x%08x", (unsigned)sr);
  if (sr & FLASH_SR_OPERR)  xprintf(" OPERR");
  if (sr & FLASH_SR_WRPERR)  xprintf(" WRPERR");
  if (sr & FLASH_SR_PGSERR)  xprintf(" PGSERR");
  if (sr & FLASH_SR_PROGERR) xprintf(" PROGERR");
  if (sr & FLASH_SR_PGAERR)  xprintf(" PGAERR");
  if (sr & FLASH_SR_SIZERR)  xprintf(" SIZERR");
  if (sr & FLASH_SR_MISERR)  xprintf(" MISERR");
  if (sr & FLASH_SR_FASTERR) xprintf(" FASTERR");
  if (sr & FLASH_SR_EOP)     xprintf(" EOP");
  xprintf("\n");
}

/**
 * @brief Erase a page of flash then write data
 *
 * @param address   address in flash
 * @param size      number of bytes to write
 * @param data      pointer to data to write
 *
 * @return none
 */
void flash_write(uint8_t *address, int size, uint8_t *data)
{
#define FLASH_BASE_ADDR 0x08000000UL
#define FLASH_PAGE_SIZE 0x800 /* 2KB */

  uint32_t addr = (uint32_t)address;
  if (addr < FLASH_BASE_ADDR)
  {
    xprintf("flash_write: invalid address %08x\n", addr);
    return;
  }

  uint32_t page_base = addr & ~(FLASH_PAGE_SIZE - 1);
  uint32_t page_index = (page_base - FLASH_BASE_ADDR) / FLASH_PAGE_SIZE;
  uint32_t offset = addr - page_base;

  if (offset + (uint32_t)size > FLASH_PAGE_SIZE)
  {
    xprintf("flash_write: span across pages not supported\n");
    return;
  }

  static uint8_t page_buf[FLASH_PAGE_SIZE];

  /* Read existing page into RAM */
  flash_read((uint8_t *)page_base, FLASH_PAGE_SIZE, page_buf);

  /* Update buffer with new data */
  for (int i = 0; i < size; i++)
    page_buf[offset + i] = data[i];

  /* Wait before unlocking; do not issue keys to an already unlocked FLASH. */
  if (!flash_wait_idle() || !flash_unlock())
  {
    xprintf("flash_write: flash busy or unlock failed\n");
    return;
  }
  FLASH->SR = FLASH_PROGRAM_ERROR_FLAGS | FLASH_SR_EOP;

  /* Erase the page */
  MODIFY_REG(FLASH->CR, 0x1ff8, (page_index << 3) | FLASH_CR_PER);
  FLASH->CR |= FLASH_CR_STRT;
  if (!flash_wait_idle())
  {
    xprintf("flash_write: erase timeout\n");
    return;
  }
  FLASH->CR &= ~FLASH_CR_PER;
  if (FLASH->SR & FLASH_PROGRAM_ERROR_FLAGS)
  {
    xprintf("flash_write: erase error SR=0x%08x\n", (unsigned)FLASH->SR);
    FLASH->SR = FLASH_PROGRAM_ERROR_FLAGS | FLASH_SR_EOP;
    flash_lock();
    return;
  }

  /* Verify the page was erased (read flash memory directly) */
  {
    int erased = 1;
    uint8_t *p = (uint8_t *)page_base;
    for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++)
    {
      if (p[i] != 0xFF)
      {
        erased = 0;
        break;
      }
    }
    xprintf("flash_write: erase verify page=%08x -> %s\n", (unsigned)page_base, erased ? "OK" : "FAIL");
  }

  /* Program the page in aligned 8-byte double-word writes */
  FLASH->SR = FLASH_PROGRAM_ERROR_FLAGS | FLASH_SR_EOP;

  for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i += 8)

  {
    uint64_t dw = (uint64_t)page_buf[i] |
                  ((uint64_t)page_buf[i+1] << 8) |
                  ((uint64_t)page_buf[i+2] << 16) |
                  ((uint64_t)page_buf[i+3] << 24) |
                  ((uint64_t)page_buf[i+4] << 32) |
                  ((uint64_t)page_buf[i+5] << 40) |
                  ((uint64_t)page_buf[i+6] << 48) |
                  ((uint64_t)page_buf[i+7] << 56);

    volatile uint64_t *ptr64 = (volatile uint64_t *)(page_base + i);

    /* Set PG, perform a single 64-bit write, then wait */
    FLASH->CR |= FLASH_CR_PG;
    *ptr64 = dw;

    if (!flash_wait_idle())
    {
      xprintf("flash_write: programming timeout\n");
      return;
    }

    uint32_t sr = FLASH->SR;
    if (sr & FLASH_PROGRAM_ERROR_FLAGS)
    {
      FLASH->SR = FLASH_PROGRAM_ERROR_FLAGS | FLASH_SR_EOP;
      flash_log_program_error(sr);
      FLASH->CR &= ~FLASH_CR_PG;
      break;
    }

    if (sr & FLASH_SR_EOP)
      FLASH->SR = FLASH_SR_EOP;

    FLASH->CR &= ~FLASH_CR_PG;
  }

  flash_lock();
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

