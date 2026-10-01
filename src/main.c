/**
 * MDH 868MHz Lone Worker Alarm / Pendant
 *
 * Micro - STM32G071CBT6 128kbyte flash, 36kbyte RAM
 *
**/
#include "main.h"

/* Port use - U8 STM32G071CBT6, LQFP48, CA01003 schematic rev 1.1.
 * VLIPO is moved from PC6/pin 30 to PA5/pin 16 (ADC_IN5).
 * Dir: I=input, O=output, B=bidirectional, A=analogue, X=unused.
 * -----------------------------------------------------------------------------
 * | Port | Pin | Dir | Signal  - Function                                       |
 * -----------------------------------------------------------------------------
 * | PC13 |   1 |  O  | BLUE    - blue LED / activity, active high                |
 * | PC14 |   2 |  O  | RED     - red LED, active high                            |
 * | PC15 |   3 |  O  | GREEN   - green LED, active high                          |
 * | VBAT |   4 |  X  | NC      - backup supply not connected in schematic        |
 * | VREF+|   5 | PWR | 3V0     - ADC reference supply                            |
 * | VDD  |   6 | PWR | 3V0     - processor supply                                |
 * | VSS  |   7 | PWR | GND     - ground                                          |
 * | PF0  |   8 |  I  | SET     - setting button, active low, switched PU/PD      |
 * | PF1  |   9 |  I  | ACT     - alarm button, active low, switched PU/PD        |
 * | PF2  |  10 |  I  | NRST    - processor reset                                |
 * | PA0  |  11 |  I  | VINP    - serial connector presence, active high          |
 * | PA1  |  12 |  O  | SCK     - radio SPI1 clock, AF0                           |
 * | PA2  |  13 |  O  | MOSI    - radio SPI1 data output, AF0                     |
 * | PA3  |  14 |  A  | VUSB    - USB voltage divider, not sampled               |
 * | PA4  |  15 |  O  | NSS     - radio SPI select, active low GPIO               |
 * | PA5  |  16 |  A  | VLIPO   - battery divider, ADC_IN5; wiring change needed  |
 * | PA6  |  17 |  I  | MISO    - radio SPI1 data input, AF0, pull-down           |
 * | PA7  |  18 |  X  | NC                                                       |
 * | PB0  |  19 |  O  | BUZZ    - buzzer drive; sequence buzzer flags disabled   |
 * | PB1  |  20 |  I  | INT     - accelerometer interrupt; driver polls I2C       |
 * | PB2  |  21 |  X  | NC                                                       |
 * | PB10 |  22 |  X  | NC                                                       |
 * | PB11 |  23 |  X  | NC                                                       |
 * | PB12 |  24 |  X  | NC                                                       |
 * | PB13 |  25 |  X  | NC                                                       |
 * | PB14 |  26 |  O  | EX_RST  - expander reset; not configured by firmware      |
 * | PB15 |  27 |  I  | EX_INT  - expander interrupt; not used by firmware        |
 * | PA8  |  28 |  X  | NC                                                       |
 * | PA9  |  29 |  O  | TX      - serial USART1 transmit, AF1                     |
 * | PC6  |  30 |  X  | NC      - former VLIPO connection, moved to PA5           |
 * | PC7  |  31 |  X  | NC                                                       |
 * | PA10 |  32 |  I  | RX      - serial USART1 receive, AF1                      |
 * | PA11 |  33 | I/O | RESET   - radio reset, input at startup then output low   |
 * | PA12 |  34 |  I  | DIO0    - radio packet-ready input                       |
 * | PA13 |  35 |  B  | SWDIO   - programming/debug data                         |
 * | PA14 |  36 |  I  | BOOT    - BOOT0 / SWCLK programming/debug clock           |
 * | PA15 |  37 |  X  | NC                                                       |
 * | PD0  |  38 |  X  | NC                                                       |
 * | PD1  |  39 |  X  | NC                                                       |
 * | PD2  |  40 |  X  | NC                                                       |
 * | PD3  |  41 |  X  | NC                                                       |
 * | PB3  |  42 |  X  | NC                                                       |
 * | PB4  |  43 |  X  | NC                                                       |
 * | PB5  |  44 |  O  | VIB     - vibrator drive, active high                    |
 * | PB6  |  45 |  O  | SCL     - shared software I2C clock, open drain           |
 * | PB7  |  46 |  B  | SDA     - shared software I2C data, open drain            |
 * | PB8  |  47 |  X  | NC                                                       |
 * | PB9  |  48 |  I  | STAT    - charger status, active low, switched PU/PD      |
 * -----------------------------------------------------------------------------
 * Directions describe signal use; unimplemented signals retain reset settings.
 * Existing usart2_* function names access USART1 on PA9/PA10.
 */

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

/* Enable clock to GPIOA, GPIOB, GPIOC and GPIOF (SET/ACT). */
  RCC->IOPENR |= (RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN | RCC_IOPENR_GPIOFEN);

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
  
/* Set VINP/PA0 as the serial-presence input. */
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
