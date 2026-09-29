/**
 * @file    usart.h
 * @brief   Header file for usart.c
 *
**/
#ifndef USART_H
#define USART_H

void usart2_init(void);
void usart2_sen_init(void);
void usart2_deinit(void);
void usart2_tx_char(int data);
int  usart2_rx_char(void);
int  usart2_rx_waiting(void);
void usart2_puts(char *s);
int  usart2_serial_active(void);
bool usart2_txfifo_empty(void);

#endif
