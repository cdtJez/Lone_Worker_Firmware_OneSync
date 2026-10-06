/**
 *
 * @file		flash.h
 * @brief   Header file for flash.c
 *
 *
**/
#ifndef FLASH_H
#define FLASH_H

#include <stdbool.h>
#include <stdint.h>
#include "stm32g0xx.h"

#define FLASH_PROGRAM_ERROR_FLAGS (FLASH_SR_OPERR | FLASH_SR_PROGERR | \
    FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR | \
    FLASH_SR_MISERR | FLASH_SR_FASTERR)
#define FLASH_OPTION_ERROR_FLAGS (FLASH_PROGRAM_ERROR_FLAGS | \
    FLASH_SR_RDERR | FLASH_SR_OPTVERR)

/* Low-level helpers: caller must serialize FLASH access. The wait uses a
 * bounded polling budget for the 16 MHz clock, without requiring SysTick.
 * On timeout, do not change FLASH control registers while still busy.
 */
bool flash_wait_idle(void);
bool flash_unlock(void);
void flash_lock(void);

void flash_write(uint8_t *address, int size, uint8_t *data);
void flash_read(uint8_t *address, int size, uint8_t *data);

#endif