/*
 * Binary to c header
 */
#include <stdlib.h>
#include <stdio.h> 
#include <string.h> 



/**
 * @brief DJB2 hash
 *
**/
uint32_t djb2_hash (unsigned char *str)
{
  uint32_t hash = 5381;
  int c;

  while ((c = *str++))
    hash = hash * 33 + c;

  return hash;
}


int main (int argc, char *argv[])
{
	if (argc < 2)
		return (1);
		
	printf ("%08X\n", djb2_hash (argv[1]));
	return (0);
}

