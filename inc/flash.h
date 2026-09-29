/**
 *
 * @file		flash.h
 * @brief   Header file for flash.c
 *
 *
**/
#ifndef FLASH_H
#define FLASH_H

void flash_write(uint8_t *address, int size, uint8_t *data);
void flash_read(uint8_t *address, int size, uint8_t *data);

#endif