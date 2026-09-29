/**
 * @file    rfm69.h
 * @brief   Header file for rfm69.c
 *
**/
#ifndef RFM69_H
#define RFM69_H

/* RFM69HW modes */
enum rf69_mode { RF69_MODE_SLEEP, RF69_MODE_STANDBY, RF69_MODE_SYNTH, RF69_MODE_TX, RF69_MODE_RX };

void rfm69hw_init(void);
int rfm69hw_payload_ready(void);
uint8_t rfm69hw_read_reg(uint8_t address);
void rfm69hw_write_reg(uint8_t address, uint8_t data);
void rfm69hw_set_mode(enum rf69_mode mode);
void rfm69hw_register_dump(void);
int32_t rfm69hw_read_rssi(void);
int32_t rfm32hw_temperature(void);
void rfm69hw_packet_send(struct packet_t * mes);
void rfm69hw_packet_rx(struct packet_t * mes);
void rfm69hw_tx_power(int level);

#endif
