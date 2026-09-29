/**
 * @file  parameters.c
 * @brief System parameters
 *
**/
#include "main.h"

//#define FLASH_TOP_PAGE  0x08007800  
#define PARAMETER_OFFSET  0
#define PARAMETER_ADDRESS (FLASH_TOP_PAGE + PARAMETER_OFFSET)

struct parameters params;



/**
 * @brief   Generate a checksum for the parameter data
 *
 * @param   pointer to parameter block
 * @return  Checksum
 *
**/
uint32_t parameter_generate_checksum(struct parameters *p)
{
  return(crc32((uint8_t *)p, sizeof(*p) - 4));
}



/**
 * @brief Default set of parameters
 *
 * @param p pointer to parameter structure
 * @return none
 *
 * Default values are defined in device.h
**/
void parameter_default(struct parameters *p)
{
  p->network_id         = DEVICE_DEFAULT_NETWORK_ID;
  p->device_id          = DEVICE_DEFAULT_DEVICE_ID;
  p->invert_input_mask  = DEVICE_DEFAULT_INVERT_INPUT_MASK; 
  p->in_1_act_msg       = DEVICE_DEFAULT_IN_1_ACT_MSG;
  p->in_1_deact_msg     = DEVICE_DEFAULT_IN_1_DEACT_MSG;
  p->in_2_act_msg       = DEVICE_DEFAULT_IN_2_ACT_MSG;
  p->in_2_deact_msg     = DEVICE_DEFAULT_IN_2_DEACT_MSG;  
  p->in_3_act_msg       = DEVICE_DEFAULT_IN_3_ACT_MSG;
  p->in_3_deact_msg     = DEVICE_DEFAULT_IN_3_DEACT_MSG;  
  p->in_4_act_msg       = DEVICE_DEFAULT_IN_4_ACT_MSG;
  p->in_4_deact_msg     = DEVICE_DEFAULT_IN_4_DEACT_MSG;  
  p->active_resend_time = DEVICE_DEFAULT_ACTIVE_RESEND_TIME;
  p->battery_state_time = DEVICE_DEFAULT_BATTERY_STATUS_TIME;
  p->still_alive_time   = DEVICE_DEFAULT_STILL_ALIVE_TIME;
  p->tx_power           = DEVICE_DEFAULT_TX_POWER;
  p->tilt_timeout       = DEVICE_DEFAULT_TILT_TIMEOUT;
  p->tilt_resend_time   = DEVICE_DEFAULT_TILT_RESEND; 
  p->repeater_delay     = DEVICE_DEFAULT_REPEATER_DELAY;
  p->resend_delay       = DEVICE_DEFAULT_RESEND_DELAY;
  p->structure_version  = STRUCTURE_VERSION;
  xstrcpyz(&p->model[0],  DEVICE_DEFAULT_NAME);
  xstrcpyz(&p->serial[0], DEVICE_DEFAULT_SERIAL_NUMBER);
}



/**
 * @brief Save the parameters to flash
 *
 * @param  none
 * @return none
 *
**/
void parameter_save(struct parameters *p)
{
  p->checksum = parameter_generate_checksum (p);
  flash_write((uint8_t *)PARAMETER_ADDRESS, sizeof (*p), (uint8_t *)p);
}



/**
 * @brief Load the parameters from flash
 *
 * @param p pointer to parameter structure
 * @return 1 on loading a valid parameter set
 *         0 if the defaults are loaded
 *
**/
int parameter_load(struct parameters *p)
{
  flash_read((uint8_t *)PARAMETER_ADDRESS, sizeof (*p), (uint8_t *)p);
  
  /* Check to see if the loaded parameters are invalid - this should happen on first boot or error */
  if (parameter_generate_checksum (p) != p->checksum)
  {
    parameter_default(p);
    parameter_save(p);        /* Save the default parameters */
    return(1);
  }
  
  return(0);
}



/**
 * @brief Print the parameters
 *
 * @param p pointer to parameter structure
 * @return None
 *
**/
void parameter_print(struct parameters *p)
{
  xprintf("Network ID      (0..ff)      - %08b (%02x)\n", p->network_id, p->network_id);
  xprintf("Device ID       (0..ff)      - %08b (%02x)\n", p->device_id,  p->device_id);
  xprintf("Invert input mask            - %08b\n",        p->invert_input_mask); 
  xprintf("Input 1 act message          - %d - %s\n",     p->in_1_act_msg,   message_lookup_number(p->in_1_act_msg));
  xprintf("Input 1 deact message        - %d - %s\n",     p->in_1_deact_msg, message_lookup_number(p->in_1_deact_msg));
  xprintf("Input 2 act message          - %d - %s\n",     p->in_2_act_msg,   message_lookup_number(p->in_2_act_msg));
  xprintf("Input 2 deact message        - %d - %s\n",     p->in_2_deact_msg, message_lookup_number(p->in_2_deact_msg));
  xprintf("Input 3 act message          - %d - %s\n",     p->in_3_act_msg,   message_lookup_number(p->in_3_act_msg));
  xprintf("Input 3 deact message        - %d - %s\n",     p->in_3_deact_msg, message_lookup_number(p->in_3_deact_msg));
  xprintf("Input 4 act message          - %d - %s\n",     p->in_4_act_msg,   message_lookup_number(p->in_4_act_msg));
  xprintf("Input 4 deact message        - %d - %s\n",     p->in_4_deact_msg, message_lookup_number(p->in_4_deact_msg));
  xprintf("Active resend time    (s)    - %d\n",          p->active_resend_time);  
  xprintf("Transmit power  (0..23)      - %d\n",          p->tx_power);
  xprintf("Battery state time    (h)    - %d\n",          p->battery_state_time);
  xprintf("Still alive time      (h)    - %d\n",          p->still_alive_time);
  xprintf("Tilt timeout          (s)    - %d\n",          p->tilt_timeout);
  xprintf("Tilt resend time      (s)    - %d\n",          p->tilt_resend_time); 
  xprintf("Repeater delay        (s/10) - %d\n",          p->repeater_delay);
  xprintf("Resend delay          (s/10) - %d\n",          p->resend_delay);
  xprintf("Model                        - %s\n",          p->model);
  xprintf("Serial number                - %s\n",          p->serial);
  xprintf("Parameter structure version  - %d\n",          p->structure_version);
	xprintf("Checksum                     - %08x\n",        p->checksum);
}
