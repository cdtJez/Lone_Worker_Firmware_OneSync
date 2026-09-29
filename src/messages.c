/**
 * @file  message.c
 * @brief Lookup message names or numbers
 *
**/
#include "main.h"


/* Structure to hold message numbers and strings */
struct message_struct {
  uint8_t msg_number;
	char *msg_str;
};



/* Message number and string table */
static struct message_struct messages[] = {
  { Msg_Null,                 "null"              },
  { Msg_Activate_1,           "activate1"         },
  { Msg_Deactivate_1,         "deactivate1"       },
  { Msg_Activate_2,           "activate2"         },
  { Msg_Deactivate_2,         "deactivate2"       },
  { Msg_Activate_3,           "activate3"         },
  { Msg_Deactivate_3,         "deactivate3"       },
  { Msg_Activate_4,           "activate4"         },
  { Msg_Deactivate_4,         "deactivate4"       },
  { Msg_Activate_5,           "activate5"         },
  { Msg_Deactivate_5,         "deactivate5"       },
  { Msg_Activate_6,           "activate6"         },
  { Msg_Deactivate_6,         "deactivate6"       },
  { Msg_Activate_7,           "activate7"         },
  { Msg_Deactivate_7,         "deactivate7"       },
  { Msg_Activate_8,           "activate8"         },
  { Msg_Deactivate_8,         "deactivate8"       },
  { Msg_TiltActivated,        "tiltactivated"     },
  { Msg_TiltDeactivated,      "tiltdeactivated"   },
  { Msg_TiltFalseAlarm,       "tiltfalsealarm"    },
  { Msg_StillAlive,           "stillalive"        },
  { Msg_PowerLoss,            "powerloss"         },
  { Msg_PowerReturn,          "powerreturn"       },
  { Msg_BatteryOK,            "batteryok"         },
  { Msg_BatteryLow,           "batterylow"        },
  { Msg_BatteryCritical,      "batterycritical"   },
  { Msg_SounderActivate,      "sounderactivate"   },
  { Msg_SounderReset,         "sounderreset"      },
  { Msg_SounderActivate00,    "sounderactivate00" },
  { Msg_SounderReset00,       "sounderreset00"    },
  { Msg_SounderActivate01,    "sounderactivate01" },
  { Msg_SounderReset01,       "sounderreset01"    },
  { Msg_SounderActivate02,    "sounderactivate02" },
  { Msg_SounderReset02,       "sounderreset02"    },
  { Msg_SounderActivate03,    "sounderactivate03" },
  { Msg_SounderReset03,       "sounderreset03"    },
  { Msg_SounderActivate04,    "sounderactivate04" },
  { Msg_SounderReset04,       "sounderreset04"    },
  { Msg_SounderActivate05,    "sounderactivate05" },
  { Msg_SounderReset05,       "sounderreset05"    },
  { Msg_SounderActivate06,    "sounderactivate06" },
  { Msg_SounderReset06,       "sounderreset06"    },
  { Msg_SounderActivate07,    "sounderactivate07" },
  { Msg_SounderReset07,       "sounderreset07"    },
  { Msg_SounderActivate08,    "sounderactivate08" },
  { Msg_SounderReset08,       "sounderreset08"    },
  { Msg_SounderActivate09,    "sounderactivate09" },
  { Msg_SounderReset09,       "sounderreset09"    },
  { Msg_SounderActivate10,    "sounderactivate10" },
  { Msg_SounderReset10,       "sounderreset10"    },
  { Msg_SounderActivate11,    "sounderactivate11" },
  { Msg_SounderReset11,       "sounderreset11"    },
  { Msg_SounderActivate12,    "sounderactivate12" },
  { Msg_SounderReset12,       "sounderreset12"    },
  { Msg_SounderActivate13,    "sounderactivate13" },
  { Msg_SounderReset13,       "sounderreset13"    },
  { Msg_SounderActivate14,    "sounderactivate14" },
  { Msg_SounderReset14,       "sounderreset14"    },
  { Msg_SounderActivate15,    "sounderactivate15" },
  { Msg_SounderReset15,       "sounderreset15"    },
  { MESSAGE_UNKNOWN,          "Unknown"           }
};



/**
 * @brief Look up a name in the message table and return the message number
 *
 * @param  name  pointer to the message name
 * @return message number
 *
**/
int message_lookup_name(char *name)
{
  struct message_struct *msg_ptr = messages;
  
/* Search for the message string in the message structure */
  while (msg_ptr->msg_number != MESSAGE_UNKNOWN && xstrcmp(msg_ptr->msg_str, name))
    msg_ptr++;

  return(msg_ptr->msg_number);
}



/**
 * @brief Look up a message number in the message table and return the message name
 *
 * @param  number  message number
 * @return pointer to the message name
 *
**/
char *message_lookup_number(int number)
{
  struct message_struct *msg_ptr = messages;  
  
/* Search for the message string in the message structure */
  while ((msg_ptr->msg_number != MESSAGE_UNKNOWN) && (msg_ptr->msg_number != number))
    msg_ptr++;  

  return(msg_ptr->msg_str);
}



/**
 * @brief Print the message table
 *
 * @param  None
 * @return None
 *
**/
void message_print(void)
{
  struct message_struct *msg_ptr = messages;  

  while (msg_ptr->msg_number != MESSAGE_UNKNOWN)
  {
    xprintf("(%d) %s\n", msg_ptr->msg_number, msg_ptr->msg_str);
    msg_ptr++;  
  }  
}



/**
 * @brief Check if the message number is valid in a non-contiguous list
 *
 * @param  message_number    message number
 * @return true if message_number is in the message table
 *
**/
int message_is_valid(int message_number)
{
  struct message_struct *msg_ptr = messages;  

  while (msg_ptr->msg_number != MESSAGE_UNKNOWN)
  {
    if (msg_ptr->msg_number == message_number)
      return(1);
    msg_ptr++;
  }    

  return(0);
}



/**
 * @brief Send a message
 *
 * @param  message_type    message type to send
 * @return None
 * 
 * Not really the right place for this but it is to do with messages
 * 
 * 
**/
void message_send(int message_type)
{
  tx_queue_insert(message_type, 0);
  
  if (params.resend_delay == 0)
	return;
	
  tx_queue_insert(message_type, params.resend_delay + (random() % params.resend_delay));
}

