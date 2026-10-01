/**
 *  Library for RFM69HW
 *
 *----------------------------------------------------------------------------------------------
 *
 *  Data sent as a fixed/ length packet:
 *
 *  1   Preamble 1
 *  2   Preamble 2
 *  3   Preamble 3
 *  4   SystemID       MDH system ID              (Sync byte 1)
 *  5   NetworkID      ID for installed network   (Sync byte 2)
 *  6   DeviceID       Address of sending device  (Payload byte 1)
 *  7   MessageType    Message                    (Payload byte 2)
 *  8   RepeatCount    Incremented each repeat    (Payload byte 3)
 *  9   CRC 1          System generated CRC
 *  10  CRC 2          System generated CRC
 *
 *  Message types:
 *
 *  Message types define in messages.h
 *
 *----------------------------------------------------------------------------------------------
 *
 *  Messages are sent at 1200bps.
 *
 *  DeviceID is 0 for the control panel and 255 for the default value.
 *
 *----------------------------------------------------------------------------------------------
**/
#include "main.h"

/* Pin assignments */
#define DIO0_PIN          12
#define RESET_PIN         11

/* MDH network ID - all MDH networks use this */
#define MDH_ID            0x7b

/* RFM69HW Internal clock frequency [Hz] */
#define RFM69_FXO         32000000

/* Define the carrier frequency [Hz] */
#define FREQUENCY         869500000uLL
#define FREQ_VAL          (((unsigned long long)FREQUENCY * 256uLL) / 15625uLL)
#define FREQ_MSB          (uint8_t)(FREQ_VAL >> 16)
#define FREQ_MID          (uint8_t)(FREQ_VAL >> 8)
#define FREQ_LSB          (uint8_t)(FREQ_VAL)

/* Frequency deviation [Hz] */
#define FREQ_DEV          20000
#define FREQ_DEV_VAL      ((FREQ_DEV * 256) / 15626)
#define FREQ_DEV_VAL_MSB  (uint8_t)(FREQ_DEV_VAL >> 8)
#define FREQ_DEV_VAL_LSB  (uint8_t)(FREQ_DEV_VAL)

/* Data rate [bps] */
#define DATA_RATE         1200
#define DATA_RATE_VAL     (RFM69_FXO / DATA_RATE)
#define DATA_RATE_VAL_MSB (uint8_t)(DATA_RATE_VAL >> 8)
#define DATA_RATE_VAL_LSB (uint8_t)(DATA_RATE_VAL)


const uint8_t rfm69hw_config[][2] =
{
  { RegOpMode,        RFM69HW_OPMODE_STANDBY },
  { RegDataModul,     0x00 },                               /* Packet mode, FSK, no shaping */
  { RegBitrateMsb,    DATA_RATE_VAL_MSB },
  { RegBitrateLsb,    DATA_RATE_VAL_LSB },
  { RegFdevMsb,       FREQ_DEV_VAL_MSB },
  { RegFdevLsb,       FREQ_DEV_VAL_LSB },
  { RegFrfMsb,        FREQ_MSB },
  { RegFrfMid,        FREQ_MID },
  { RegFrfLsb,        FREQ_LSB },
  { RegLna,           0x88 },                               /* 200 Ohm impedance, gain set by AGC loop */
  { RegRxBw,          0x4C },                               /* 25 kHz */
  { RegDioMapping1,   RFM69HW_DIOMAPPING1_DIO0_01 },        /* DIO0 signals packet ready */
  { RegPreambleMsb,   0x00 },                               /* 3 bytes preamble */
  { RegPreambleLsb,   0x03 },
  { RegSyncConfig,    0x90 },                               /* Enable sync word, 1 bytes sync word */
//  { RegSyncConfig,    0x98 },                               /* Enable sync word, 2 bytes sync word */
  { RegSyncValue1,    MDH_ID },                             /* ID for all MDH product networks */
//  { RegSyncValue2,    0x48 },                               /* NetworkID for local network - overwritten by value from parameters */
  { RegPacketConfig1, 0x10 },                               /* Fixed length, CRC on, No packet address matching! */
  { RegPayloadLength, 0x03 },                               /* 3 bytes payload */
  { RegFifoThresh,    RFM69HW_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RFM69HW_FIFOTHRESH_VALUE }, /* TxStart on FifoNotEmpty, 15 bytes FifoLevel */
  { RegTestLna,       0x1B },                               /* Normal sensitivity mode */
  { RegTestDagc,      0x30 }                                /* Improved margin, use if AfcLowBetaOn=0 (default) */
};


/**
 * @brief Write a data byte to the RFM69HW
 *
 * @param address Address of register
 * @param data    Data byte to be written
 *
 * @return None
 *
**/
void rfm69hw_write_reg(uint8_t address, uint8_t data)
{
  spi_select();
  spi_xfer(address | 0x80);
  spi_xfer(data);
  spi_deselect();
}


/**
 * @brief Read a data byte from the RFM69HW
 *
 * @param address Address of register
 *
 * @return data
 *
**/
uint8_t rfm69hw_read_reg(uint8_t address)
{
  spi_select();
  spi_xfer(address & 0x7f);
  uint8_t ret = spi_xfer(0);
  spi_deselect();
  return(ret);
}


/**
 * @brief RFM69HW hardware initialiser
 *
 * @param none
 *
 * @return none
 *
**/
void rfm69hw_init(void)
{
/* RFM69HW is on SPI1 */
  spi_init();

/* PA12 is an input for DIO0 - payload ready signal, PA11 is RESET */
//  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;                            /* Enable clock to GPIOA - done in main */
  MODIFY_REG(GPIOA->MODER, (3 << 24) | (3 << 22), 0);     /* PA11 & PA12 are inputs */

/* Wait for the RFM69HW to come out of reset */
  while (GPIOA->IDR & (1 << RESET_PIN)) {};         /* Wait for RESET pin to go low */

  delay_us (10000);
  
/* Reset is now an input on the RFM69HW so set PA11 to output and low */
  MODIFY_REG(GPIOA->MODER, (3 << 22), (1 << 22));
  GPIOA->ODR &= ~(1 << RESET_PIN);

/* Initialise the RFM69HW */
  for (int i = 0; i < (sizeof(rfm69hw_config) / 2); i++)
    rfm69hw_write_reg(rfm69hw_config[i][0], rfm69hw_config[i][1]);

/* Load the network ID from the parameters */
//  rfm69hw_write_reg(RegSyncValue2, params.network_id);
  
/* Set the over current at 120mA */
  rfm69hw_write_reg(RegOcp, 0x1f);

/* Set the Tx power level */
  rfm69hw_tx_power(params.tx_power);

/* Receive mode */
  rfm69hw_set_mode(RF69_MODE_SLEEP);
}


/**
 * @brief Test if a payload is ready by seeing if DIO0 pin high
 *
**/
int rfm69hw_payload_ready(void)
{
  return(GPIOA->IDR & (1 << DIO0_PIN));
}


/**
 * @brief Set the data bitrate
 *
**/
void rfm69hw_set_bitrate(int bitrate)
{
  rfm69hw_set_mode(RF69_MODE_STANDBY);

/* Calculate bitrate value */
  bitrate = RFM69_FXO / bitrate;

/* Write the new value to the RFM69HW */
  rfm69hw_write_reg(RegBitrateMsb, bitrate >> 8);
  rfm69hw_write_reg(RegBitrateLsb, bitrate);
}


/**
 * @brief RFM69HW set high power mode
 *
**/
void rfm69hw_set_high_power(void)
{
  rfm69hw_write_reg(RegTestPa1, 0x5D);
  rfm69hw_write_reg(RegTestPa2, 0x7C);
}


/**
 * @brief RFM69HW clear high power mode
 *
**/
void rfm69hw_clear_high_power(void)
{
  rfm69hw_write_reg(RegTestPa1, 0x55);
  rfm69hw_write_reg(RegTestPa2, 0x70);
}


/**
 * @brief RFM69HW set operating mode
 *
 * @param mode to set
 *
**/
void rfm69hw_set_mode(enum rf69_mode mode)
{
  switch(mode)
  {
    case RF69_MODE_TX:
      rfm69hw_write_reg(RegOpMode, (rfm69hw_read_reg(RegOpMode) & 0xE3) | RFM69HW_OPMODE_TX);
      rfm69hw_set_high_power();
      break;

    case RF69_MODE_RX:
      rfm69hw_write_reg(RegOpMode, (rfm69hw_read_reg(RegOpMode) & 0xE3) | RFM69HW_OPMODE_RX);
      rfm69hw_clear_high_power();
      break;

    case RF69_MODE_SYNTH:
      rfm69hw_write_reg(RegOpMode, (rfm69hw_read_reg(RegOpMode) & 0xE3) | RFM69HW_OPMODE_SYNTH);
      break;

    case RF69_MODE_STANDBY:
      rfm69hw_write_reg(RegOpMode, (rfm69hw_read_reg(RegOpMode) & 0xE3) | RFM69HW_OPMODE_STANDBY);
      break;

    case RF69_MODE_SLEEP:
      rfm69hw_write_reg(RegOpMode, (rfm69hw_read_reg(RegOpMode) & 0xE3) | RFM69HW_OPMODE_SLEEP);
      break;

    default:
      break;
  }

/* Wait for the mode change to complete */
  while (((rfm69hw_read_reg(RegIrqFlags1) & RFM69HW_IRQFLAGS1_MODEREADY) == 0)) {};
}


/**
 * @brief RFM69HW read the RSSI
 *
 * @param none
 *
**/
int32_t rfm69hw_read_rssi(void)
{
  return((-rfm69hw_read_reg(RegRssiValue)) / 2);
}


/**
 * @brief RFM69HW read the temperature in K
 *
 * @param none
 *
 * @return Temperature in K
 *
**/
int32_t rfm32hw_temperature(void)
{
  rfm69hw_set_mode(RF69_MODE_STANDBY);

  rfm69hw_write_reg(RegTemp1, RFM69HW_TEMP_MEAS_START);
  while ((rfm69hw_read_reg(RegTemp1) & RFM69HW_TEMP_MEAS_RUNNING)) {};

/* Add a calibration coefficient and C -> K conversion */
  int temp = -rfm69hw_read_reg(RegTemp2) + 157 + 273;

  rfm69hw_set_mode(RF69_MODE_RX);

  return (temp);
}


/**
 * @brief Dump the RFM69HW registers in a 16 byte wide block
 *
 * @param none
 *
**/
void rfm69hw_register_dump(void)
{
#ifdef DEBUG_REGS
  for (int i = 0; i < 0x71; i++)
  {
    uint8_t r = rfm69hw_read_reg (i);
    xprintf("0x%02X = 0x%02X %03d (%08b) %s\n", i, r, r, r, regs_lookup(i));
  }
#else
  for (int i = 0; i < 0x71; i++)
    xprintf("[0x%02X] = 0x%02X\n", i, rfm69hw_read_reg(i));
#endif
}


/**
 * @brief RFM69HW packet send
 *
 * @param mes  Packet structure
 *
**/
void rfm69hw_packet_send(struct packet_t *mes)
{
  LED_ON;
  
  rfm69hw_set_mode(RF69_MODE_STANDBY);
  rfm69hw_write_reg(RegIrqFlags2, RFM69HW_IRQFLAGS2_FIFOOVERRUN);

/* Transmission starts with the first byte written to the FIFO */
  spi_select();
  spi_xfer(RegFifo | 0x80);
  spi_xfer(mes->network_id);
  spi_xfer(mes->sending_device);
  spi_xfer(mes->message_type);
  spi_deselect();

  rfm69hw_set_mode(RF69_MODE_TX);

/* Wait for the packet to be sent - this takes about 80ms */
  while ((rfm69hw_read_reg (RegIrqFlags2) & RFM69HW_IRQFLAGS2_PACKETSENT) == 0x00) 
    delay_us(10000);

  rfm69hw_set_mode(RF69_MODE_SLEEP);
  
  LED_OFF;
}


/**
 * @brief RFM69HW packet rx
 *
 * @param mes Packet structure
 *
**/
void rfm69hw_packet_rx(struct packet_t *mes)
{
  rfm69hw_set_mode (RF69_MODE_STANDBY);

/* Read the FIFO contents */
  spi_select();
  spi_xfer(RegFifo & 0x7F);
  mes->network_id     = spi_xfer(0);
  mes->sending_device = spi_xfer(0);
  mes->message_type   = spi_xfer(0);
  spi_deselect();

  rfm69hw_set_mode(RF69_MODE_RX);
}


/**
 * @brief RFM69HW set tx power
 *
 * @param level 0..23
 *
 * See SX1231H data sheet Table 10
 *
**/
void rfm69hw_tx_power(int level)
{
  int power_level;
  int pa_setting;

  if (level > 23)
    level = 23;

  power_level = level;

  if (level < 16)
  {
    power_level += 16;
    pa_setting = RFM69HW_RF_PALEVEL_PA1_ON;
  }
  else
  {
    if (level < 20)
      power_level += 10;
    else
      power_level += 8;

    pa_setting = RFM69HW_RF_PALEVEL_PA1_ON | RFM69HW_RF_PALEVEL_PA2_ON;
    rfm69hw_set_high_power();
  }

  rfm69hw_write_reg(RegPaLevel, pa_setting | power_level);
}

