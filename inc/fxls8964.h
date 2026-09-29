/**
 * @file		fxls8964.h
 * @brief   Header file for the FXLS8964 accelerometer
 *
**/
#ifndef FXLS8964_H
#define FXLS8964_H

void fxls8964_init(void);
void fxls8964_on(void);
void fxls8964_off(void);
int fxls8964_read_x(void);
void fxls8964_tilt_check(void);

#endif
