/*
 * @file		button.c
 * @brief   Button handler
 *
**/
#include "main.h"

#define INPUT_DEBOUNCE_TIME          2	 /* Button debounced      - 300ms */
#define INPUT_LONG_DEBOUNCE_TIME    10   /* Long press time       - 1s    */
#define INPUT_SET_OFF_DEBOUNCE_TIME 90   /* Time to turn unit off - 9s    */

int32_t input_set_count;                 /* Setting debounce counter      */
int32_t input_alarm_count;               /* Alarm debounce counter        */

#define INPUT_SETTING_BIT          0     /* Input setting = PF0           */
#define INPUT_SETTING_MASK       (1 << INPUT_SETTING_BIT)
#define INPUT_ALARM_BIT            1     /* Input alarm = PF1             */
#define INPUT_ALARM_MASK         (1 << INPUT_ALARM_BIT)

/* Set pullups, pull downs or no pull on PF0 & PF1 */
#define INPUTS_PULLUP     (MODIFY_REG(GPIOF->PUPDR, (3 << 2) | (3 << 0), (1 << 2) | (1 << 0)))
#define INPUTS_PULLDOWN   (MODIFY_REG(GPIOF->PUPDR, (3 << 2) | (3 << 0), (2 << 2) | (2 << 0)))
#define INPUTS_NO_PULL    (GPIOF->PUPDR &= ~(3 << 2 | 3 << 0))


/**
 * @brief Initialise the button system
 *
 * @param  none
 * @return none
 *
 * Buttons on PF0 (SET) and PF1 (ACT)
 *
**/
void input_init(void)
{
  GPIOF->MODER &= ~((3 << 0) | (3 << 2));     /* Make PF0 & PF1 inputs */
  INPUTS_PULLDOWN;                              /* Stop the inputs from floating */

  input_set_count  = 0;
  input_alarm_count  = 0;
}


/**
 * @brief Scan the inputs and generate events
 *        Called every 100ms.
 *
 * @param  none
 * @return none
 *
**/
void input_scan(void)
{
  INPUTS_PULLUP;                                      /* Pullup resistors 55K worst case */
  delay_us(10);                                        /* Short delay to allow input capacitance (5pF) to charge */
  int port_read = GPIOF->IDR;                         /* Read inputs */ 
  INPUTS_PULLDOWN;                                    /* Make sure inputs are not floating */
  
/* Setting input events */
  if (port_read & INPUT_SETTING_MASK)                 /* Input high = inactive */
    input_set_count = 0;
  else
  {
    if (input_set_count == INPUT_LONG_DEBOUNCE_TIME)
      event_put(EVENT_SET_PRESS);
    if (input_set_count == INPUT_SET_OFF_DEBOUNCE_TIME)
      event_put(EVENT_SET_LONG);

    input_set_count++;                                 /* Don't worry about rolling over as it takes 13.5 years */ 
  }

/* Alarm input events */
  if (port_read & INPUT_ALARM_MASK)                    /* Input high = inactive */
    input_alarm_count = 0;
  else
  {
    if (input_alarm_count == INPUT_DEBOUNCE_TIME)
      event_put(EVENT_ALARM_PRESS);

    input_alarm_count++;
  }
}
