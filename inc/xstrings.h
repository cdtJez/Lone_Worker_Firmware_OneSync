/**
 * @file		xstring.h
 * @brief   Header file for xstring.c
 *
**/
#ifndef XSTRING_H
#define XSTRING_H

/* Define the character output function fputc */
#define xputc usart2_tx_char

int xtoupper(int c);
int xisdigit(char c);
int32_t xatoi(char *s);
void xput02x(uint32_t n);
void xput04x(uint32_t n);
void xputs(char *s);
void xputsn(char *s, int len);
void xstrcpy(char *dst ,char *src);
void xstrcpylen(char *dst ,char *src, int len);
void xputdstr(char *dst, int32_t n);
void xstrcpyz(char *dst ,char *src);
void xstrcpyzlen(char *dst ,char *src, int len);
int xstrcmp(const char *str1, const char *str2);
void xstrcat(char *dst ,char *src);
uint32_t xxtoi(char *s);
uint32_t xbasetoi(char *s);
void xstrputx(char *s);
void xprintf(char *buff, ...);
void xdump(uint32_t address, int len);

#endif