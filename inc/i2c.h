/**
 * @file   i2c.h
 * @brief  i2c header file
 * @author Mark Senior
 * 
**/
#ifndef I2C_H
#define I2C_H



enum { ACK = 0, NACK };

void i2c_config(void);
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_tx(uint8_t data);
uint8_t i2c_rx(uint8_t ack_bit);

#endif /* I2C_H */
