/**
 * MDH 868MHz Lone Worker Alarm / Pendant
 *
 * Micro - STM32G071CBT6 128kbyte flash, 36kbyte RAM
 *
**/
#include "main.h"

/* Port use - */
/* --------------------------------------------------------------------------*/
/* | Port | Pin | Dir | Pull | Name  - Function                             |*/
/* --------------------------------------------------------------------------*/
/* | PB9  |  1  |  I  |  PU  | STAT  - input from charger                 	|*/
/* | PC14 |  2  |  I  |  A   | NC                                           |*/
/* | PC15 |  3  |  I  |  A   | ACT   - alarm button input    - active PU/PD |*/
/* | VDD  |  4  | PWR |      | Power - 3V                                   |*/
/* | VSS  |  5  | PWR |      | Power - ground                               |*/
/* | NRST |  6  |  I  |      | Reset input                                  |*/
/* | PA0  |  7  |  A  |  N   | VUSB  - Potential divider on USB +V          |*/
/* | PA1  |  8  |  A  |  N   | VLIPO - Potential divider on Lipo cell       |*/
/* | PA2  |  9  |  O  |  N   | Tx    - RS232 Transmit                       |*/
/* | PA3  | 10  |  I  |  N   | Rx    - RS232 Receive                        |*/
/* | PA4  | 11  |  O  |  N   | NSS   - RFM69 SPI select                     |*/
/* | PA5  | 12  |  O  |  N   | CLK   - RFM69 SPI clock                      |*/
/* | PA6  | 13  |  I  |  N   | MISO  - RFM69 SPI master in slave out        |*/
/* | PA7  | 14  |  O  |  N   | MOSI  - RFM69 SPI master out slave in        |*/
/* | PB0  | 15  |  O  |  N   | BLUE  - blue LED drive                       |*/
/* | PB1  | 16  |  O  |  N   | RED   - red LED drive                        |*/
/* | PB2  | 17  |  O  |  N   | GREEN - green LED drive                      |*/
/* | PA8  | 18  |  I  |  N   |       - RFM69 reset - Input then output      |*/
/* | PA9  | 19  |  I  |  PU  |       - RFM69 packet ready signal            |*/
/* | PC6  | 20  |  X  |  N   | SET   - settings button input - active PU/PD |*/
/* | PA10 | 21  |  X  |  N   | NC                                           |*/
/* | PA11 | 22  |  X  |  N   | NC                                           |*/
/* | PA12 | 23  |  X  |  N   | NC                                           |*/
/* | PA13 | 24  |  B  |  N   | SWDIO - programming data i/o                 |*/
/* | PA14 | 25  |  I  |  N   | SWCLK - programming clock                    |*/
/* | PA15 | 26  |  X  |  N   | NC                                           |*/
/* | PB3  | 27  |  O  |  N   | VIB   - vibrator drive                       |*/
/* | PB4  | 28  |  O  |  N   | BUZZ  - buzzer drive                         |*/
/* | PB5  | 29  |  I  |  PU  | INT   - interrupt input from accelerometer   |*/
/* | PB6  | 30  |  O  |  EXT | SCL   - accelerometer I2C clock              |*/
/* | PB7  | 31  |  B  |  EXT | SDA   - accelerometer I2C data               |*/
/* | PB8  | 32  |  X  |  N   | NC                                           |*/
/* --------------------------------------------------------------------------*/

/* Timer variables */
uint32_t tick = 0;
uint32_t battery_state_time_counter = 0;
uint32_t still_alive_counter = 0;

struct packet_t message;



/**
 * @brief Set stop mode 1
 *
**/
void go_to_sleep(void)
{
/* Set wake up timer write flag */
  RTC->ICSR |= RTC_ICSR_WUTWF;

/* Reset auto wakeup - clear the wakeup timer flag */
  RTC->SCR = RTC_SCR_CWUTF;

/* Set SLEEPDEEP bit */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

/* Set stop mode 1 */
  PWR->CR1 = (PWR->CR1 & ~0x7) | 1;

/* Enable auto wakeup */
  RTC->SR |= RTC_SR_WUTF;

/* Clear wake up timer write flag */
  RTC->ICSR &= ~RTC_ICSR_WUTWF;

/* Enter stop mode */
  __WFI();
}



/**
 * @brief  Check the battery voltage and report low values
 *
**/
void check_battery_voltage(void)
{
/* Check if battery state monitor is off */
  if (battery_state_time_counter == 0)
    return;

  battery_state_time_counter--;
  if (battery_state_time_counter > 0)
    return;

/* Counter now zero so re-initialise */
  battery_state_time_counter = params.battery_state_time;    
  
/* Read and report battery condition */
  int mV = adc_read_battery_mV();
  if (mV < DEVICE_BATTERY_CRITICAL_MV)
    message_send(Msg_BatteryCritical);
  else if (mV < DEVICE_BATTERY_LOW_MV)
    message_send(Msg_BatteryLow);
}



/**
 * @brief  Check if the still alive counter has timed out
 *
**/
void check_still_alive(void)
{
/* Test if still alive counter active */
  if (still_alive_counter == 0)
    return;

/* Now check if it has timed out */    
  still_alive_counter--;
  if (still_alive_counter > 0)
    return;

/* Still alive counter now zero so re-initialise and report */
  still_alive_counter = params.still_alive_time;          
  message_send(Msg_StillAlive);
}



/**
 * @brief  Setup and main loop
 *
**/
int main(void)
{
/* Enable the power control block */
  RCC->APBENR1 |= RCC_APBENR1_PWREN;

/* Enable clock to GPIOA, GPIOB & GPIOC */
  RCC->IOPENR |= (RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN);

/* Load the parameters */
  parameter_load(&params);

/* Initialise the RF module and put into standby mode */
  rfm69hw_init();

/* Seed the random number generator */
  random_seed(params.checksum ^ rfm32hw_temperature());

/* Initialise the button system */
  input_init();

/* Initialise the event system */
  event_init();
  
/* Initialise the LED, buzzer & vibrator sequencer */
  sequencer_init();
  
/* Initialise the charger STAT line */  
  charger_init();

/* Initialise the wake up timer. Wakes the device every 1/10th second */
  rtc_init_auto_wake();
  
/* Make sure the rfm69hw is in sleep mode, this needs to happen after */
/*  the tranceiver is initialised */
  rfm69hw_set_mode(RF69_MODE_SLEEP);
  
/* Initialise the tilt sensor */
  fxls8964_init();
  
/* Set PB7 as an input with pullup to signal that the serial is active */
  usart2_sen_init();

/* If the serial is plugged in, print a sign on string */
  if (usart2_serial_active())
  {
    usart2_init();
    xprintf("\nMDH Wireless - %s V%d.%d:B%d SN%s\n\n", params.model, VERSION_MAJOR, VERSION_MINOR, BUILDNUM, params.serial);
    usart2_deinit();
  }
  
/* Initialise the state machine */
  machine_state_initialise();

/* Initialise the battery read and still alive timers */
  battery_state_time_counter = params.battery_state_time;
  still_alive_counter = params.still_alive_time;
 
/* Initialise the packet transmission queue */  
  tx_queue_init();


/* Main loop */
  for (;;)
  {
/* Sleep until the 100ms interrupt */
    spi_disable_before_sleep();       /* Check SPI transactions have completed then disable */
    go_to_sleep();
    spi_enable_after_sleep();

/* Tick count */
    tick++;
    
/* Scan the inputs and generate events */
    input_scan();
    
/* Update the state machine */
    machine_state_process();
 
/* Run the LED, buzzer & vibrator sequencer */    
    sequencer_update();

/* Check if the serial is connected - this stops alarm services */    
    if (usart2_serial_active())
    {
      usart2_init();
      while (usart2_serial_active())
      {
        if (usart2_rx_waiting())
          command_store(usart2_rx_char());
          
//        if (!charger_stat())
//          sequencer_static(LED_G);

/* If the battery is below UVLO threshold for charger then the Ipreg current */
/*  is less than the LED current so the battery will continue to discharge   */
        if (adc_read_battery_mV() > DEVICE_BATTERY_CRITICAL_MV)
        {
/* Show charging state */          
          if (charger_stat())
            sequencer_static(LED_R);
          else
            sequencer_static(LED_G);
        }
      }
      usart2_deinit();
      
/* Make sure the LED is off */      
      sequencer_static(BLANK);
      
/* Re-initialise the machine state */      
      machine_state_initialise();
    }

/* If the machine is off, don't do any more */    
    if (machine_state_is_off())
      continue;
 
/* Any packets ready to be transmitted? */
    tx_queue_process(); 

/* Check if the unit is tilted every second */    
    if ((tick % TICKS_PER_SECOND) == 0)
      fxls8964_tilt_check();      
      
/* One hour checks for battery state and still alive */ 
/* This needs testing */ 
//    if (tick == TICKS_IN_1_HOUR)
//    {
//      tick = 0;
//        
//      check_battery_voltage();
//      check_still_alive();
//    }
  }
}



/**
 * @brief  Low level system configuration
 * @retval None
 *
 * Gets called from startup_stm32g071xx.s before main
 *
**/
void SystemInit(void)
{
  /* Make sure HSI clock is on */
  RCC->CR |= RCC_CR_HSION;
  while (!(RCC->CR & RCC_CR_HSIRDY)) {};   /* Wait for HSI to stabilise */

  /* Set AHB & APB pre-scaler to 1 */
  RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE_Msk);

  /* Sysclk clock source is HSI */
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, 0);

/* Turn on the low power internal clock */
	RCC->CSR |= RCC_CSR_LSION;
	while ((RCC->CSR & RCC_CSR_LSIRDY) == 0) {}; /* Wait for LSI clock to stabilise */
}
