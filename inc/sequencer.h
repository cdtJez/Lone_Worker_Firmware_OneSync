/**
 * @file		sequencer.h
 * @brief   Header file for the sequencer for the LEDs, buzzer and vibrator
 *
**/
#ifndef SEQUENCER_H
#define SEQUENCER_H

/* LED, buzzer and vibrator port values */
#define BLANK                      0x00
#define LED_B                     (1 << 0)                    /* Blue     */
#define LED_R                     (1 << 1)                    /* Red      */
#define LED_G                     (1 << 2)                    /* Green    */
#define LED_C                     (LED_G | LED_B)             /* Cyan     */
#define LED_Y                     (LED_R | LED_G)             /* Yellow   */
#define LED_M                     (LED_R | LED_B)             /* Magenta  */
#define LED_W                     (LED_R | LED_G | LED_B)     /* White    */

#define VIB                       (1 << 3)
#define BUZZ                      (1 << 4)


/* LED, buzzer & vibrator sequence tables */
extern uint8_t sequence_startup[];
extern uint8_t sequence_flash_red[];
extern uint8_t sequence_flash_red_fast[];
extern uint8_t sequence_flash_green[];
extern uint8_t sequence_flash_green_fast[];
extern uint8_t sequence_flash_yellow[];
extern uint8_t sequence_flash_magenta[];
extern uint8_t sequence_flash_cyan[];
extern uint8_t sequence_colours[];  

void sequencer_init(void);
void sequencer_setup(int loop, uint8_t *table_ptr);
void sequencer_stop(void);
void sequencer_update(void);
void sequencer_static(uint8_t data);

#endif
