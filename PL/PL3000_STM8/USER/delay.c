/**
  ******************************************************************************
  * @file    delay.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   This file implement delay ms and delay us functions. 
   *****************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"

/* Public Functions ----------------------------------------------------------*/

/*
********************************************************************************
*                                       DELAY US FUNCTIONS
*
* Description: This function Delay N us.
*
* Arguments  : Number of Delay times.
*
* Note(s)    : none.
********************************************************************************
*/
void Delay_us(u16 nus)
{  
__asm(
"PUSH A          \n"  //1T,压栈
"DELAY_XUS:      \n"  
"LD A,#$4        \n"  
"NOP             \n"
"DELAY_X:        \n"
"DEC  A          \n"  //1T,x--
"JRNE DELAY_X    \n"    //不等于0,则跳转(2T)到DELAY_XUS继续执行,若等于0,则不跳转(1T).
"DECW X          \n"  //1T,x--
"JRNE DELAY_XUS  \n"    //不等于0,则跳转(2T)到DELAY_XUS继续执行,若等于0,则不跳转(1T).
"POP A           \n"  //1T,出栈
);
}

/*
********************************************************************************
*                                       DELAY MS FUNCTIONS
*
* Description: This function Delay N ms.
*
* Arguments  : Number of Delay times.
*
* Note(s)    : none.
********************************************************************************
*/
void Delay_ms(uint16_t N)
{
  while(N--)
  {
    TIM4->IER = 0x00;     /* Disable Interrupt */
    TIM4->CNTR = 0x00;    /* Re-initializes Counter */
    TIM4->SR1 = 0x00;     /* Clean Flag */
    TIM4->ARR = 0xFA;     /* 0.25MHz Num = 250, 1000us + 0.5us = 1000.5us*/
    TIM4->PSCR = 0x06;    /* SET Clk, Fck = 16/64 MHz = 0.25MHz*/
    TIM4->CR1 = 0x01;     /* Start */
    while(!(TIM4->SR1 & 0x01));
    TIM4->SR1 = 0x00;     /* Clean Flag */
    TIM4->CR1 = 0x00;     /* Stop */
  }
}


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/