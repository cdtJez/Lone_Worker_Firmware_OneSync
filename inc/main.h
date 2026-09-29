/**
 * @file  main.h
 * @brief Header file for main.c
 *
**/
#ifndef MAIN_H
#define MAIN_H

/* Uncomment to display register names in the reg command */
//#define DEBUG_REGS

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>

/* A structure to hold packet information */
struct packet_t {
  uint8_t network_id;
  uint8_t sending_device;		  /* Device message was from  */
	uint8_t message_type;				/* Message type             */
};

#include "stm32g030xx.h"
#include "stm32g0xx.h"
#include "delay.h"
#include "rfm69hw_regs.h"
#include "rfm69hw.h"
#include "spi.h"
#include "usart.h"
#include "command.h"
#include "event.h"
#include "inputs.h"
#include "xstrings.h"
#include "parameters.h"
#include "flash.h"
#include "regs.h"
#include "version.h"
#include "buildnum.h"
#include "messages.h"
#include "rtc.h"
#include "adc.h"
#include "crc.h"
#include "device.h"
#include "random.h"
#include "message_table.h"
#include "ht12e.h"
#include "tx_queue.h"
#include "fxls8964.h"
#include "sequencer.h"
#include "machine_state.h"
#include "i2c.h"
#include "charger.h"
#include "program.h"


#define SYSCLK              16000000

/* LED pin is PB3 */
#define LED_BIT             3
#define LED_OFF             (GPIOB->BSRR = (1 << LED_BIT))
#define LED_ON              (GPIOB->BRR  = (1 << LED_BIT))

#define GPIO_PUPD_NOPULL    0
#define GPIO_PUPD_PULLUP    1
#define GPIO_PUPD_PULLDOWN  2  

#define TICKS_PER_SECOND    10
#define SECONDS_IN_1_MINUTE 60
#define MINUTES_IN_1_HOUR   60
#define TICKS_IN_1_MINUTE   (TICKS_PER_SECOND * SECONDS_IN_1_MINUTE)
#define SECONDS_IN_1_HOUR   (SECONDS_IN_1_MINUTE * MINUTES_IN_1_HOUR)
#define TICKS_IN_1_HOUR     (SECONDS_IN_1_HOUR   * TICKS_PER_SECOND)  

extern uint32_t tick;

#endif
