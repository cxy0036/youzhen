/**
  ******************************************************************************
  * @file    LED.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define KEY Pin, PORT and Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_H
#define __KEY_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>

/*
********************************************************************************
*                               FLAGS
********************************************************************************
*/
#define ON                       1
#define OFF                      0

#define YES                      1
#define NO                       0

#define HIGH                     1
#define LOW                      0

/*
********************************************************************************
*                               TIMEOUT VALUE
********************************************************************************
*/
#define KEY_READ_TIME_OUT        50000
#define TIME_OUT_10M             29000

/*
********************************************************************************
*                               FLASH MODEL FLAG
********************************************************************************
*/
#define STANDARD_MODE           1
#define AUTO_MODE               2
#define STANDARD_MULTI_MODE     3
#define AUTO_MULTI_MODE         4
#define OFF_FLASH_MODE          5

/*
********************************************************************************
*                               KEY PORT DEFINE
********************************************************************************
*/
#define KEY_PORT                 GPIOB
#define KEY_PIN                  GPIO_PIN_5

/*
********************************************************************************
*                               KEY TIMER DEFINE
********************************************************************************
*/
#define KEY_TIMER                TIM2

/*
********************************************************************************
*                               KEY NUMBER DEFINE
********************************************************************************
*/
#define KEY_ADD                  0x01
#define KEY_DEC                  0x02
#define KEY_AUTO                 0x04
#define KEY_POWER                0x08
#define KEY_UNLOCK               0x03       /* bit7 and bit5 */
#define KEY_FIX                  0x0b

/*
********************************************************************************
*                        FTC332 KEY TIME THRESHOLD DEFINE
********************************************************************************
*/
#define KEY_START_BIT_THRL       2940
#define KEY_START_BIT_THRH       3850

#define KEY_H_BIT_THRL           1440
#define KEY_H_BIT_THRH           1585

#define KEY_L_BIT_THRL           690
#define KEY_L_BIT_THRH           810

/*
********************************************************************************
*                               THERSHOLD
********************************************************************************
*/    
#define TOUCH_SW_RELEASE               0x365     /* 4.25V-------> No touch, no SW */ 
#define TOUCH_PUSH_H                   0x299     /* 3.25V-------> Touch push */
#define TOUCH_PUSH_L                   0x1A3     /* 2.05V-------> Touch push */
#define FLASH_SW_PUSH                  0x066     /* 0.5V-------> Flash SW push */
    
/*
********************************************************************************
*                               FUNCTIONS
********************************************************************************
*/
void Key_Init_Fun(void);
void Key_Timer_Init_Fun(void);
void Key_Read(void);
void Key_Analysis(void);
void Key_Process_Fun(void);
void Key_Power_Off_Proc(void);
void Key_Level_Up_Proc(void);
void Key_Level_Down_Proc(void);
void Key_Unlock_Proc(void);
void Function_Processe(void);
void Key_Power_On_Proc(void);
void Key_Mute_Proc(void);
void Key_Mode_Proc(void);
void Multi_Auto_Mode_Flash(void);
void Standard_Mode_Flash(void);
void Auto_Mode_Flash(void);
void Multi_Standard_Mode_Flash(void);
void Error_Power_Off_Proc(void);

#endif /*__KEY_H*/


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/
