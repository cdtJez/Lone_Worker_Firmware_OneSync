/*
 * @file		adc.c
 * @brief   Analogue to digital converter
 *
 *
**/
#include "main.h"

/* Scaling factors for ADC count to mV */
#define ADC_MV_MULTIPLY   1875
#define ADC_MV_DIVIDE     1792

/**
 * @brief  Initialise the ADC
 *
**/
void adc_init(void)
{
/* Turn on the ADC clock */
  RCC->APBENR2 |= RCC_APBENR2_ADCEN;

/* Make sure the ADC is not enabled */  
  ADC1->CR &= ~(ADC_CR_ADEN);
  
/* Turn on the voltage regulator */  
  ADC1->CR |= ADC_CR_ADVREGEN;
  
/* 20us voltage regulator settling time */
  delay_us(20);  
  
/* Set the calibration */
  ADC1->CR |= ADC_CR_ADCAL;
  while (ADC1->CR & ADC_CR_ADCAL) {};

/* Enable VREF*/
  ADC->CCR |= ADC_CCR_VREFEN;

/* VLIPO is on PA5/pin 16, ADC_IN5 (approved move from PC6/pin 30). */
  ADC1->CHSELR |= (ADC_CHSELR_CHSEL5);
  
/* Enable the ADC */
  ADC1->CR |= (ADC_CR_ADEN);
  
/* Wait for the ADC to be ready */
  while (!(ADC1->ISR & ADC_ISR_ADRDY)) {};
}



/**
 * @brief  De-initialise the ADC
 *
**/
void adc_deinit(void)
{
/* Disable the ADC */  
  ADC1->CR |= ADC_CR_ADDIS;
}



/**
 * @brief  Read the battery voltage
 *
**/
int adc_read_battery_mV(void)
{
  adc_init();

/* Start a conversion */
  ADC1->CR |= (ADC_CR_ADSTART);

/* Wait for conversion to end */
  while (!(ADC1->ISR & ADC_ISR_EOC)) {};
 
/* Read the ADC and convert to mV */ 
  int mv = (ADC1->DR * ADC_MV_MULTIPLY) / ADC_MV_DIVIDE;
  
  adc_deinit();
  
  return(mv);
}




