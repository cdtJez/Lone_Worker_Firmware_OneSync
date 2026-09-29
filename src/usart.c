/*
 * @file		usart.c
 * @brief   Serial functions
 *
 *
**/
#include "main.h"

/* Baud rate register values */
#define BAUD_16M_1200			  13333
#define BAUD_16M_2400			   6667
#define BAUD_16M_4800			   3333
#define BAUD_16M_9600			   1667
#define BAUD_16M_19200		    833
#define BAUD_16M_38400		    417
#define BAUD_16M_57600		    278
#define BAUD_16M_115200		    139

#define SERIAL_ACTIVE_PIN       0
#define SERIAL_ACTIVE_PIN_MASK  (1 << SERIAL_ACTIVE_PIN)

/**
 * @brief  Initialise USART2
 *
**/
void usart2_init(void)
{
/* Enable clock to USART2 */
	RCC->APBENR1 |= RCC_APBENR1_USART2EN;
	
/* Enable clock to GPIOA - done at the start of main */
/*	RCC->IOPENR |= RCC_IOPENR_GPIOAEN; */

/* Set alternate function on PA2 and PA3 */
	MODIFY_REG(GPIOA->MODER, (3 << 4) | (3 << 6), (2 << 4) | (2 << 6));

/* Set AF1 on PA9 & PA10 */
	MODIFY_REG(GPIOA->AFR[0], (15 << 8) | (15 << 12), (1 << 8) | (1 << 12));
	
/* Disable the USART */	
	USART2->CR1 = 0;
	
/* Set the baud rate */	
	USART2->BRR = BAUD_16M_9600;
	
/* Enable the USART, Rx & Tx */
//	USART2->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_FIFOEN;
  USART2->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
}


/**
 * @brief  Initialise PA0 as an input for the serial enable read pin.
 *         PORTB clock is enabled in the main function.
**/
void usart2_sen_init(void)
{
  MODIFY_REG(GPIOA->MODER, (3 << 0), 0);
}


/**
 * @brief  Turn off USART2
 *
**/
void usart2_deinit(void)
{
/* Make PA2 and PA3 inputs */  
//  GPIOA->MODER &= ~((3 << 4) | (3 << 6));
//  MODIFY_REG(GPIOA->PUPDR, ((3 << 4) | (3 << 6)), ((GPIO_PUPD_PULLDOWN << 4) | (GPIO_PUPD_PULLDOWN << 6)));  
  
/* Disable the USART */	
	USART2->CR1 = 0;

/* Disable clock to USART2 */
	RCC->APBENR1 &= ~RCC_APBENR1_USART2EN;
}


/**
 * @brief  Send a serial character
 *
 * @param  data character to send
**/
void usart2_tx_char(int data)
{
/* Wait for Tx register to be empty */
	while (!(USART2->ISR & USART_ISR_TXE_TXFNF)) {};
	
	USART2->TDR = data;
}


/**
 * @brief  Receive a serial character
 *
 * @return character received
**/
int usart2_rx_char(void)
{
  return(USART2->RDR);
}


/**
 * @brief  Test if there is a serial character waiting
 *
 * @return True if character waiting
**/
int usart2_rx_waiting(void)
{
  return(USART2->ISR & USART_ISR_RXNE_RXFNE);
}


/**
 * @brief  PB7 low indicates serial active
 *
 * @return True if serial active
**/
int usart2_serial_active(void)
{
  return(GPIOA->IDR & SERIAL_ACTIVE_PIN_MASK);
}


/**
 * @brief  Check if the TX FIFO is empty
 *
 * @return True if TX FIFO empty
**/
bool usart2_txfifo_empty(void)
{
  return((USART2->ISR & USART_ISR_TXFE) != 0);
}


