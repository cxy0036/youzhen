/**
  ******************************************************************************
  * @file    clock_config.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   This file implement functions that in clock_config.h define.
   *****************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "clock_config.h"
#include <stm8s_clk.h>

/* Public Functions ----------------------------------------------------------*/

/*
********************************************************************************
*                              CLOCK INITIALIZATION FUNCTIONS
*
* Description: This function is called by Init_Fun() in GPIO_Config.h at the beginning in main() .
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/
void Clock_Config(void)
{
   /*
  CLK_HSECmd(ENABLE);                                   //�ⲿʱ�ӿ�    
  CLK_LSICmd(ENABLE);                                   //�ڲ���ƵRC��    
  CLK_HSICmd(ENABLE);                                   //�ڲ���ƵRC��   
  while(SET != CLK_GetFlagStatus(CLK_FLAG_HSERDY));    //�ȴ��ⲿ��������    
  CLK_ClockSwitchCmd(ENABLE);                           //�л�ʹ��    
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_MANUAL,CLK_SOURCE_HSE,DISABLE,CLK_CURRENTCLOCKSTATE_DISABLE);//�л����ⲿ����   
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);              //1��Ƶ    
 */
  CLK_HSECmd(DISABLE);           
  CLK_HSICmd(ENABLE);   // Enable HSI Clock 
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // Set Fhsidiv = 16/1 = 16 MHz 
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);      // Set Fmaster = 16MHz, Fcpu = 16MHz 
   
}


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/