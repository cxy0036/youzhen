/**
  ******************************************************************************
  * @file    GPIO_Config.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2018-3-22
  * @brief   define HV and Flash Pin, PORT and Functions,all init function.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "GPIO_Config.h"  
#include "delay.h"
#include "clock_config.h"
#include "HV_Charge.h"
#include "stm8s_gpio.h"
#include "LED.h"
#include "KEY.h"
//#include "UART.h"
//#include "KEY_FTC332.h"
//#include "ADC.h"
//#include "Bluetooth.h"
//uint8_t Power_LED_Start_Flag = 1;
/* Public functions ----------------------------------------------------------*/



/*
********************************************************************************
*                           IWDG INIT FUNCTIONS
*
* Description: This function Initial IWDG.
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/
void IWDG_Init(void)
{
  IWDG->KR = 0xCC; //����IWDG
  IWDG->KR = 0x55; //��� PR �� RLR ��д����
  IWDG->RLR = 0xff; //���Ź���������װ����ֵ       
  IWDG->PR = 0x06; //��Ƶϵ��Ϊ256�����ʱʱ��Ϊ��1.02S
  IWDG->KR = 0xAA; //ˢ��IDDG������������Ź���λ��ͬʱ�ָ� PR �� RLR ��д����״̬
}

/*
********************************************************************************
*                           ALL INIT FUNCTIONS
*
* Description: This function Initial All function.
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/
void Init_Fun(void)
{	
  Clock_Config();  /* clock initial, Fmarst = 16Mhz */
  LED_GPIO_Init();
  KEY_GPIO_Init();
  EEPROM_IO_Init();
  IWDG_Init();
  
  
}




/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/