 /**
 * @file  message_table.c
 *
 * @brief received message table with timeout
 *
 * Used in repeater to determine if the message received is the same
 * as it has previously received that has been repeated by another 
 * repeater.
 *
**/
#include "main.h"

#define MESSAGE_MAX               16

/* Timeout in 1/10ths of a second */
#define MESSAGE_DEFAULT_TIMEOUT   100

static struct message_table_t message_table[MESSAGE_MAX];


/**
 * @brief   message_table_init
 *
 * Initialise the message table by setting the message timeouts to 0
 * This marks the message slots as unused
 *
**/
void message_table_init(void)
{
  for (int i = 0; i < MESSAGE_MAX; i++) 
    message_table[i].timeout = 0;
}


/**
 * @brief   message_table_update
 *
 * Decrement any non-zero timeouts
 *
**/
void message_table_update(void)
{
  for (int i = 0; i < MESSAGE_MAX; i++)
    if (message_table[i].timeout > 0)
      message_table[i].timeout--;
}


/**
 * @brief   message_table_insert
 *
 * Insert a new message into the first available table slot
 *
**/
void message_table_insert(uint8_t device, uint8_t message)
{
/* Scan through the message table looking for an unused slot - one with a timeout of 0 */
  int i = 0;
  while ((i < MESSAGE_MAX) && (message_table[i].timeout != 0))
    i++;

/* If i < MESSAGE_MAX then we have found an unused slot */  
  if (i < MESSAGE_MAX)
  {
    message_table[i].timeout = MESSAGE_DEFAULT_TIMEOUT;
    message_table[i].device  = device;
    message_table[i].message = message;
  }
}


/**
 * @brief   Check if a message is in the table and return the repeat count (>0) or 0
 *
 * @param   device    Device number
 * @param   message   Message type
 *
 * @return  0 if not found, repeat count (> 0) if found
 *
**/
bool message_in_table(uint8_t device, uint8_t message)
{
  for (int i = 0; i < MESSAGE_MAX; i++)
  {
    if (message_table[i].timeout > 0)
    {
      if (message_table[i].device == device && message_table[i].message == message)
        return(true);
    }
  }  
  return (false);
}

