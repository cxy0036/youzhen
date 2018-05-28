/**
  ******************************************************************************
  * @file    ADC.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   This file implement functions that in ADC.h define.
   *****************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ADC.h"
#include "delay.h"
//#include "stdio.h"

/* Public Functions ----------------------------------------------------------*/

/*
********************************************************************************
*                                     ADC INITIALIZATION FUNCTIONS
*
* Description: This function is called by Init_Fun() in GPIO_Config.h at the beginning in main() .
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/
void ADC_Init_Fun(void)
{
/* IO Initial ----------------------------------------------------------------*/
   ADC_PORT->ODR |= ADC_MAN_FAN_PIN;  /* Out Hight */
   ADC_PORT->CR1 &= ~ADC_MAN_FAN_PIN; /* Floating Input */
   ADC_PORT->CR2 &= ~ADC_MAN_FAN_PIN; /* Disable External Interrupt */
   ADC_PORT->DDR &= ~ADC_MAN_FAN_PIN; /* Input Model */
  
   ADC_HV_IN_PORT->ODR |= ADC_HV_IN_PIN;  /* Out hight */ 
   ADC_HV_IN_PORT->CR1 &= ~ADC_HV_IN_PIN; /* Floating Input */
   ADC_HV_IN_PORT->CR2 &= ~ADC_HV_IN_PIN; /* Disable External Interrupt */
   ADC_HV_IN_PORT->DDR &= ~ADC_HV_IN_PIN; /* Input */
   
/* ADC Initial ---------------------------------------------------------------*/  
   ADC1->CR1 = 0x02;     /* Fadc = Fmaster/2, Continue Conversion Mode, Disable ADC Conversion */
   ADC1->CR2 = 0x00;     /* Disable External Event Eonversion, Left Alignment, Disable Scan Mode */
   ADC1->CR3 = 0x00;     /* Disable Data Buffer */
   ADC1->CSR = 0x01;     /* Disable ALL ADC Interrupt, Select Channel AIN3(HV_IN) */
   ADC1->TDRH |= 0x02;    /* Disable Schmitt Trigger AN9*/
   ADC1->TDRL |= 0x01;    /* Disable Schmitt Trigger AN2*/
  //ADC1->HTRL = 0xFF;    /* Disable Schmitt Trigger */
 // ADC1->AWCRH = 0x00;   /* Disable AWD in Data Buffer */
 // ADC1->AWCRL = 0x00;   /* Disable AWD in Data Buffer */
}

/*
********************************************************************************
*                           READ ONE ADC CHANNEL VALUE FUNCTIONS
*
* Description: This function read one ADC channel value,return the 16 bit converter value.
*
* Arguments  : wangt convert ADC channel
*
* Return  : Return the convert value of 16 Bit for the channel
*
* Note(s)    : none.
********************************************************************************
*/
uint16_t ADC_Read_Channel_Fun(uint8_t channel)
{
  uint16_t readdata,read_arry[6];  
  uint8_t i,j;
  ADC1->CSR  = channel;    /* Clean Flag, Select Channel  */
  ADC1->CR1 |= 0x01;    /* Start Conversion */
  Delay_us(10);   /* Must Need Delay 7us More Than */
  ADC1->CR1 |= 0x01;    /* Start Conversion */
  for(i=0;i<5;i++)
  {   
    while(!(ADC1->CSR & 0x80));   /* Wait Conversion Complete ,Convert 5 Times*/
    read_arry[i+1] = (uint16_t)ADC1->DRH;     /* Read Data Convertsion to 16 Bit*/
    read_arry[i+1] = (read_arry[i+1] << 2) | ADC1->DRL;
    ADC1->CSR &= ~0x80;  /* Clear Complete Flag */
  }
  ADC1->CR1 &= ~0x01;   /* Stop Conversion */
  
/* Filter Funtion**************************************************************/
  for(i=1;i<6;i++)  /* Insert sort */
  {
    read_arry[0] = read_arry[i];
    for(j=i-1; read_arry[0]<read_arry[j]; j--)
    {
        read_arry[j+1] = read_arry[j];
    }
    read_arry[j+1] = read_arry[0];
  }
  readdata = read_arry[3]; /*  Midle Data to Retrun */
/******************************************************************************/
  return readdata;
}


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/


