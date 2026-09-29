/*
 * @file    xstring.c
 * @brief   String functions
 *
 *
**/
#include "main.h"

#define HEX_NUMBER_DESIGNATOR     'h'
#define BINARY_NUMBER_DESIGNATOR  'b'

/**
 * @brief Convert a lower case character to upper case 
 *
 * @param  c - Character to convert
 * @return Converted character
 *
**/
int xtoupper(int c)
{
  return((c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c);
}


/**
 * @brief Test if a character is a digit
 *
 * @param  c - Character to test
 * @return 1 if it is a digit else 0
 *
**/
int xisdigit(char c)
{
  return((c >= '0' && c <= '9') ? 1 : 0);
}



/**
 * @brief Convert a number string to an integer
 *
 * @param  s - pointer to number string
 * @return number equivalent of number string
 *
**/
int32_t xatoi(char *s)
{
  int neg;
  char c;
   
  if ((neg = (*s == '-')))
      s++;
      
  int value = 0;
  while ((c = *s++)) 
  {
    if (c < '0' || c > '9')
      break;
   
    value = (value * 10) + (c - '0');
  }

  return(neg ? -value : value);
}


/**
 * @brief Convert a single hexadecimal digit to an integer
 *
 * @param  c - hexadecimal digit character
 * @return number equivalent of hexadecimal character
 *
**/
int xxtod(char c)
{
  if (c >= '0' && c <= '9')
    return(c - '0');

  c &= 0xdf;      /* Convert lower case to upper case */
  if (c >= 'A' && c <= 'F')
    return(c - 'A' + 10);

  return(-1);
}


/**
 * @brief Convert a hexadecimal number string to an integer
 *
 * @param  s - pointer to hexadecimal number string
 * @return number equivalent of number string
 *
**/
uint32_t xxtoi(char *s)
{
  uint32_t value = 0;
  int n;

  while (*s != 0)
  {
    if ((n = xxtod(*s)) == -1)
      break;

    value = (value << 4) + n;
    s++;
  }

  return(value);
}



/**
 * @brief Convert a hexadecimal, binary or decimal number string to an integer
 *
 * @param  s - pointer to number string
 * @return number equivalent of number string
 *
 * Prefix the number with h for hex or b for binary.
 * Defaults to decimal.
 *
**/
uint32_t xbasetoi(char *s)
{
  uint32_t value = 0;
  int base = 10;
  
  if (*s == HEX_NUMBER_DESIGNATOR)
  {
    base = 16;
    s++;
  }
  else if (*s == BINARY_NUMBER_DESIGNATOR)
  {
    base = 2;
    s++;
  }  

  while (*s != 0)
  {
    int n = xxtod(*s);         /* Reads value 0..15 or -1 */
    if (n == -1 || n >= base)
      break;

    value = (value * base) + n;
    s++;
  }

  return(value);
}




/**
 * @brief converts 0..15 to ASCII hex equivalent
 *
 * @param  n - number
 * @return ASCII number equivalent
 *
**/
int ntohex(int n)
{
  if (n > 9)
    n += 7;
  
  return(n + '0');
}


/**
 * @brief Prints a 2 digit zero padded hexadecimal number
 *
 * @param  n - number
 *
**/
void xput02x(uint32_t n)
{
  xputc(ntohex ((n >> 4) & 0xf));
  xputc(ntohex (n & 0xf));
}


/**
 * @brief Prints a 4 digit zero padded hexadecimal number
 *
 * @param  n - number
 *
**/
void xput04x(uint32_t n)
{
  xputc(ntohex((n >> 12) & 0xf));
  xputc(ntohex((n >> 8)  & 0xf));
  xputc(ntohex((n >> 4)  & 0xf));
  xputc(ntohex(n & 0xf));
}


/**
 * @brief Prints the decimal value of a number to a string
 *
 * @param  n - number
 * @param  dst - string to write output to
 *
 * @return none
 *
**/
void xputdstr(char *dst, int32_t n)
{
  char a[12];
  int i = 0, minus;

  if ((minus = (n < 0)))
    n = -n;

  do {
    a[i++] = (n % 10) + '0';
    n /= 10;
  } while (n);
  
  if (minus)
    *dst++ = '-';
  
  while (--i >= 0)
    *dst++ = a[i];
  
  *dst = 0;
}


/**
 * @brief Send a string to the serial port
 *
 * @param  s     - pointer to string
 * @return none
 *
**/
void xputs(char *s)
{
  while (*s)
    xputc(*s++);
}


/**
 * @brief Send a string to the serial port with a maximum length
 *
 * @param  s     - pointer to string
 * @return none
 *
**/
void xputsn(char *s, int len)
{
  while (*s && len--)
    xputc(*s++);
}



/**
 * @brief Copies a string
 *
 * @param  src  - pointer to source string
 * @param  dst  - pointer to destination string
 * @return none
 *
**/
void xstrcpy(char *dst ,char *src)
{
  while (*src)
    *dst++ = *src++;
}


/**
 * @brief Copies a string with a maximum length
 *
 * @param  src  - pointer to source string
 * @param  dst  - pointer to destination string
 * @param  len  - maximum string length
 * @return none
 *
**/
void xstrcpylen(char *dst ,char *src, int len)
{
  while (*src && len--)
    *dst++ = *src++;
}



/**
 * @brief Copies a string null terminated
 *
 * @param  src  - pointer to source string
 * @param  dst  - pointer to destination string
 * @return none
 *
**/
void xstrcpyz(char *dst, char *src)
{
  while (*src)
    *dst++ = *src++;
  
  *dst = 0;
}


/**
 * @brief Copies a string null terminated with a maximun length
 *
 * @param  src  - pointer to source string
 * @param  dst  - pointer to destination string
 * @param  len  - maximum length (allow buffer length - 1 for terminating 0)
 *
 * @return none
 *
**/
void xstrcpyzlen(char *dst ,char *src, int len)
{
  while (*src && len--)
    *dst++ = *src++;
  
  *dst = 0;
}


/**
 * @brief Compares two strings
 *
 * @param  str1  - first string to compare
 * @param  str2  - second string to compare
 * @return 0 if string found
 *
**/
int xstrcmp(const char *str1, const char *str2)
{
	while (*str1 == *str2++) 
  {
		if (*str1++ == '\0')
			return 0;
	}
  
	if (*str1 == '\0')
    return -1;
  
	if (*--str2 == '\0') 
    return 1;
  
	return (unsigned char) *str1 - (unsigned char) *str2;
}



/**
 * @brief Adds a string (src) to another (dst)
 *
 * @param  src  - pointer to source string
 * @param  dst  - pointer to destination string
 * @return none
 *
**/
void xstrcat(char *dst ,char *src)
{
  while (*dst)
    dst++;
  
  while (*src)
    *dst++ = *src++;
  
  *dst = 0;
}


/**
 * @brief Outputs a string as ASCII hex
 *
 * @param  s  pointer to string
 * @return none
 *
**/
void xstrputx(char *s)
{
  while (*s != 0)
		xprintf("%02x ", *s++);
	
	xputc('\n');
}


/* Uncomment to use octal - never used octal! */
//#define I_NEED_OCTAL

#define OUT_BUFFER_LEN  33
#define BIN_BASE         2
#define OCT_BASE         8
#define DEC_BASE        10
#define HEX_BASE        16
/**
 * @brief Low resource print format
 *
 * @param  format_string  pointer to formatting string
 * @param  varargs
 * @return none
 *
 * Formatting characters:
 *   %b - bin - Binary up to 32 bit
 *   %c - chr - Single character
 *   %s - str - String
 *   %d - dec - Decimal number
 *   %o - oct - Octal
 *   %x - hex - Hexadecimal (lower case characters)
 *   %X - HEX - Hexadecimal (upper case characters)
 *   %Any other letter - defaults to '%'
 *
 * Can use field width and padding character '0'
 * i.e. xprintf("%4X %04X", 170, 85); prints "  AA 0055"
 *
 * Todo:
 *	Field width on strings
 *
**/
void xprintf(char *format_string, ...)
{
  va_list ap;
  va_start(ap, format_string);

  int32_t format_character;      /* Formatting character                  */
  char out_buff[OUT_BUFFER_LEN]; /* buffer to build up output number      */
  int32_t width;                 /* Format width                          */
  uint32_t number_base = 0;      /* Number base                           */
  char *print_ptr;               /* Pointer used to print strings         */
  char pad_char;                 /* Formatting pad character (' ' or '0') */
  uint32_t unumber;              /* Unsigned number to output             */
  int32_t count;                 /* Format width counter                  */
  int32_t neg;                   /* Negative flag                         */
  int32_t char_offset;           /* Upper or lower case hex               */

  while (*format_string)
  {
    pad_char = ' ';
    width = 0;
    char_offset = 'a' - '0' - 10;  /* Default to lower case */

    if (*format_string != '%')
    {
      xputc(*format_string++);
      continue;
    }

    format_string++;

  /* Check for pad character */
    if (*format_string == '0')    /* Pad character modifier */
    {
      pad_char = '0';
      format_string++;
    }

  /* Check for and read format width modifier - can be 2 digit */
    if (*format_string >= '0' && *format_string <= '9')
    {
      width = (*format_string++) - '0';
      if (*format_string >= '0' && *format_string <= '9')
      {
        width = (width * 10) + ((*format_string++) - '0');
        if (width > (OUT_BUFFER_LEN - 1))   /* Limit so we don't overflow the output buffer */
          width = (OUT_BUFFER_LEN - 1);
      }
    }

    switch (format_character = *format_string++)
    {
      case 'b':     /* Binary */
        number_base = BIN_BASE;
        goto number_output;

      case 'c':     /* Single character */
        xputc(va_arg (ap, int));
        break;
        
      default: 			/* Copes with "%%" */ 
        xputc('%');
        break;

      case 's':     /* String */
        print_ptr = va_arg(ap, char *);    /* Pointer to the string */
        goto write_string;

      case 'u':     /* Unsigned decimal */
      case 'd':     /* Decimal */
        number_base = DEC_BASE;
        goto number_output;

#ifdef I_NEED_OCTAL
      case 'o':     /* Octal */
        number_base = OCT_BASE;
        goto number_output;
#endif

      case 'X':
        char_offset = 'A' - '0' - 10;
        /* no break */
      case 'x':     /* Hexadecimal */
        number_base = HEX_BASE;

  number_output:
        int32_t number = va_arg(ap, int);

        if ((number < 0) && (format_character == 'd'))
        {
          unumber = (uint32_t)(-number);
          neg = 1;
        }
        else
        {
          unumber = (uint32_t)number;
          neg = 0;
        }

        print_ptr = out_buff + (OUT_BUFFER_LEN - 1);  /* Point to end of array  */
        *print_ptr = 0;                               /* Null terminated string */
        count = 0;

      /* Generate the number string */
        do {
          number = unumber % number_base;
          unumber /= number_base;
          if (number > 9)
            number += char_offset;
          *--print_ptr = number + '0';
          count++;
        } while (unumber);

      /* Add minus sign if needed */
        if ((neg) && (pad_char == ' '))
        {
          *--print_ptr = '-';
          neg = 0;
          count++;
        }

      /* Insert the pad characters */
        while (count++ < width)
          *--print_ptr = pad_char;

      /* Sort out the negative sign for padded numbers */
        if (neg)
          *print_ptr = '-';      /* Overwrites a pad character */

  write_string:
        while (*print_ptr)
          xputc(*print_ptr++);
        break;
    }
  }
  va_end (ap);
}


/**
 * @brief Dump memory contents
 *
 * @param address pointer to data to dump
 * @param length  length of data to dump in bytes
 *
**/
void xdump (uint32_t address, int length)
{
	/* Make sure address starts on a 16 byte boundary and adjust length to suit */   
	length += (address - (address & 0xfffffff0)) - 1;
	address &= 0xfffffff0;
	
	/* Adjust length to be a multiple of 16 */
	length = ((((length - 1) / 16) + 1 ) * 16);
	
  xputs("          0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
  while (length--)
  {
    if ((address & 15) == 0)
			xprintf("\n%08x", address);

		xprintf(" %02x", *(uint8_t *)address++);
  }
  xputc('\n');
}
