/**
  ******************************************************************************
  * @file    UART.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define UART Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "UART.h"
#include <stdio.h>

/* Public functions ----------------------------------------------------------*/

/*
********************************************************************************
*                           DEBUG UART CONFIG FUNCTIONS
*
* Description: This function config uart.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void DEBUG_USART_Config(void)
{
/*************GPIO Config*************/	
  GPIO_Init(USART_DEBUG_TX_PORT,USART_DEBUG_TX_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(USART_DEBUG_RX_PORT,USART_DEBUG_RX_PIN, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(UART_DE_PORT,UART_DE_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
  
/*************USART Config*************/
  UART2_Init(USART_BLUETOOTH_BAUTRATE, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, 
             UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
  UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
  UART2_Cmd(ENABLE);
}

/*
********************************************************************************
*                          UART SEND ONE　BYTE FUNCTIONS
*
* Description: This function send one byte data.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void USARTx_SendByte(uint8_t data)
{
 UART2_SendData8(data);
 while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);  
}

/*
********************************************************************************
*                          UART SEND ONE　FRAME DATA FUNCTIONS
*
* Description: This function send one frame data.
*
* Arguments  : data arry , data length.
*
* Note(s)    : none.
********************************************************************************
*/
void USARTx_SendFrame(uint8_t data[],uint8_t data_length)
{
  for(int i=0;i<data_length;i++)
  {
    USARTx_SendByte(data[i]);
    while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
  }
}

/*
********************************************************************************
*                          UART SEND ONE　STRING FUNCTIONS
*
* Description: This function send one string data.
*
* Arguments  : string pointer.
*
* Note(s)    : none.
********************************************************************************
*/
void USARTx_SendString(char *str)
{
  unsigned int i=0;
  do
  {
    USARTx_SendByte(*(str+i));
    i++;
  }while(*(str+i)!='\0');
  while(UART2_GetFlagStatus(UART2_FLAG_TC)==RESET);   
}

/*
********************************************************************************
*                          UART SEND HALFWORD　FUNCTIONS
*
* Description: This function send one halfword.
*
* Arguments  : 16 bit data.
*
* Note(s)    : none.
********************************************************************************
*/
void USARTx_SendHalfWord(uint16_t data)
{
  uint8_t temp_h,temp_l;
  temp_h=(data&0xff00)>>8;
  temp_l=data&0xff;
  UART2_SendData8(temp_h);
  while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
  UART2_SendData8(temp_l);
  while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
}

/*
********************************************************************************
*                          UART MAPPING TO　FPUTC FUNCTIONS
*
* Description: This function mapping uart function to fputc.
*
* Arguments  : .
*
* Note(s)    : Do this function you can use printf send data.
********************************************************************************
*/
int fputc(int ch,FILE *f)
{
  UART2_SendData8(ch);
  while(UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET);
  return (ch);
}

/*
********************************************************************************
*                          UART MAPPING TO　FGETC FUNCTIONS
*
* Description: This function mapping uart function to fgetc.
*
* Arguments  : .
*
* Note(s)    : Do this function you can use scanf get data.
********************************************************************************
*/
int fgetc(FILE *f)
{
  while(UART2_GetFlagStatus(UART2_FLAG_RXNE)==RESET);
  return (int)UART2_ReceiveData8();
}



/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/



