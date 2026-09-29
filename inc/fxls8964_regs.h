/**
 * @file		fxls8964_regs.h
 * @brief   Header file for the FXLS8964 accelerometer registers
 *
**/
#ifndef FXLS8964_REGS_H
#define FXLS8964_REGS_H

#define FXLS8964_INT_STATUS         0x00
#define FXLS8964_TEMP_OUT           0x01
#define FXLS8964_VECM_LSB           0x02
#define FXLS8964_VECM_MSB           0x03
#define FXLS8964_OUT_X_LSB          0x04
#define FXLS8964_OUT_X_MSB          0x05
#define FXLS8964_OUT_Y_LSB          0x06
#define FXLS8964_OUT_Y_MSB          0x07
#define FXLS8964_OUT_Z_LSB          0x08
#define FXLS8964_OUT_Z_MSB          0x09
#define FXLS8964_BUF_STATUS         0x0B
#define FXLS8964_BUF_X_LSB          0x0C
#define FXLS8964_BUF_X_MSB          0x0D
#define FXLS8964_BUF_Y_LSB          0x0E
#define FXLS8964_BUF_Y_MSB          0x0F
#define FXLS8964_BUF_Z_LSB          0x10
#define FXLS8964_BUF_Z_MSB          0x11
#define FXLS8964_PROD_REV           0x12
#define FXLS8964_WHO_AM_I           0x13
#define FXLS8964_SYS_MODE           0x14
#define FXLS8964_SENSOR_CONFIG1     0x15
#define FXLS8964_SENSOR_CONFIG2     0x16
#define FXLS8964_SENSOR_CONFIG3     0x17
#define FXLS8964_SENSOR_CONFIG4     0x18
#define FXLS8964_SENSOR_CONFIG5     0x19
#define FXLS8964_WAKE_IDLE_LSB      0x1A
#define FXLS8964_WAKE_IDLE_MSB      0x1B
#define FXLS8964_SLEEP_IDLE_LSB     0x1C
#define FXLS8964_SLEEP_IDLE_MSB     0x1D
#define FXLS8964_ASLP_COUNT_LSB     0x1E
#define FXLS8964_ASLP_COUNT_MSB     0x1F
#define FXLS8964_INT_EN             0x20
#define FXLS8964_INT_PIN_SEL        0x21
#define FXLS8964_OFF_X              0x22
#define FXLS8964_OFF_Y              0x23
#define FXLS8964_OFF_Z              0x24
#define FXLS8964_BUF_CONFIG1        0x26
#define FXLS8964_BUF_CONFIG2        0x27
#define FXLS8964_ORIENT_STATUS      0x28
#define FXLS8964_ORIENT_CONFIG      0x29
#define FXLS8964_ORIENT_DBCOUNT     0x2A
#define FXLS8964_ORIENT_BF_ZCOMP    0x2B
#define FXLS8964_ORIENT_THS_REG     0x2C
#define FXLS8964_SDCD_INT_SRC1      0x2D
#define FXLS8964_SDCD_INT_SRC2      0x2E
#define FXLS8964_SDCD_CONFIG1       0x2F
#define FXLS8964_SDCD_CONFIG2       0x30
#define FXLS8964_SDCD_OT_DBCNT      0x31
#define FXLS8964_SDCD_WT_DBCNT      0x32
#define FXLS8964_SDCD_LTHS_LSB      0x33
#define FXLS8964_SDCD_LTHS_MSB      0x34
#define FXLS8964_SDCD_UTHS_LSB      0x35
#define FXLS8964_SDCD_UTHS_MSB      0x36
#define FXLS8964_SELF_TEST_CONFIG1  0x37
#define FXLS8964_SELF_TEST_CONFIG2  0x38


#endif
