/*
 * @file		machine_state.c
 * @brief   Machine state functions
 *
**/
#include "main.h"


/* Possible events defined in events.h:

#define EVENT_NONE            0
#define EVENT_ALARM_PRESS     1
#define EVENT_SET_PRESS       2
#define EVENT_TILT            3
#define EVENT_UNTILT          4
#define EVENT_SET_LONG        5
*/

#define STATE_IDLE                0
#define STATE_ALARM               1
#define STATE_TILT_HOLDOFF        2
#define STATE_TILT_DEACTIVATE     3
#define STATE_TILT_ALARM          4
#define STATE_OFF                 5
#define STATE_CHECK_IN            6

#define NUMBER_OF_STATES		      7
#define NUMBER_OF_EVENTS					6

/* 15 minute timeout */
#define TILT_DEACTIVATE_TIME	    (TICKS_IN_1_MINUTE * 15)


/* Function prototypes */
void no_fn(void)	{}
void enter_idle_state(void);
void enter_alarm_state(void);
void enter_tilt_holdoff_state(void);
void enter_tilt_deactivate_state(void);
void enter_off_state(void);
void default_tilt_holdoff(void);
void default_tilt_deactivate(void);
void pre_idle(void);
void pre_tilt_holdoff(void);
void pre_tilt_deactivate(void);
void pre_alarm(void);
void pre_tilt_alarm(void);
void pre_off(void);
void pre_check_in(void);


/* Event table definition */
void (*state_table[NUMBER_OF_STATES][NUMBER_OF_EVENTS])(void) =
{
  {	/* State idle                                     */
		no_fn,				               /* Event NONE        */
		enter_alarm_state,			     /* Event ALARM_PRESS */
		enter_tilt_deactivate_state, /* Event SET_PRESS   */
		enter_tilt_holdoff_state,    /* Event TILT        */
		no_fn,				               /* Event UNTILT      */
    no_fn                        /* Event SET_LONG    */
	},
  {	/* State alarm                                    */
		no_fn,				               /* Event NONE        */
		enter_alarm_state,				   /* Event ALARM_PRESS */
		enter_idle_state,				     /* Event SET_PRESS   */
		no_fn,				               /* Event TILT        */
		no_fn,				               /* Event UNTILT      */
    no_fn                        /* Event SET_LONG    */
	},
  {	/* State tilt_holdoff                             */
		default_tilt_holdoff,				 /* Event NONE        */
		enter_alarm_state,				   /* Event ALARM_PRESS */
		default_tilt_holdoff,				 /* Event SET_PRESS   */
		default_tilt_holdoff,				 /* Event TILT        */
		enter_idle_state,				     /* Event UNTILT      */
    no_fn                        /* Event SET_LONG    */
	},				
  {	/* State tilt_deactivate                          */
		default_tilt_deactivate,		 /* Event NONE        */
		enter_alarm_state,           /* Event ALARM_PRESS */
		enter_idle_state,	           /* Event SET_PRESS   */
		default_tilt_deactivate,		 /* Event TILT        */
		default_tilt_deactivate,		 /* Event UNTILT      */
    enter_off_state              /* Event SET_LONG    */
	},		
  {	/* State tilt_alarm                               */
		no_fn,				               /* Event NONE        */
		enter_alarm_state,				   /* Event ALARM_PRESS */
		enter_idle_state,				     /* Event SET_PRESS   */
		no_fn,				               /* Event TILT        */
		no_fn,				               /* Event UNTILT      */
    no_fn                        /* Event SET_LONG    */
	},
  { /* State off                                      */
		no_fn,				               /* Event NONE        */
		no_fn,				               /* Event ALARM_PRESS */
		no_fn,          				     /* Event SET_PRESS   */
		no_fn,				               /* Event TILT        */
		no_fn,				               /* Event UNTILT      */
    enter_idle_state             /* Event SET_LONG    */    
  },
  { /* State check in                                 */
		no_fn,				               /* Event NONE        */
		enter_alarm_state,	         /* Event ALARM_PRESS */
		enter_idle_state,				     /* Event SET_PRESS   */
		default_tilt_holdoff,        /* Event TILT        */
		no_fn,				               /* Event UNTILT      */
    no_fn                        /* Event SET_LONG    */    
  }
};



/* Function to call every time a state is entered */
void (*pre_state_table[NUMBER_OF_STATES])(void) =
{
  pre_idle,               /* STATE_IDLE               */
  pre_alarm,              /* STATE_ALARM              */
  pre_tilt_holdoff,       /* STATE_TILT_HOLDOFF       */
  pre_tilt_deactivate,    /* STATE_TILT_DEACTIVATE    */
  pre_tilt_alarm,         /* STATE_TILT_ALARM         */
  pre_off,                /* STATE_OFF                */
  pre_check_in            /* STATE_CHECK_IN           */
};



/* Machine state structure */
struct machine_state_t {
  int state;              /* Machine state            */
  int alarm_time;		      /* Time in alarm for resend */
  int tilt_time;          /* Tilt count up timer      */
  int tilt_deactivate;    /* Tilt de-activate counter */
};

struct machine_state_t machine;



/**
 * @brief Initialise the state machine
 *
**/
void machine_state_initialise(void)
{
  machine.state           = STATE_IDLE;
  machine.alarm_time		  = 0;
  machine.tilt_time       = 0;
  machine.tilt_deactivate = 0;
}



/**
 * @brief Enter the idle state
 *
**/
void enter_idle_state(void)
{
  machine.state = STATE_IDLE;
  sequencer_setup(2, sequence_flash_green_fast);  	
}



/**
 * @brief Enter the alarm state
 *
**/
void enter_alarm_state(void)
{
  message_send(params.in_1_act_msg);
  sequencer_setup(100000, sequence_flash_red);
  machine.alarm_time = 0;
  machine.state = STATE_ALARM;	
}



/**
 * @brief Enter the tilt holdoff state
 *
**/
void enter_tilt_holdoff_state(void)
{
  machine.state = STATE_TILT_HOLDOFF;
  machine.tilt_time = 0;
  sequencer_setup(100, sequence_flash_cyan);
}


/**
 * @brief Enter the tilt deactivate state
 *
**/
void enter_tilt_deactivate_state(void)
{
  sequencer_setup(10000, sequence_flash_yellow);
  machine.state = STATE_TILT_DEACTIVATE;
  machine.tilt_deactivate = 0;	
}



/**
 * @brief Enter the tilt alarm state
 *
**/
void enter_tilt_alarm_state(void)
{
  message_send(Msg_TiltActivated);
  sequencer_setup(100000, sequence_flash_red);
  machine.tilt_time = 0;
  machine.state = STATE_TILT_ALARM;	
}


/**
 * @brief Enter the off state
 *
**/
void enter_off_state(void)
{
  sequencer_setup(3, sequence_colours);
  machine.state = STATE_OFF;	  
}



/**
 * @brief Enter the check in state
 *
**/
void enter_check_in_state(void)
{
  sequencer_setup(3, sequence_colours);
  machine.state = STATE_CHECK_IN;	  
}



/**
 * @brief Default function for tilt_alarm
 *
**/
void default_tilt_holdoff(void)
{
  if (machine.tilt_time >= (params.tilt_timeout * 10))
	  enter_tilt_alarm_state();	
}



/**
 * @brief Default function for tilt_deactivate 
 *
**/
void default_tilt_deactivate(void)
{
  if (machine.tilt_deactivate >= TILT_DEACTIVATE_TIME)
	  enter_idle_state();	
}



/* Pre functions should not change state as they might miss an event */

/**
 * @brief Function executed every time when we are in state idle
 *
**/
void pre_idle(void)
{
	
}



/**
 * @brief Function executed every time when we are in state tilt_holdoff
 *
**/
void pre_tilt_holdoff(void)
{
	machine.tilt_time++;
}



/**
 * @brief Function executed every time when we are in state tilt_deactivate
 *
**/
void pre_tilt_deactivate(void)
{
	machine.tilt_deactivate++;
}



/**
 * @brief Function executed every time when we are in state alarm
 *
**/
void pre_alarm(void)
{
  if (params.active_resend_time == 0)
    return;  
  
	/* Alarm resend */
	machine.alarm_time++;
	if (machine.alarm_time >= (params.active_resend_time * 10))
	{
	  message_send(params.in_1_act_msg);
	  machine.alarm_time = 0;
	}	
}



/**
 * @brief Function executed every time when we are in state tilt_alarm
 *
**/
void pre_tilt_alarm(void)
{
  if (params.active_resend_time == 0)
    return;
  
  /* Tilt alarm resend */
	machine.tilt_time++;
	if (machine.tilt_time >= (params.active_resend_time * 10))
	{
	  message_send(Msg_TiltActivated);
	  machine.tilt_time = 0;
	}	
}



/**
 * @brief Function executed every time when we are in state off
 *
**/
void pre_off(void)
{
	
}



/**
 * @brief Function executed every time when we are in state check in
 *
**/
void pre_check_in(void)
{
  
}



/**
 * @brief Process any events - gets called every 100ms
 *
**/
void machine_state_process(void)
{
/* Function pointer gets called every 1/10th of a second depending on the state */ 
  (*(pre_state_table[machine.state]))();
  
/* Function pointer for a state and an event */
  (*(state_table[machine.state][event_get()]))();
}



/**
 * @brief Get the current machine state
 *
**/
bool machine_state_is_off(void)
{
  return(machine.state == STATE_OFF);
}

