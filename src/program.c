/**
 * @file  program.c
 * @brief Jump to program mode
 *
**/
#include "main.h"

#define PROGRAM_KEY1          0x12345678
#define PROGRAM_KEY2          0xBEEFFACE

/* 5 second timeout for key1 */
#define PROGRAM_KEY_TIMEOUT   50

static uint32_t key_timeout = 0;

/* This is the bootloader address for the STM32G030F6 processor */
/* as define in ST AN2606                                       */
#define BOOT_ADDR	0x1FFF0000


struct boot_vectable_ {
    uint32_t Initial_SP;
    void (*Reset_Handler)(void);
};

#define BOOTVTAB ((struct boot_vectable_ *)BOOT_ADDR)


/**
 * @brief  Jump to bootloader
 *
**/
void bootloader(void)
{
/* Disable all interrupts */
	__disable_irq();

/* Disable Systick timer */
	SysTick->CTRL = 0;

/* Set the RCC to the default state */
  RCC->CFGR = 0x00000000u;
  
/* Disable all interrupts */
  RCC->CIER = 0x00000000u;

/* Clear all flags */
  RCC->CICR = 0xFFFFFFFFu;  

/* Clear Interrupt Enable Register & Interrupt Pending Register */
  for (int i = 0; i < sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]); i++)
  {
    NVIC->ICER[i] = 0xFFFFFFFF;
    NVIC->ICPR[i] = 0xFFFFFFFF;
  }

/* Re-enable all interrupts */
	__enable_irq();

/* Set the MSP */
	__set_MSP(BOOTVTAB->Initial_SP);

/* Jump to bootloader firmware */
	BOOTVTAB->Reset_Handler();
}



/**
 * @brief  Verify that 2 keys are correct and jump to program mode
 *
**/
void program_key(uint32_t key)
{
/* Key1 resets the timer */  
  if (PROGRAM_KEY1 == key)
  {
    key_timeout = tick + PROGRAM_KEY_TIMEOUT;
    return;
  }

/* Jumps to the bootloader if Key2 supplied within PROGRAM_KEY_TIMEOUT of Key1 */
  if (PROGRAM_KEY2 == key && (tick < key_timeout))
  {
/* Turn LEDs on to show it has worked */
    sequencer_setup(10, sequence_colours);

/* Jump to bootloader */    
    bootloader();
  }
}

