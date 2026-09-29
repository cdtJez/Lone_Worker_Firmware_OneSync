/**
 * @file    rfm69hw_regs.h
 * @brief   Header file for rfm69.c registers
 *
**/
#ifndef RFM69HW_REGS_H
#define RFM69HW_REGS_H

/* RFM69 registers */
#define RegFifo               0x00  /* FIFO data input/output                                        */
#define RegOpMode             0x01  /* Operating mode of the transceiver                             */  
#define RegDataModul          0x02  /* Data operation mode and Modulation settings                   */  
#define RegBitrateMsb         0x03  /* Bit Rate setting, Most Significant Bits                       */  
#define RegBitrateLsb         0x04  /* Bit Rate setting, Least Significant Bits                      */  
#define RegFdevMsb            0x05  /* Frequency Deviation setting, Most Significant Bits            */
#define RegFdevLsb            0x06  /* Frequency Deviation setting, Least Significant Bits           */  
#define RegFrfMsb             0x07  /* RF Carrier Frequency, Most Significant Bits                   */
#define RegFrfMid             0x08  /* RF Carrier Frequency, Intermediate Bits                       */
#define RegFrfLsb             0x09  /* RF Carrier Frequency, Least Significant Bits                  */
#define RegOsc1               0x0A  /* RC Oscillators Settings                                       */
#define RegAfcCtrl            0x0B  /* AFC control in low modulation index situations                */
#define Reserved0C            0x0C  /* Reserved                                                      */
#define RegListen1            0x0D  /* Listen Mode settings                                          */
#define RegListen2            0x0E  /* Listen Mode Idle duration                                     */  
#define RegListen3            0x0F  /* Listen Mode Rx duration                                       */  
#define RegVersion            0x10  /* Version code of chip                                          */  
#define RegPaLevel            0x11  /* PA selection and Output Power control                         */  
#define RegPaRamp             0x12  /* Control of the PA ramp time in FSK mode                       */    
#define RegOcp                0x13  /* Over Current Protection control                               */  
#define Reserved14_17         0x14  /* Reserved 0x14 - 0x17                                          */ 
#define RegLna                0x18  /* LNA settings                                                  */
#define RegRxBw               0x19  /* Channel Filter BW Control                                     */
#define RegAfcBw              0x1A  /* Channel Filter BW control during the AFC routine              */
#define RegOokPeak            0x1B  /* OOK demodulator selection and control in peak mode            */
#define RegOokAvg             0x1C  /* Average threshold control of the OOK demodulator              */
#define RegOokFix             0x1D  /* Fixed threshold control of the OOK demodulator                */
#define RegAfcFei             0x1E  /* AFC and FEI control and status                                */
#define RegAfcMsb             0x1F  /* MSB of the frequency correction of the AFC                    */
#define RegAfcLsb             0x20  /* LSB of the frequency correction of the AFC                    */
#define RegFeiMsb             0x21  /* MSB of the calculated frequency error                         */
#define RegFeiLsb             0x22  /* LSB of the calculated frequency error                         */
#define RegRssiConfig         0x23  /* RSSI-related settings                                         */
#define RegRssiValue          0x24  /* RSSI value in dBm                                             */
#define RegDioMapping1        0x25  /* Mapping of pins DIO0 to DIO3                                  */
#define RegDioMapping2        0x26  /* Mapping of pins DIO4 and DIO5, ClkOut frequency               */
#define RegIrqFlags1          0x27  /* Status register: PLL Lock state, Timeout, RSSI > Threshold... */
#define RegIrqFlags2          0x28  /* Status register: FIFO handling flags...                       */
#define RegRssiThresh         0x29  /* RSSI Threshold control                                        */
#define RegRxTimeout1         0x2A  /* Timeout duration between Rx request and RSSI detection        */
#define RegRxTimeout2         0x2B  /* Timeout duration between RSSI detection and PayloadReady      */
#define RegPreambleMsb        0x2C  /* Preamble length, MSB                                          */
#define RegPreambleLsb        0x2D  /* Preamble length, LSB                                          */
#define RegSyncConfig         0x2E  /* Sync Word Recognition control                                 */
#define RegSyncValue1         0x2F  /* Sync Word byte 1 MSB                                          */
#define RegSyncValue2         0x30  /* Sync Word byte 2                                              */
#define RegSyncValue3         0x31  /* Sync Word byte 3                                              */
#define RegSyncValue4         0x32  /* Sync Word byte 4                                              */
#define RegSyncValue5         0x33  /* Sync Word byte 5                                              */
#define RegSyncValue6         0x34  /* Sync Word byte 6                                              */
#define RegSyncValue7         0x35  /* Sync Word byte 7                                              */
#define RegSyncValue8         0x36  /* Sync Word byte 8 LSB                                          */
#define RegPacketConfig1      0x37  /* Packet mode settings                                          */
#define RegPayloadLength      0x38  /* Payload length setting                                        */
#define RegNodeAdrs           0x39  /* Node address                                                  */
#define RegBroadcastAdrs      0x3A  /* Broadcast address                                             */
#define RegAutoModes          0x3B  /* Auto modes settings                                           */
#define RegFifoThresh         0x3C  /* Fifo threshold, Tx start condition                            */
#define RegPacketConfig2      0x3D  /* Packet mode settings                                          */
#define RegAesKey1            0x3E  /* Cypher key byte 1 MSB                                         */
#define RegAesKey2            0x3F  /* Cypher key byte 2                                             */
#define RegAesKey3            0x40  /* Cypher key byte 3                                             */
#define RegAesKey4            0x41  /* Cypher key byte 4                                             */
#define RegAesKey5            0x42  /* Cypher key byte 5                                             */
#define RegAesKey6            0x43  /* Cypher key byte 6                                             */
#define RegAesKey7            0x44  /* Cypher key byte 7                                             */
#define RegAesKey8            0x45  /* Cypher key byte 8                                             */
#define RegAesKey9            0x46  /* Cypher key byte 9                                             */
#define RegAesKey10           0x47  /* Cypher key byte 10                                            */
#define RegAesKey11           0x48  /* Cypher key byte 11                                            */
#define RegAesKey12           0x49  /* Cypher key byte 12                                            */
#define RegAesKey13           0x4A  /* Cypher key byte 13                                            */
#define RegAesKey14           0x4B  /* Cypher key byte 14                                            */
#define RegAesKey15           0x4C  /* Cypher key byte 15                                            */
#define RegAesKey16           0x4D  /* Cypher key byte 16 LSB                                        */
#define RegTemp1              0x4E  /* Temperature Sensor control                                    */
#define RegTemp2              0x4F  /* Temperature readout                                           */
#define RegTestLna            0x58  /* Sensitivity boost                                             */
#define RegTestPa1            0x5A  /* High Power PA settings                                        */
#define RegTestPa2            0x5C  /* High Power PA settings                                        */
#define RegTestDagc           0x6F  /* Fading Margin Improvement                                     */
#define RegTestAfc            0x71  /* AFC offset for low modulation index AFC                       */

/* OPMODE register bit definitions */
#define RFM69HW_OPMODE_SEQUENCER_OFF                  (1 << 7)
#define RFM69HW_OPMODE_SEQUENCER_ON                   0
#define RFM69HW_OPMODE_LISTEN_ON                      (1 << 6)
#define RFM69HW_OPMODE_LISTEN_OFF                     0
#define RFM69HW_OPMODE_LISTENABORT                    0x20 

/* OPMODE register mode values */
#define RFM69HW_OPMODE_SLEEP                          0x00        /* XTAL OFF */
#define RFM69HW_OPMODE_STANDBY                        0x04        /* XTAL ON  */
#define RFM69HW_OPMODE_SYNTH                          0x08        /* PLL ON   */
#define RFM69HW_OPMODE_TX                             0x0C        /* TX MODE  */
#define RFM69HW_OPMODE_RX                             0x10        /* RX MODE  */

/* DataModul register */
#define RFM69HW_DATAMODUL_DATAMODE_PACKET             0x00  /* Default */
#define RFM69HW_DATAMODUL_DATAMODE_CONTINUOUS         0x40
#define RFM69HW_DATAMODUL_DATAMODE_CONTINUOUSNOBSYNC  0x60

#define RFM69HW_DATAMODUL_MODULATIONTYPE_FSK          0x00  /* Default */
#define RFM69HW_DATAMODUL_MODULATIONTYPE_OOK          0x08

#define RFM69HW_DATAMODUL_MODULATIONSHAPING_00        0x00  /* Default */
#define RFM69HW_DATAMODUL_MODULATIONSHAPING_01        0x01
#define RFM69HW_DATAMODUL_MODULATIONSHAPING_10        0x02
#define RFM69HW_DATAMODUL_MODULATIONSHAPING_11        0x03

/* RF bitrate */
#define RFM69HW_BITRATEMSB_1200                       0x68  /* Default */
#define RFM69HW_BITRATELSB_1200                       0x2B  /* Default */
#define RFM69HW_BITRATEMSB_2400                       0x34
#define RFM69HW_BITRATELSB_2400                       0x15
#define RFM69HW_BITRATEMSB_4800                       0x1A
#define RFM69HW_BITRATELSB_4800                       0x0B
#define RFM69HW_BITRATEMSB_9600                       0x0D
#define RFM69HW_BITRATELSB_9600                       0x05
#define RFM69HW_BITRATEMSB_19200                      0x06
#define RFM69HW_BITRATELSB_19200                      0x83
#define RFM69HW_BITRATEMSB_38400                      0x03
#define RFM69HW_BITRATELSB_38400                      0x41

/* Frequency deviation */
#define RFM69HW_FDEVMSB_2000                          0x00
#define RFM69HW_FDEVLSB_2000                          0x21
#define RFM69HW_FDEVMSB_5000                          0x00  /* Default */
#define RFM69HW_FDEVLSB_5000                          0x52  /* Default */

/* RegRxBw */
#define RFM69HW_RXBW_DCCFREQ_000                      0x00
#define RFM69HW_RXBW_DCCFREQ_001                      0x20
#define RFM69HW_RXBW_DCCFREQ_010                      0x40  /* Recommended default */
#define RFM69HW_RXBW_DCCFREQ_011                      0x60
#define RFM69HW_RXBW_DCCFREQ_100                      0x80  /* Reset value */
#define RFM69HW_RXBW_DCCFREQ_101                      0xA0
#define RFM69HW_RXBW_DCCFREQ_110                      0xC0
#define RFM69HW_RXBW_DCCFREQ_111                      0xE0

#define RFM69HW_RXBW_MANT_16                          0x00  /* Reset value */
#define RFM69HW_RXBW_MANT_20                          0x08
#define RFM69HW_RXBW_MANT_24                          0x10  /* Recommended default */

#define RFM69HW_RXBW_EXP_0                            0x00
#define RFM69HW_RXBW_EXP_1                            0x01
#define RFM69HW_RXBW_EXP_2                            0x02
#define RFM69HW_RXBW_EXP_3                            0x03
#define RFM69HW_RXBW_EXP_4                            0x04
#define RFM69HW_RXBW_EXP_5                            0x05  /* Recommended default */
#define RFM69HW_RXBW_EXP_6                            0x06  /* Reset value */
#define RFM69HW_RXBW_EXP_7                            0x07

/* RegRssiConfig */
#define RFM69HW_RSSI_START														0x01
#define RFM69HW_RSSI_DONE															0x02

/* RegDioMapping1 */
#define RFM69HW_DIOMAPPING1_DIO0_00                   0x00  /* Default */
#define RFM69HW_DIOMAPPING1_DIO0_01                   0x40
#define RFM69HW_DIOMAPPING1_DIO0_10                   0x80
#define RFM69HW_DIOMAPPING1_DIO0_11                   0xC0

#define RFM69HW_DIOMAPPING1_DIO1_00                   0x00  /* Default */
#define RFM69HW_DIOMAPPING1_DIO1_01                   0x10
#define RFM69HW_DIOMAPPING1_DIO1_10                   0x20
#define RFM69HW_DIOMAPPING1_DIO1_11                   0x30

#define RFM69HW_DIOMAPPING1_DIO2_00                   0x00  /* Default */
#define RFM69HW_DIOMAPPING1_DIO2_01                   0x04
#define RFM69HW_DIOMAPPING1_DIO2_10                   0x08
#define RFM69HW_DIOMAPPING1_DIO2_11                   0x0C

#define RFM69HW_DIOMAPPING1_DIO3_00                   0x00  /* Default */
#define RFM69HW_DIOMAPPING1_DIO3_01                   0x01
#define RFM69HW_DIOMAPPING1_DIO3_10                   0x02
#define RFM69HW_DIOMAPPING1_DIO3_11                   0x03

/* RegDioMapping2 */
#define RFM69HW_DIOMAPPING2_DIO4_00                   0x00  /* Default */
#define RFM69HW_DIOMAPPING2_DIO4_01                   0x40
#define RFM69HW_DIOMAPPING2_DIO4_10                   0x80
#define RFM69HW_DIOMAPPING2_DIO4_11                   0xC0

#define RFM69HW_DIOMAPPING2_DIO5_00                   0x00  /* Default */
#define RFM69HW_DIOMAPPING2_DIO5_01                   0x10
#define RFM69HW_DIOMAPPING2_DIO5_10                   0x20
#define RFM69HW_DIOMAPPING2_DIO5_11                   0x30

#define RFM69HW_DIOMAPPING2_CLKOUT_32MHZ              0x00
#define RFM69HW_DIOMAPPING2_CLKOUT_16MHZ              0x01
#define RFM69HW_DIOMAPPING2_CLKOUT_8MHZ               0x02
#define RFM69HW_DIOMAPPING2_CLKOUT_4MHZ               0x03
#define RFM69HW_DIOMAPPING2_CLKOUT_2MHZ               0x04
#define RFM69HW_DIOMAPPING2_CLKOUT_1MHZ               0x05  /* Reset value */
#define RFM69HW_DIOMAPPING2_CLKOUT_RC                 0x06
#define RFM69HW_DIOMAPPING2_CLKOUT_OFF                0x07  /* Recommended default */

/* RegIrqFlags1 */
#define RFM69HW_IRQFLAGS1_MODEREADY                   0x80
#define RFM69HW_IRQFLAGS1_RXREADY                     0x40
#define RFM69HW_IRQFLAGS1_TXREADY                     0x20
#define RFM69HW_IRQFLAGS1_PLLLOCK                     0x10
#define RFM69HW_IRQFLAGS1_RSSI                        0x08
#define RFM69HW_IRQFLAGS1_TIMEOUT                     0x04
#define RFM69HW_IRQFLAGS1_AUTOMODE                    0x02
#define RFM69HW_IRQFLAGS1_SYNCADDRESSMATCH            0x01

/* RegIrqFlags2 */
#define RFM69HW_IRQFLAGS2_FIFOFULL                    0x80
#define RFM69HW_IRQFLAGS2_FIFONOTEMPTY                0x40
#define RFM69HW_IRQFLAGS2_FIFOLEVEL                   0x20
#define RFM69HW_IRQFLAGS2_FIFOOVERRUN                 0x10
#define RFM69HW_IRQFLAGS2_PACKETSENT                  0x08
#define RFM69HW_IRQFLAGS2_PAYLOADREADY                0x04
#define RFM69HW_IRQFLAGS2_CRCOK                       0x02
#define RFM69HW_IRQFLAGS2_LOWBAT                      0x01  /* not present on RFM69/SX1231 */

// RegPreamble
#define RFM69HW_PREAMBLESIZE_MSB_VALUE         				0x00
#define RFM69HW_PREAMBLESIZE_LSB_VALUE         				0x03

/* RegSyncConfig */
#define RFM69HW_SYNC_ON                               0x80  /* Default */
#define RFM69HW_SYNC_OFF                              0x00

#define RFM69HW_SYNC_FIFOFILL_AUTO                    0x00  /* Default -- when sync interrupt occurs */
#define RFM69HW_SYNC_FIFOFILL_MANUAL                  0x40

#define RFM69HW_SYNC_SIZE_1                           0x00
#define RFM69HW_SYNC_SIZE_2                           0x08
#define RFM69HW_SYNC_SIZE_3                           0x10
#define RFM69HW_SYNC_SIZE_4                           0x18  /* Default */
#define RFM69HW_SYNC_SIZE_5                           0x20
#define RFM69HW_SYNC_SIZE_6                           0x28
#define RFM69HW_SYNC_SIZE_7                           0x30
#define RFM69HW_SYNC_SIZE_8                           0x38

#define RFM69HW_SYNC_TOL_0                            0x00  /* Default */
#define RFM69HW_SYNC_TOL_1                            0x01
#define RFM69HW_SYNC_TOL_2                            0x02
#define RFM69HW_SYNC_TOL_3                            0x03
#define RFM69HW_SYNC_TOL_4                            0x04
#define RFM69HW_SYNC_TOL_5                            0x05
#define RFM69HW_SYNC_TOL_6                            0x06
#define RFM69HW_SYNC_TOL_7                            0x07

/* RegPacketConfig1 */
#define RFM69HW_PACKET1_FORMAT_FIXED                  0x00  /* Default */
#define RFM69HW_PACKET1_FORMAT_VARIABLE               0x80

#define RFM69HW_PACKET1_DCFREE_OFF                    0x00  /* Default */
#define RFM69HW_PACKET1_DCFREE_MANCHESTER             0x20
#define RFM69HW_PACKET1_DCFREE_WHITENING              0x40

#define RFM69HW_PACKET1_CRC_ON                        0x10  /* Default */
#define RFM69HW_PACKET1_CRC_OFF                       0x00

#define RFM69HW_PACKET1_CRCAUTOCLEAR_ON               0x00  /* Default */
#define RFM69HW_PACKET1_CRCAUTOCLEAR_OFF              0x08

#define RFM69HW_PACKET1_ADRSFILTERING_OFF             0x00  /* Default */
#define RFM69HW_PACKET1_ADRSFILTERING_NODE            0x02
#define RFM69HW_PACKET1_ADRSFILTERING_NODEBROADCAST   0x04

/* RegFifoThresh */
#define RFM69HW_FIFOTHRESH_TXSTART_FIFOTHRESH         0x00  /* Reset value */
#define RFM69HW_FIFOTHRESH_TXSTART_FIFONOTEMPTY       0x80  /* Recommended default */

#define RFM69HW_FIFOTHRESH_VALUE                      0x0F  /* Default */

/* RegPacketConfig2 */
#define RFM69HW_PACKET2_RXRESTARTDELAY_1BIT           0x00  /* Default */
#define RFM69HW_PACKET2_RXRESTARTDELAY_2BITS          0x10
#define RFM69HW_PACKET2_RXRESTARTDELAY_4BITS          0x20
#define RFM69HW_PACKET2_RXRESTARTDELAY_8BITS          0x30
#define RFM69HW_PACKET2_RXRESTARTDELAY_16BITS         0x40
#define RFM69HW_PACKET2_RXRESTARTDELAY_32BITS         0x50
#define RFM69HW_PACKET2_RXRESTARTDELAY_64BITS         0x60
#define RFM69HW_PACKET2_RXRESTARTDELAY_128BITS        0x70
#define RFM69HW_PACKET2_RXRESTARTDELAY_256BITS        0x80
#define RFM69HW_PACKET2_RXRESTARTDELAY_512BITS        0x90
#define RFM69HW_PACKET2_RXRESTARTDELAY_1024BITS       0xA0
#define RFM69HW_PACKET2_RXRESTARTDELAY_2048BITS       0xB0
#define RFM69HW_PACKET2_RXRESTARTDELAY_NONE           0xC0
#define RFM69HW_PACKET2_RXRESTART                     0x04

#define RFM69HW_PACKET2_AUTORXRESTART_ON              0x02  /* Default */
#define RFM69HW_PACKET2_AUTORXRESTART_OFF             0x00

#define RFM69HW_PACKET2_AES_ON                        0x01
#define RFM69HW_PACKET2_AES_OFF                       0x00  /* Default */

/* RegTemp1 */
#define RFM69HW_TEMP_MEAS_START												0x08
#define RFM69HW_TEMP_MEAS_RUNNING											0x04

// RegTestDagc
#define RFM69HW_DAGC_NORMAL                           0x00  // Reset value
#define RFM69HW_DAGC_IMPROVED_LOWBETA1                0x20
#define RFM69HW_DAGC_IMPROVED_LOWBETA0                0x30  // Recommended default

// RegPaLevel
#define RFM69HW_RF_PALEVEL_PA0_ON                     0x80  // Default
#define RFM69HW_RF_PALEVEL_PA0_OFF                    0x00
#define RFM69HW_RF_PALEVEL_PA1_ON                     0x40
#define RFM69HW_RF_PALEVEL_PA1_OFF                    0x00  // Default
#define RFM69HW_RF_PALEVEL_PA2_ON                     0x20
#define RFM69HW_RF_PALEVEL_PA2_OFF                    0x00  // Default

#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_00000          0x00
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_00001          0x01
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_00010          0x02
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_00011          0x03
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_00100          0x04
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_00101          0x05
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_00110          0x06
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_00111          0x07
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_01000          0x08
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_01001          0x09
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_01010          0x0A
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_01011          0x0B
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_01100          0x0C
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_01101          0x0D
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_01110          0x0E
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_01111          0x0F
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_10000          0x10
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_10001          0x11
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_10010          0x12
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_10011          0x13
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_10100          0x14
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_10101          0x15
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_10110          0x16
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_10111          0x17
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_11000          0x18
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_11001          0x19
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_11010          0x1A
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_11011          0x1B
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_11100          0x1C
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_11101          0x1D
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_11110          0x1E
#define RFM69HW_RF_PALEVEL_OUTPUTPOWER_11111          0x1F  // Default

#endif