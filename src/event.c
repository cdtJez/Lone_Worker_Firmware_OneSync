/**
 * @file	event.c
 * @brief   Event handler system
 *
**/
#include "main.h"

/* EVENT_Q_SIZE must be a power of 2 */
#define EVENT_Q_SIZE    16
#define EVENT_Q_MASK    (EVENT_Q_SIZE - 1)

/* Structure to hold a queue with input and output pointers */
struct event_struct {
	uint32_t in;
  uint32_t out;
  uint8_t q[EVENT_Q_SIZE];
};

struct event_struct ev;


/**
 * @brief Initialise the event system
 *
 * @param  none
 * @return none
 *
**/
void event_init(void)
{
  ev.in = 0;
  ev.out = 0;
}


/**
 * @brief Test if there is an event waiting
 *
 * @param  none
 * @return True if an event waiting
 *
**/
uint32_t event_waiting(void)
{
  return(ev.in != ev.out);
}


/**
 * @brief Put an event into the event queue
 *
 * @param  event 
 * @return none
 *
**/
void event_put(uint32_t event)
{
  ev.q[ev.in++ & EVENT_Q_MASK] = event;
}


/**
 * @brief Get an event from the event queue
 *
 * @param  none
 * @return event
 *
**/
uint32_t event_get(void)
{
  if (ev.in == ev.out)
    return(EVENT_NONE);
  else
    return(ev.q[ev.out++ & EVENT_Q_MASK]);
}
