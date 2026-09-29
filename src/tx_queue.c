/**
 * @file  tx_queue.c
 * @brief Transmit queue with timeout
 *
**/
#include "main.h"

/* Must be a power of 2 */
#define QUEUE_LEN         8
#define QUEUE_LEN_MASK    (QUEUE_LEN - 1)


/* A structure to hold tx packet information */
struct tx_packet_t {
  uint8_t timeout_count;      /* Timeout time in 1/10th seconds */
  uint8_t network_id;
  uint8_t sending_device;     /* Device message was from        */
  uint8_t message_type;       /* Message type                   */
};


/* A structure to hold packet information */
struct q_packet_t {
  int in;
  int out;
  struct tx_packet_t queue[QUEUE_LEN];
};


struct q_packet_t tx_q;
struct packet_t packet;


/**
 * @brief Initialise the tx queue
 *
 *
**/
void tx_queue_init(void)
{
  for (int i = 0; i < QUEUE_LEN; i++)
  {
    tx_q.queue[i].timeout_count = 0;
    tx_q.queue[i].network_id = 0;
    tx_q.queue[i].sending_device = 0;
    tx_q.queue[i].message_type = 0;
  }

  tx_q.in = 0;
  tx_q.out = 0;
}


/**
 * @brief Is the tx queue empty?
 *
**/
bool tx_queue_empty(void)
{
  return(tx_q.in == tx_q.out);
}


/**
 * @brief Insert a Tx packet into the queue
 *
**/
void tx_queue_insert(int message_type, uint8_t timeout)
{	
  if (tx_queue_empty() && (timeout == 0))
  {
    struct packet_t mess;    
    
    mess.network_id     = params.network_id;
    mess.sending_device = params.device_id;
    mess.message_type   = message_type;    

    rfm69hw_packet_send(&mess);
    return;
  }
  
  tx_q.queue[tx_q.in].sending_device = params.device_id;
  tx_q.queue[tx_q.in].network_id     = params.network_id;
  tx_q.queue[tx_q.in].message_type   = message_type;
  tx_q.queue[tx_q.in].timeout_count  = timeout;

  tx_q.in = (tx_q.in + 1) & QUEUE_LEN_MASK;
}



/**
 * @brief Check if any packets have timed out and send them
 *
**/
void tx_queue_process(void)
{
/* If there is nothing in the queue then return */
  if (tx_q.in == tx_q.out)
    return;

/* Decrement the timeout on the next item in the queue */
  if (tx_q.queue[tx_q.out].timeout_count > 0)
    tx_q.queue[tx_q.out].timeout_count--;

/* If the timeout is zero then send packet */
  if (tx_q.queue[tx_q.out].timeout_count == 0)
  {
    packet.network_id     = tx_q.queue[tx_q.out].network_id;
    packet.sending_device = tx_q.queue[tx_q.out].sending_device;
    packet.message_type   = tx_q.queue[tx_q.out].message_type;
    rfm69hw_packet_send(&packet);

    tx_q.out = (tx_q.out + 1) & QUEUE_LEN_MASK;
  }
}

