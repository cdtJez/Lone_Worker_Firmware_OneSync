/**
 * @file  crc.c
 * @brief CRC32 function
 *
**/
#include "main.h"


/**
 * @brief   CRC32 - Generate a 32 bit CRC
 *
 * @param   data    pointer to data
 * @param   len     length of data
 * @return  CRC32   checksum
 *
**/
unsigned int crc32(unsigned char *data, int len)
{
  unsigned int crc = 0xFFFFFFFF;

  while (len--)
  {
    crc = crc ^ *data++;
    for (int j = 8; j > 0; j--)
    {
      unsigned int mask = -(crc & 1);
      crc = (crc >> 1) ^ (0xEDB88320 & mask);
    }
  }
  return(~crc);
}

