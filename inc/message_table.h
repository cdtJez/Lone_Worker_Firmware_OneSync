/**
 * @file  message_table.h
 * @brief Header file for message_table.c
 *
**/
#ifndef MESSAGE_TABLE_H
#define MESSAGE_TABLE_H


/* Structure to hold timeout, sending device and message type */
struct message_table_t {
  uint8_t timeout;		    /* Timeout counter          */
  uint8_t device;		      /* Device message was from  */
	uint8_t message;				/* Message type             */
};


void message_table_init(void);
void message_table_update(void);
void message_table_insert(uint8_t device, uint8_t message);
bool message_in_table(uint8_t device, uint8_t message);

#endif