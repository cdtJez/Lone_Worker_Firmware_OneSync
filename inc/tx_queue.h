/**
 * @file  tx_queue.h
 * @brief Header file for tx_queue.c
 *
**/
#ifndef TX_QUEUE_H
#define TX_QUEUE_H

void tx_queue_init(void);
bool tx_queue_empty(void);
void tx_queue_insert(int message_type, uint8_t timeout);
void tx_queue_process(void);

#endif