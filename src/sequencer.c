/**
 * @file  sequence.c
 * @brief Sequencer for LEDs, buzzer and vibrator
 *
**/
#include "main.h"


#define LED_BLUE_BIT       13
#define LED_BLUE_MASK     (1 << LED_BLUE_BIT)

#define LED_RED_BIT        14
#define LED_RED_MASK      (1 << LED_RED_BIT)

#define LED_GREEN_BIT      15
#define LED_GREEN_MASK    (1 << LED_GREEN_BIT)

#define VIBRATOR_BIT       5
#define VIBRATOR_MASK     (1 << VIBRATOR_BIT)

#define BUZZER_BIT         0
#define BUZZER_MASK       (1 << BUZZER_BIT)

#define SEQUENCE_LED_PORT_MASK    (LED_RED_MASK | LED_GREEN_MASK | LED_BLUE_MASK)
#define SEQUENCE_TABLE_END_TOKEN  0xff

/* Map the existing byte-sized sequence flags to the new physical ports.
 * Buzzer flags remain disabled, as in the original port mask.
 */
static void sequence_write_pins(uint8_t data)
{
  uint32_t leds = ((data & LED_B) ? LED_BLUE_MASK : 0U) |
                  ((data & LED_R) ? LED_RED_MASK : 0U) |
                  ((data & LED_G) ? LED_GREEN_MASK : 0U);
  MODIFY_REG(GPIOC->ODR, SEQUENCE_LED_PORT_MASK, leds);
  MODIFY_REG(GPIOB->ODR, VIBRATOR_MASK, (data & VIB) ? VIBRATOR_MASK : 0U);
}


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
/* PC13/PC14/PC15 are blue/red/green; PB5 is vibrator; PB0 is buzzer. */
  MODIFY_REG(GPIOC->MODER, ((3UL << 26) | (3UL << 28) | (3UL << 30)),
             ((1UL << 26) | (1UL << 28) | (1UL << 30)));
  MODIFY_REG(GPIOB->MODER, ((3 << 10) | (3 << 0)), ((1 << 10) | (1 << 0)));

/* Set the outputs low */
  GPIOC->BRR = SEQUENCE_LED_PORT_MASK;
  GPIOB->BRR = VIBRATOR_MASK;

/* Set AF1 (TIM3 CH3) for PB0. */
  MODIFY_REG(GPIOB->AFR[0], (15 << 0), (1 << 0));

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
  GPIOC->BRR = SEQUENCE_LED_PORT_MASK;
  GPIOB->BRR = VIBRATOR_MASK;    /* Clear the port lines */
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
    GPIOC->BRR = SEQUENCE_LED_PORT_MASK;
  GPIOB->BRR = VIBRATOR_MASK;    /* Clear the port lines */
    sequence_loop_counter = -1;
    return;
  }

/* Set the next port values from the sequence table */
  sequence_write_pins(*sequence_table_ptr);
    

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
  sequence_write_pins(data);
}

