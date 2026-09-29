/**
 * @file		device.h
 * @brief   Header file for setting device defaults
 *
**/
#ifndef DEVICE_H
#define DEVICE_H

#define MDH0201_LONE_WORKER
//#define MDH0202_LEGACY_RX
//#define MDH0203_WALL_TX
//#define MDH0204_REPEATER



#ifdef MDH0201_LONE_WORKER
  #define DEVICE_INPUTS                       1
  #define DEVICE_BATTERY_LOW_MV               3800
  #define DEVICE_BATTERY_CRITICAL_MV          3600

/* Parameter defaults */
  #define DEVICE_DEFAULT_NETWORK_ID           0x55
  #define DEVICE_DEFAULT_DEVICE_ID            1
  #define DEVICE_DEFAULT_INVERT_INPUT_MASK    0
  #define DEVICE_DEFAULT_IN_1_ACT_MSG         Msg_Activate_1
  #define DEVICE_DEFAULT_IN_1_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_2_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_2_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_3_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_3_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_4_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_4_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_ACTIVE_RESEND_TIME   120
  #define DEVICE_DEFAULT_BATTERY_STATUS_TIME  0
  #define DEVICE_DEFAULT_STILL_ALIVE_TIME     0
  #define DEVICE_DEFAULT_TX_POWER             23
  #define DEVICE_DEFAULT_TILT_TIMEOUT         10
  #define DEVICE_DEFAULT_TILT_RESEND          0
  #define DEVICE_DEFAULT_REPEATER_DELAY       0
  #define DEVICE_DEFAULT_RESEND_DELAY         0
  #define DEVICE_DEFAULT_NAME                 "MDH0201"
  #define DEVICE_DEFAULT_SERIAL_NUMBER        "10000"
#endif



#ifdef MDH0202_LEGACY_RX
  #define DEVICE_INPUTS                       2
  #define DEVICE_BATTERY_LOW_MV               3800
  #define DEVICE_BATTERY_CRITICAL_MV          3600

/* Parameter defaults */
  #define DEVICE_DEFAULT_NETWORK_ID           0x55
  #define DEVICE_DEFAULT_DEVICE_ID            1
  #define DEVICE_DEFAULT_INVERT_INPUT_MASK    0
  #define DEVICE_DEFAULT_IN_1_ACT_MSG         Msg_Activate_1
  #define DEVICE_DEFAULT_IN_1_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_2_ACT_MSG         Msg_Activate_2
  #define DEVICE_DEFAULT_IN_2_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_3_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_3_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_4_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_4_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_ACTIVE_RESEND_TIME   2
  #define DEVICE_DEFAULT_BATTERY_STATUS_TIME  0
  #define DEVICE_DEFAULT_STILL_ALIVE_TIME     0
  #define DEVICE_DEFAULT_TX_POWER             23
  #define DEVICE_DEFAULT_TILT_TIMEOUT         0
  #define DEVICE_DEFAULT_TILT_RESEND          0
  #define DEVICE_DEFAULT_REPEATER_DELAY       0
  #define DEVICE_DEFAULT_RESEND_DELAY         0
  #define DEVICE_DEFAULT_NAME                 "MDH0202"
  #define DEVICE_DEFAULT_SERIAL_NUMBER        "12345"
#endif


#ifdef MDH0203_WALL_TX
  #define DEVICE_INPUTS                       2
  #define DEVICE_BATTERY_LOW_MV               3800
  #define DEVICE_BATTERY_CRITICAL_MV          3600

/* Parameter defaults */
  #define DEVICE_DEFAULT_NETWORK_ID           0x55
  #define DEVICE_DEFAULT_DEVICE_ID            1
  #define DEVICE_DEFAULT_INVERT_INPUT_MASK    0
  #define DEVICE_DEFAULT_IN_1_ACT_MSG         Msg_Activate_1
  #define DEVICE_DEFAULT_IN_1_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_2_ACT_MSG         Msg_Activate_2
  #define DEVICE_DEFAULT_IN_2_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_3_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_3_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_4_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_4_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_ACTIVE_RESEND_TIME   120
  #define DEVICE_DEFAULT_BATTERY_STATUS_TIME  0
  #define DEVICE_DEFAULT_STILL_ALIVE_TIME     0
  #define DEVICE_DEFAULT_TX_POWER             23
  #define DEVICE_DEFAULT_TILT_TIMEOUT         0
  #define DEVICE_DEFAULT_TILT_RESEND          0
  #define DEVICE_DEFAULT_REPEATER_DELAY       0
  #define DEVICE_DEFAULT_RESEND_DELAY         5
  #define DEVICE_DEFAULT_NAME                 "MDH0203"
  #define DEVICE_DEFAULT_SERIAL_NUMBER        "12345"
#endif

#ifdef MDH0204_REPEATER
  #define DEVICE_INPUTS                       1
  #define DEVICE_BATTERY_LOW_MV               3800
  #define DEVICE_BATTERY_CRITICAL_MV          3600

/* Parameter defaults */
  #define DEVICE_DEFAULT_NETWORK_ID           0x55
  #define DEVICE_DEFAULT_DEVICE_ID            1
  #define DEVICE_DEFAULT_INVERT_INPUT_MASK    0
  #define DEVICE_DEFAULT_IN_1_ACT_MSG         Msg_Activate_1
  #define DEVICE_DEFAULT_IN_1_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_2_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_2_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_3_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_3_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_IN_4_ACT_MSG         Msg_Null
  #define DEVICE_DEFAULT_IN_4_DEACT_MSG       Msg_Null
  #define DEVICE_DEFAULT_ACTIVE_RESEND_TIME   2
  #define DEVICE_DEFAULT_BATTERY_STATUS_TIME  0
  #define DEVICE_DEFAULT_STILL_ALIVE_TIME     0
  #define DEVICE_DEFAULT_TX_POWER             23
  #define DEVICE_DEFAULT_TILT_TIMEOUT         0
  #define DEVICE_DEFAULT_TILT_RESEND          0
  #define DEVICE_DEFAULT_REPEATER_DELAY       5
  #define DEVICE_DEFAULT_RESEND_DELAY         0
  #define DEVICE_DEFAULT_NAME                 "MDH0204"
  #define DEVICE_DEFAULT_SERIAL_NUMBER        "12345"
#endif

#endif
