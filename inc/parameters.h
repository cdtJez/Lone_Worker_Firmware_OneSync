/**
 * @file  parameters.h
 * @brief Header file for parameters.c
 *
**/
#ifndef PARAMETERS_H
#define PARAMETERS_H

extern struct parameters params;

#define FLASH_TOP_PAGE          0x08007800  

#define PARAMETER_STRING_LEN    16

/* Increment the version for each table change */
#define STRUCTURE_VERSION        1

/* Structure to hold a parameter set */
struct parameters {
	uint8_t  network_id;           /* On legacy devices this is used as the system address                    */
	uint8_t  device_id;            /* On legacy devices botton 4 bits used as device ID                       */
  uint8_t  invert_input_mask;    /* Setting a bit inverts the corresponding input                           */
  uint8_t  in_1_act_msg;         /* Message to send on input 1 activation                                   */
  uint8_t  in_1_deact_msg;       /* Message to send on input 1 de-activation                                */
  uint8_t  in_2_act_msg;         /* Message to send on input 2 activation                                   */
  uint8_t  in_2_deact_msg;       /* Message to send on input 2 de-activation                                */  
  uint8_t  in_3_act_msg;         /* Message to send on input 3 activation                                   */
  uint8_t  in_3_deact_msg;       /* Message to send on input 3 de-activation                                */  
  uint8_t  in_4_act_msg;         /* Message to send on input 4 activation                                   */
  uint8_t  in_4_deact_msg;       /* Message to send on input 4 de-activation                                */  
  uint8_t  active_resend_time;   /* Tx will repeat every n seconds if the input is active (1..255, 0 = off) */ 
  uint8_t  tx_power;             /* Transmit power level 0..23                                              */
  uint8_t  battery_state_time;   /* How often to read battery and report condition (hours, 0 = never)       */
  uint8_t  still_alive_time;     /* How often to send a still alive message (hours, 0 = never)              */
  uint8_t  tilt_timeout;         /* How many seconds to wait before sending a tilt alarm                    */
  uint8_t  tilt_resend_time;     /* How many seconds between tilt alarm re-sends (0 = never)                */ 
  uint8_t  repeater_delay;       /* Delay for repeater to re-transmitting (1/10s)(0..25.5s) + random        */
  uint8_t  resend_delay;         /* If > 0, resend a message after a delay + random                         */
  uint8_t  structure_version;    /* Version of the parameter structure                                      */
  char     model[PARAMETER_STRING_LEN];     /* Model number string                                          */
  char     serial[PARAMETER_STRING_LEN];    /* Serial number string                                         */
  uint32_t checksum;             /* Checksum of all the data before the checksum                            */
};

void parameter_default(struct parameters *p);
int  parameter_load(struct parameters *p);
void parameter_save(struct parameters *p);
void parameter_print(struct parameters *p);

#endif