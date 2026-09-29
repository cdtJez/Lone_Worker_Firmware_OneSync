/**
 *
 * @file		led.h
 * @brief   Header file for led.c
 *
 *
**/
#ifndef LED_H
#define LED_H

void led_init(void);
void led_on(uint32_t count);
void led_off(void);
void led_sequencer(void);

#endif