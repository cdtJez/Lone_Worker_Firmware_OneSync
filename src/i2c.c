/**
 * @file   i2c.c
 * @brief  i2c routines 
 * @author Mark Senior
 * 
**/
#include "main.h"


/* GPIOB pinout: PB6/SCL, PB7/SDA (unchanged). */
#define SCL               6
#define SDA               7


/* I2C macros */
#define I2C_DATA_LO       GPIOB->BRR  = (1 << SDA)           
#define I2C_DATA_HI       GPIOB->BSRR = (1 << SDA)

#define I2C_CLK_LO        GPIOB->BRR  = (1 << SCL)
#define I2C_CLK_HI        GPIOB->BSRR = (1 << SCL)

#define I2C_SDA_PIN       (GPIOB->IDR & (1 << SDA))

#define I2C_DELAY					delay_us(4)


/**
 * @brief   i2c configuration
 * @param   None
 * @retval  None
**/
void i2c_config(void)
{
/* Enable clock to GPIOB - done at the start of main */
/*	RCC->IOPENR |= RCC_IOPENR_GPIOBEN; */  
  
/* SCL & SDA are outputs */
  MODIFY_REG(GPIOB->MODER, (3 << 12) | (3 << 14), (1 << 12) | (1 << 14));
  
/* SCL & SDA outputs are open collector */
  MODIFY_REG(GPIOB->OTYPER, (1 << 6) | (1 << 7), (1 << 6) | (1 << 7));

/* Initialise the bus */
  i2c_init();
}



/**
 *  @brief 	I2C start condition
 *
**/
void i2c_start(void)
{
  I2C_DATA_HI;
  I2C_CLK_HI;
  I2C_DELAY;
  I2C_DATA_LO;    // Start condition
  I2C_DELAY;
}


/**
 *  @brief 	I2C stop condition
 *         	Assumes clock is low on entry
 *
**/
void i2c_stop(void)
{
  I2C_DATA_LO;  
  I2C_DELAY;
  I2C_CLK_HI;
  I2C_DELAY;
  I2C_DATA_HI;  // Stop condition
}


/** 
 *  @brief Initialise the I2C
 *
**/
void i2c_init(void)
{
  I2C_CLK_HI;
  I2C_DELAY;
  I2C_DATA_HI;   // Stop condition
}


/** 
 *	@brief 	Transmit a data byte on the I2C
 *
 * 	@param  data - data byte to send
 * 	@retval 0 - ACK low   1 - ACK high
 *
**/
uint8_t i2c_tx(uint8_t data)
{
  uint8_t ret = 0;
  uint8_t i = 8;

// I2C bit loop - bit7 down to bit0  
  while (i--)
  {
    I2C_CLK_LO;  

    if (data & 0x80)
      I2C_DATA_HI;
    else
      I2C_DATA_LO;

    data <<= 1;

    I2C_DELAY;
    I2C_CLK_HI;
    I2C_DELAY;
  }
  I2C_CLK_LO;
  
// Setup ACK
  I2C_DATA_HI;    // Bus floating

  I2C_DELAY;
  I2C_CLK_HI;
  
  I2C_DELAY;
  if (!I2C_SDA_PIN)  // Read ACK
    ret = 1;

  I2C_CLK_LO;
  I2C_DELAY;  
  
  return (ret);
}


/** 
 *  @brief Receive a data byte 
 *
 *  @param  ack_bit - ACK bit state
 *  @retval received data
 *
**/
uint8_t i2c_rx(uint8_t ack_bit)
{
  uint8_t ret = 0;
  uint8_t bitnum = 8;
  
  I2C_DATA_HI;    // Data line floating
  
// I2C read bit loop  
  while (bitnum--)
  {
    I2C_CLK_HI; 
    I2C_DELAY;

    ret <<= 1;
    if (I2C_SDA_PIN)
      ret |= 1;

    I2C_CLK_LO;  
    I2C_DELAY;
  }
  
  if (ack_bit == ACK)    // NACK by default
    I2C_DATA_LO;

  I2C_DELAY;
  
  I2C_CLK_HI;
  I2C_DELAY; 
  I2C_CLK_LO;
  
  I2C_DATA_HI;    // Data line floating  
  I2C_DELAY;
  return (ret);
}

