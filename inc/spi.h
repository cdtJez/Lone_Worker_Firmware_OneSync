/**
 *
 * @file    spi.h
 * @brief   Header file for spi.c
 *
 *
**/
#ifndef SPI_H
#define SPI_H

void spi_init(void);
void spi_select(void);
void spi_deselect(void);
uint8_t spi_xfer(uint8_t data);
void spi_disable_before_sleep(void);
void spi_enable_after_sleep(void);

#endif