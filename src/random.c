 /**
 * @file  random.c
 * @brief Simple random function using a linear congruential generator
 *
**/
#include "main.h"

uint32_t random_state;


/**
 * @brief   Seed the random number generator
 *
 * @param   seed    New random seed
 *
**/
void random_seed(uint32_t seed)
{
  random_state = seed;
}


/**
 * @brief   Generates a random number
 *
 * @return  random number in range 0..(2^31 - 1)
 *
 *
**/
uint32_t random(void)
{
  random_state = ((random_state * 1103515245) + 12345) & 0x7fffffff;
  
  return (random_state);
}

