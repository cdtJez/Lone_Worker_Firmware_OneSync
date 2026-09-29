/**
 * @file  fxls8964.c
 * @brief Serial interface command decoder and functions
 *
**/
#include "main.h"
#include "fxls8964_regs.h"

#define FXLS8964_WR           0x30
#define FXLS8964_RD           0x31

#define TILT_60_DEGREES       32



/**
 * @brief Initialise the accelerometer
 *
**/
void fxls8964_init(void)
{
/* Set up I2C interface */
  i2c_config();

/* Reset the tilt sensor */
  i2c_start();
  i2c_tx(FXLS8964_WR);
  i2c_tx(FXLS8964_SENSOR_CONFIG1);
  i2c_tx(1 << 7);
  i2c_stop();

/* Reset delay 1ms */
  delay_us(1000);

/* Set fast read - 8 bit data */
  i2c_start();
  i2c_tx(FXLS8964_WR);
  i2c_tx(FXLS8964_SENSOR_CONFIG2);
  i2c_tx(1);              /* Fast read */
  i2c_stop();

/* Set the update rate to 1.536Hz */  
  i2c_start();
  i2c_tx(FXLS8964_WR);
  i2c_tx(FXLS8964_SENSOR_CONFIG3);
  i2c_tx(0xB0);      
  i2c_stop();  
  
/* Turn on the tilt sensor */
  i2c_start();
  i2c_tx(FXLS8964_WR);
  i2c_tx(FXLS8964_SENSOR_CONFIG1);
  i2c_tx(1);              /* Enable the device */
  i2c_stop();
}



/**
 * @brief Turn the accelerometer on
 *
**/
void fxls8964_on(void)
{
  i2c_start();
  i2c_tx(FXLS8964_WR);
  i2c_tx(FXLS8964_SENSOR_CONFIG1);
  i2c_tx(1);              /* Enable the device */
  i2c_stop();  
  
}



/**
 * @brief Turn the accelerometer on
 *
**/
void fxls8964_off(void)
{
  i2c_start();
  i2c_tx(FXLS8964_WR);
  i2c_tx(FXLS8964_SENSOR_CONFIG1);
  i2c_tx(0);              /* Disable the device */
  i2c_stop();  
  
}



/**
 * @brief Read the x axis value
 
 *
 * @return x
 *
**/
int fxls8964_read_x(void)
{
  i2c_start();
  i2c_tx(FXLS8964_WR);
  i2c_tx(FXLS8964_OUT_X_LSB);

  i2c_start();
  i2c_tx(FXLS8964_RD);
  int8_t x = i2c_rx(NACK);
  i2c_stop();  

  return((int)x);
}



/**
 * @brief Test if the unit is tilted
 *
 * Generates a tilt event if the unit is tilted more than 60° in any direction
 *
**/
void fxls8964_tilt_check(void)
{
  static bool tilt_flag = false;
	
//  if (params.tilt_timeout == 0)
//    return;
  
  if (fxls8964_read_x() < TILT_60_DEGREES)
  {
    if (tilt_flag == false)
    {
      tilt_flag = true;
      event_put(EVENT_TILT);
    }
  }
  else
  {
    if (tilt_flag)
      event_put(EVENT_UNTILT);
    
    tilt_flag = false;
  }
}

