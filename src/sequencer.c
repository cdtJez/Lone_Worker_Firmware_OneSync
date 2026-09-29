/**
 * @file  sequence.c
 * @brief Sequencer for LEDs, buzzer and vibrator
 *
**/
#include "main.h"


#define LED_BLUE_BIT       0
#define LED_BLUE_MASK     (1 << LED_BLUE_BIT)

#define LED_RED_BIT        1
#define LED_RED_MASK      (1 << LED_RED_BIT)

#define LED_GREEN_BIT      2
#define LED_GREEN_MASK    (1 << LED_GREEN_BIT)

#define VIBRATOR_BIT       3
#define VIBRATOR_MASK     (1 << VIBRATOR_BIT)

#define BUZZER_BIT         4
#define BUZZER_MASK       (1 << BUZZER_BIT)

//#define SEQUENCE_PORT_BIT_MASK    (LED_RED_MASK | LED_GREEN_MASK | LED_BLUE_MASK | VIBRATOR_MASK | BUZZER_MASK)
#define SEQUENCE_PORT_BIT_MASK    (LED_RED_MASK | LED_GREEN_MASK | LED_BLUE_MASK | VIBRATOR_MASK)
#define SEQUENCE_TABLE_END_TOKEN  0xff


int sequence_loop_counter = 0;    /* Number of times to go through the table              */
uint8_t *sequence_table;          /* Pointer to the start of the sequence table           */
uint8_t *sequence_table_ptr;      /* Pointer to the current element in the sequence table */

/* Sequence tables must end with SEQUENCE_TABLE_END_TOKEN */
uint8_t sequence_startup[]          = { LED_R | VIB, LED_R, LED_R | VIB, LED_G, LED_G | VIB, LED_G, LED_B | VIB, LED_B | BUZZ,
                                        LED_B | VIB | BUZZ, SEQUENCE_TABLE_END_TOKEN };
uint8_t sequence_flash_red[]        = { LED_R, BLANK, BLANK,   VIB, BLANK, BLANK, BLANK,  BUZZ, BLANK, BLANK, SEQUENCE_TABLE_END_TOKEN };
uint8_t sequence_flash_green[]      = { BLANK, LED_G, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, SEQUENCE_TABLE_END_TOKEN };
uint8_t sequence_flash_yellow[]     = { LED_Y, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
                                        BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
                                        BLANK, BLANK, BLANK, BLANK, SEQUENCE_TABLE_END_TOKEN };
uint8_t sequence_flash_green_fast[] = { BLANK, LED_G, BLANK, SEQUENCE_TABLE_END_TOKEN };
uint8_t sequence_flash_red_fast[]   = { BLANK, LED_R, BLANK, SEQUENCE_TABLE_END_TOKEN };
uint8_t sequence_flash_magenta[]    = { BLANK, LED_M, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, SEQUENCE_TABLE_END_TOKEN };
uint8_t sequence_flash_cyan[]       = { BLANK, LED_C, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, SEQUENCE_TABLE_END_TOKEN };
uint8_t sequence_colours[]          = { BLANK, LED_R, LED_R, BLANK, LED_G, LED_G, BLANK, LED_B, LED_B, BLANK, SEQUENCE_TABLE_END_TOKEN };



/**
 * @brief Initialise the LEDs, buzzer & vibrator port lines
 *
**/
void sequencer_init(void)
{
/* Set PB0 (LED blue), PB1 (LED red), PB2 (LED green), PB3 (vibrator) to outputs & PB4 (buzzer) */
  MODIFY_REG(GPIOB->MODER, ((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6) | (3 << 8)), ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8)));

/* Set the outputs low */
  GPIOB->BRR = SEQUENCE_PORT_BIT_MASK;

/* Set AF1 (TIM3 CH1) for PB4 */
  MODIFY_REG(GPIOB->AFR[0], (15 << 16), (1 << 16));

  sequencer_setup(1, sequence_startup);
}



/**
 * @brief Set up the sequencer
 *
 * @param loop        number of times to go through the sequence table
 * @param table_ptr   pointer to the sequence table
**/
void sequencer_setup(int loop, uint8_t *table_ptr)
{
  sequence_loop_counter = loop;
  sequence_table = table_ptr;
  sequence_table_ptr = table_ptr;
}



/**
 * @brief Stop the sequencer
 *
**/
void sequencer_stop(void)
{
  sequence_loop_counter = -1;
  GPIOB->BRR = SEQUENCE_PORT_BIT_MASK;    /* Clear the port lines */
}



/**
 * @brief Sequencer for the LEDs, buzzer & vibrator port lines
 *
**/
void sequencer_update(void)
{
/* If the loop counter is minus one then the sequencer is inactive */
  if (sequence_loop_counter == -1)
    return;

/* If the loop counter is zero then turn off the LEDs */
  if (sequence_loop_counter == 0)
  {
    GPIOB->BRR = SEQUENCE_PORT_BIT_MASK;    /* Clear the port lines */
    sequence_loop_counter = -1;
    return;
  }

/* Set the next port values from the sequence table */
  MODIFY_REG(GPIOB->ODR, SEQUENCE_PORT_BIT_MASK, *sequence_table_ptr & SEQUENCE_PORT_BIT_MASK);
    

/* Increment the sequence pointer and check if it is pointing to the end of the table */
  sequence_table_ptr++;
  if (*sequence_table_ptr == SEQUENCE_TABLE_END_TOKEN)
  {
    sequence_table_ptr = sequence_table;    /* Point to start of sequence table */
    sequence_loop_counter--;
  }
}



/**
 * @brief Set the LED, buzzer and vibrator to a static value - used in charge mode
 *
**/
void sequencer_static(uint8_t data)
{
  MODIFY_REG(GPIOB->ODR, SEQUENCE_PORT_BIT_MASK, data & SEQUENCE_PORT_BIT_MASK);
}

