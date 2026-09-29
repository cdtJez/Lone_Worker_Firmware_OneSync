/**
 * @file		event.h
 * @brief   Header file for the event system
 *
**/
#ifndef EVENT_H
#define EVENT_H

/* List of possible events */
#define EVENT_NONE            0 
#define EVENT_ALARM_PRESS     1
#define EVENT_SET_PRESS       2
#define EVENT_TILT            3
#define EVENT_UNTILT          4
#define EVENT_SET_LONG        5


/* Function prototypes */
void event_init(void);
uint32_t event_waiting(void);
void event_put(uint32_t event);
uint32_t event_get(void);

#endif
