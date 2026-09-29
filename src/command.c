/**
 * @file  command.c
 * @brief Serial interface command decoder and functions
 *
**/
#include "main.h"

#define COMMAND_BUFFER_LENGTH   64
#define MAXARGS                  8

#define NO_ECHO_CHAR						'#'

#define CHAR_LF                 0x0A
#define CHAR_CR                 0x0D
#define CHAR_BS                 0x08

#define ECHO_UNDEFINED          99
#define ECHO_OFF                 0
#define ECHO_ON                  1

char command_buffer[COMMAND_BUFFER_LENGTH];
int  command_index = 0;
int argc;
char *argv[MAXARGS + 1];

/* Command function prototypes */
void command_status(void);
void command_help(void);
void command_reg(void);
void command_test(void);
void command_rssi(void);
void command_temp(void);
void command_version(void);
void command_dump(void);
void command_deviceid(void);
void command_networkid(void);
void command_power(void);
void command_tilt_timeout(void);
void command_tilt_resend(void);
void command_battery_state_time(void);
void command_still_alive(void);
void command_model(void);
void command_serial(void);
void command_resend_time(void);
void command_wipe_device(void);
void command_message_print(void);
void command_input(void);
void command_volt(void);
void command_repeat(void);
void command_invert(void);
void command_resend_delay(void);
void command_tilt(void);
void command_prog_key(void);


/* Structure to hold a command name, function pointer and help string */
struct commands {
  char *command_str;		    /* Command name   */
  void (*fn_ptr) (void);		/* Pointer to command function */
	char *help_str;						/* Help information            */
};


/* Command word, function pointer and help string structure */
/* Pre-fixing NO_ECHO_CHAR stops the command being echoed */
static struct commands cmd[] = {
  {"help",         command_help,               "                 - this help message"},
  {"status",       command_status,             "               - current machine status"},
/*  {"reg",          command_reg,                "<*aa> <*dd>      - read or set RFM69HW registers"}, */
/*  {"test",         command_test,               "                 - test command - various"}, */
/*	{"temp",         command_temp,               "                - read the RFM69HW chip temperature"}, */
/*	{"version",      command_version,            "             - report the current firmware version"}, */
/*	{"dump",         command_dump,               "<*aa> <*dd>     - dump memory default 0x08007800 and 256 bytes"}, */
	{"deviceid",     command_deviceid,           "<*id>        - read or set the device ID"},
	{"networkid",    command_networkid,          "<*id>       - read or set the network ID"},
  {"power",        command_power,              "<level>         - read or set the power level (0..23)"},
  {"tilttime",     command_tilt_timeout,       "<secs>       - read or set the tilt activate time (seconds - 0 = off)"},
  {"tiltresend",   command_tilt_resend,        "<secs>     - read or set the tilt resend time (seconds - 0 = off)"},
  {"batterystate", command_battery_state_time, "<hours>  - read or set the battery state send time (hours - 0 = off)"},
  {"stillalive",   command_still_alive,        "<hours>    - read or set the still alive time (hours - 0 = off)"},
  {"model",        command_model,              "                - read the unit model number"},
  {"serial",       command_serial,             "               - read the unit serial number"},
  {"resend",       command_resend_time,        "<secs>         - read or set the active resend time"},
  {"wipe",         command_wipe_device,        "Y                - reset the device parameters"},
  {"message",      command_message_print,      "              - print the message names and numbers"},
  {"input",        command_input,              "n a/d message   - Set the message sent for input n"},
  {"volt",         command_volt,               "                 - Get the battery voltage in mV"},
  {"repeat",       command_repeat,             "               - read or set the repeater delay (s/10)"},
  {"invert",       command_invert,             "<*mask>        - read or set the input invert mask"},
  {"resenddelay",  command_resend_delay,       "<secs x 10> - if non-zero a packet will be sent twice"},
  {"tilt",         command_tilt,               "                 - Current tilt angles"},
  {"progkey",      command_prog_key,           "key            - Program key"},  
  {"",             NULL,                       ""}
};


/**
 * @brief Initialise the command interface
 *
**/
void command_init(void)
{
  command_index = 0;
}


/**
 * @brief Set argv[] to point to the arguments
 *        Set argc to the number of arguments
 *        Execute argv[0]
 *
 * @param  None
 * @return None
 *
**/
void command_process(void)
{
  char *s = command_buffer;
  struct commands *cmd_ptr = cmd;
  argc = 0;

  while (argc < MAXARGS)
  {
    /* Skip over whitespace */
    while (((*s == ' ') || (*s == '\t')) && (*s != 0))
      s++;

    /* Check for end of string */
    if (*s == 0)
      break;

    /* Now at start of argument so store address */
    argv[argc] = s;

    /* Skip over non-whitespace */
    while ((*s != ' ') && (*s != '\t') && (*s != 0))
      s++;

    /* Check for end of string */
    if (*s == 0)
      break;

    *s = 0;   /* Null terminate the argument */
    s++;
    argc++;
  }
  argc++;     /* argc has counted one to few arguments */

/* Check for a NULL command */
  if (argv[0] == 0)
    return;

/* Search for the command name in the command structure */
  while (cmd_ptr->command_str != 0 && xstrcmp(cmd_ptr->command_str , argv[0]))
    cmd_ptr++;

/* Report and return if no command found */
  if (cmd_ptr->command_str == 0)
  {
    xputs (argv[0]);
    xputs (" - Unknown command\n");
    return;
  }

/* Execute the command */
  (*(cmd_ptr->fn_ptr))();
}


/**
 * @brief Store a character in the command buffer
 *
 * @param  c  Character to store
 * @return none
 *
**/
void command_store(char c)
{
	static int echo = ECHO_UNDEFINED;

	if (echo == ECHO_UNDEFINED)
	{
		if (c == NO_ECHO_CHAR)
		{
			echo = ECHO_OFF;
			return;
		}
		echo = ECHO_ON;
	}

	if (echo)
		xputc(c);				/* Echo character */

/* Carriage return or line feed terminates the command */
  if (c == CHAR_LF || c == CHAR_CR)
  {
    command_buffer[command_index] = 0;  /* Null terminate the command string */
    command_process();

    command_index = 0;                  /* Clear the command buffer index */
		echo = ECHO_UNDEFINED;              /* Reset the echo state */
    return;
  }

/* If we have a backspace character, delete the last character */
  if (c == CHAR_BS)
  {
/* Test if we have a character to delete */
    if (command_index == 0)
      return;
    
    xputc(CHAR_BS);
    xputc(' ');
    xputc(CHAR_BS);
    command_index--;
    return;
  }

/* Add the character to the command buffer */
  command_buffer[command_index++] = c;
  if (command_index == COMMAND_BUFFER_LENGTH)
    command_index--;
}


/**
 * @brief Report the machine status
 *
**/
void command_status(void)
{
  parameter_print(&params);
}


/**
 * @brief Print the command help information
 *
 * @param  none
 * @return none
 *
**/
void command_help(void)
{
  struct commands *cmd_ptr = cmd;

	xprintf("\n\nPrefix with %c for no echo\n", NO_ECHO_CHAR);
  xprintf("Input numbers with * can be prefixed with h for hexadecimal or b for binary. Default decimal\n");
	while (cmd_ptr->command_str != 0)
	{
		xprintf ("%s %s\n", cmd_ptr->command_str, cmd_ptr->help_str);
		cmd_ptr++;
	}
}


/**
 * @brief Display or write a register on the RFM69HW
 *
 * No arguments  - Dump the whole register file
 * One argument  - Print the contents of reg(arg1)
 * Two arguments - Write arg2 into reg(arg1)
 *
**/
void command_reg(void)
{
  if (argc == 1)
    rfm69hw_register_dump ();
  else if (argc == 2)
  {
    xprintf("%02x\n", rfm69hw_read_reg(xbasetoi(argv[1])));
  }
  else if (argc == 3)
  {
    int addr = xbasetoi(argv[1]);
    int data = xbasetoi(argv[2]);

    xprintf("%02x -> [%02x]\n", data, addr);
    rfm69hw_write_reg(addr, data);
  }
}


/**
 * @brief Test stuff here!
 *
**/
void command_test(void)
{
//	xputs("Testing...\n");
  
//	if (argc == 2)
//	{
//		random_seed(xatoi(argv[1]));
//	}

//	xprintf("%08x\n", random());  
//  xprintf("%d %d\n", params.resend_delay, random() % params.resend_delay);
  
}


/**
 * @brief Read the RSSI from the RFM69HW
 *
**/
void command_rssi(void)
{
	xprintf("%d\n", rfm69hw_read_rssi());
}


/**
 * @brief Read the temperature from the RFM69HW
 *
**/
void command_temp(void)
{
	xprintf("%d\n", rfm32hw_temperature());
}


/**
 * @brief Read the version number from the RFM69HW
 *
**/
void command_version(void)
{
	xprintf("%02x\n", rfm69hw_read_reg(RegVersion));
}


/**
 * @brief Dump memory
 *
**/
void command_dump(void)
{
/* Default to the top flash page (parameter store) */
	uint32_t address = FLASH_TOP_PAGE;
	int length = 256;

	if (argc > 1) address = xbasetoi(argv[1]);
	if (argc > 2)	length  = xbasetoi(argv[2]);
	xdump(address, length);
}


/**
 * @brief Set or read the device ID
 *
**/
void command_deviceid(void)
{
	if (argc == 2)
	{
		params.device_id = xbasetoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%02x\n", params.device_id);
}


/**
 * @brief Set or read the network ID
 *
**/
void command_networkid(void)
{
	if (argc == 2)
	{
		params.network_id = xbasetoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%02x\n", params.network_id);
}


/**
 * @brief Set or read the power level
 *
**/
void command_power(void)
{
	if (argc == 2)
	{
    int power = xatoi(argv[1]);
    if (power > 23)
      power = 23;

		params.tx_power = power;
		parameter_save(&params);
	}

	xprintf("%d\n", params.tx_power);
}


/**
 * @brief Set or read the tilt timeout time (seconds)
 *
 * When non-zero the unit will wait n seconds (default 15) before sending a tilt alarm.
 * When zero the function is off
 *
**/
void command_tilt_timeout(void)
{
	if (argc == 2)
	{
		params.tilt_timeout = xatoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%d\n", params.tilt_timeout);
}


/**
 * @brief Set or read tilt_resend_time (seconds)
 *
 * When non-zero the unit will resend a tilt message every n seconds
 * When zero no resend will be sent
 *
**/
void command_tilt_resend(void)
{
	if (argc == 2)
	{
		params.tilt_resend_time = xatoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%d\n", params.tilt_resend_time);
}


/**
 * @brief Set or read the battery state report time (hours)
 *
 * When non-zero the unit will report the battery status every n hours
 *
**/
void command_battery_state_time(void)
{
	if (argc == 2)
	{
		params.battery_state_time = xatoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%02x\n", params.battery_state_time);
}


/**
 * @brief Set or read the still alive report time (hours)
 *
 * When non-zero a still alive message will be sent every n hours
 *
**/
void command_still_alive(void)
{
	if (argc == 2)
	{
		params.still_alive_time = xatoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%02x\n", params.still_alive_time);
}


/**
 * @brief Set or send the model string
 *
**/
void command_model(void)
{
	if (argc == 2)
	{
    xstrcpyzlen(&params.model[0] , argv[1], PARAMETER_STRING_LEN - 1);
    params.model[PARAMETER_STRING_LEN - 1] = 0;
    parameter_save(&params);
	}

  xputsn(&params.model[0], PARAMETER_STRING_LEN - 1);
  xputc('\n');
}


/**
 * @brief Set or send the serial string
 *
 *
**/
void command_serial(void)
{
	if (argc == 2)
	{
    xstrcpyzlen(&params.serial[0] , argv[1], PARAMETER_STRING_LEN - 1);
    params.serial[PARAMETER_STRING_LEN - 1] = 0;
    parameter_save(&params);
	}

  xputsn(&params.serial[0], PARAMETER_STRING_LEN - 1);
  xputc('\n');
}


/**
 * @brief   Send the battery voltage
 *
 *
**/
void command_volt(void)
{
  xprintf("%d\n", adc_read_battery_mV());
}


/**
 * @brief   Print or set the active resend time
 *
 * Setting this value to non-zero will send a packet every active_resend_time while the input is active
**/
void command_resend_time(void)
{
	if (argc == 2)
	{
		params.active_resend_time = xatoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%02x\n", params.active_resend_time);
}


/**
 * @brief   Wipe the device - set the parameters to default
 *
 *
**/
void command_wipe_device(void)
{
  if (argv[1][0] != 'Y')
    return;
  
  parameter_default(&params);
  parameter_save(&params);
  xprintf("Parameters set to default\n");
}



/**
 * @brief   Print the messages and message numbers
 *
 *
**/
void command_message_print(void)
{
	if (argc == 2)
	{
    if (xisdigit(argv[1][0]))
    {
      int num = xatoi(argv[1]);

      xprintf("(%d) %s\n", num, message_lookup_number(num));
    }
    else
    {
      int num = message_lookup_name(argv[1]);

      if (num == MESSAGE_UNKNOWN)
        xprintf("Unknown message - %s\n", argv[1]);
      else
        xprintf("(%d) %s\n", num, argv[1]);
    }
	}
  else
    message_print();
}


/**
 * @brief   Set the message sent for each input
 *
 *  input            - prints all the settings for all inputs
 *  input 1          - prints the activate and deactivate messages for input 1
 *  input n m        - prints the relevent message for input n where m = a for activate or d for de-activate
 *  input 1 d 3      - sets the de-activate message number for input 1
 *  input 1 a press1 - sets the activate message for input 1
 *
 *  Messages can be a message name or number, see the message command for a list
 *
**/
void command_input(void)
{
  uint8_t *msg = &params.in_1_act_msg;

  if (argc == 1)
  {
    for (int in = 0; in < (DEVICE_INPUTS * 2); in++)
      xprintf("Input %d %s : %02d - %s\n", (in / 2) + 1, (in & 1) ? "deactivate" : "activate  ", msg[in], message_lookup_number(msg[in]));

    return;
  }

  int inp = xatoi(argv[1]) - 1;
  if (inp >= DEVICE_INPUTS)
  {
    xprintf("Unknown input %s\n", argv[1]);
    return;
  }

  if (argc == 2)
  {
    xprintf("Input %d activate   : %02d - %s\n", inp + 1, msg[inp * 2],       message_lookup_number(msg[inp * 2]));
    xprintf("Input %d deactivate : %02d - %s\n", inp + 1, msg[(inp * 2) + 1], message_lookup_number(msg[(inp * 2) + 1]));
    return;
  }

  int act;
  if (argv[2][0] == 'a')
    act = 0;
  else if (argv[2][0] == 'd')
    act = 1;
  else
  {
    xprintf("Unknown activity: %s\n", argv[2]);
    return;
  }

  if (argc == 3)
  {
    xprintf("Input %d %s : %s\n", inp + 1, (act == 0) ? "activate" : "deactivate", message_lookup_number(msg[(inp * 2) + act]));
    return;
  }

/* Read the new message, it could be a number or text */
  int new_msg;
  if (xisdigit(argv[3][0]))
    new_msg = xatoi(argv[3]);
  else
    new_msg = message_lookup_name(argv[3]);

/* Test for valid message number - there are some holes! */  
//  if (!(message_is_valid(new_msg)))
//  {
//    xprintf("Unknown message\n");    
//    return;
//  }
  
  if (argc == 4)
  {
    msg[(inp * 2) + act] = new_msg;
    parameter_save(&params);
    return;
  }
  
  xprintf("Too many arguments\n");
}


/**
 * @brief Set or send the repeater delay
 *
**/
void command_repeat(void)
{
	if (argc == 2)
	{
		params.repeater_delay = xatoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%02x\n", params.repeater_delay);  
}


/**
 * @brief Set or send the input invert flag
 *
**/
void command_invert(void)
{
	if (argc == 2)
	{
		params.invert_input_mask = xbasetoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%02x - %08b\n", params.invert_input_mask, params.invert_input_mask);   
}


/**
 * @brief Set or send the resend delay
 *
 * If this value is non-zero, a second transmit is scheduled with a timeout of resend_delay
 * This will be ignored if active_resend_time is set
**/
void command_resend_delay(void)
{
	if (argc == 2)
	{
		params.resend_delay = xatoi(argv[1]);
		parameter_save(&params);
	}

	xprintf("%02x\n", params.resend_delay);  
}



/**
 * @brief I2C test
 *
**/
void command_tilt(void)
{
  xprintf("%d\n", fxls8964_read_x());
}



/**
 * @brief Program mode secret keys
 *
**/
void command_prog_key(void)
{
	if (argc == 2)
	{
		program_key(xbasetoi(argv[1]));
	}  
}


