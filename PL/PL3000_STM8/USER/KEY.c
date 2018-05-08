/**
  ******************************************************************************
  * @file    LED.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define LED Pin, PORT and Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#include "KEY.h"
#include "LED.h"

uint8_t Key_Trg = 0,Key_Cont = 0;       /*  */

uint8_t Power_Flag = OFF;       /* POWER Flag, defualt OFF */
uint8_t Mute_Flag = OFF;        /* MUTE Flag, defualt OFF */
uint8_t UP_Flag = OFF;          /* Key UP Flag, defualt OFF */
uint8_t Down_Flag = OFF;        /* KEY Down Flag, defualt OFF */
uint8_t Auto_Flag = ON;         /* AUTO Flag, defualt ON */
uint8_t Lock_Flag = ON;         /* LOCK Flag, defualt ON */

uint8_t HV_Leve = 0;         /* HV Level count */

/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Read Key Function.
  * @note   Key_Trg = 0 and Key_Cont = 0, Key release
  *         Key_Trg = 1 and Key_Cont = 1, Key First Push;
  *         Key_Trg = 0 and Key_Cont = 1, Key Not release Or Long Push  
  * @param  None 
  * @retval None
  */
void Key_Read_Fun(void)
{
  uint8 ReadData;
  ReadData = KEY_PORT->IDR ^ 0xFF;
  Key_Trg = ReadData & (ReadData ^ Key_Cont);
  Key_Cont = ReadData;
}

/**
  * @brief  Key process Function.
  * @note   
  * @param  None 
  * @retval None
  */
void Key_Process_Fun(void)
{
  uint8_t Key_Trg_Tmp,Key_Cont_Tmp;
  Key_Trg_Tmp = Key_Trg & 0xF0;   /* bit[3:0] clean */
  Key_Cont_Tmp = Key_Cont & 0xF0;
  
  switch(Key_Cont_Tmp)
  {
    case KEY_POWER_PIN        :  if(Key_Trg_Tmp != KEY_POWER_PIN)
                               {
                                  Key_Power_Proc(); break;
                               }else{
                                 break;
                               }                           
    case KEY_AUTO_PIN       :  if(Key_Trg_Tmp != KEY_AUTO_PIN)
                               { 
                                 Key_Auto_Proc(); break;
                               }else{
                                 break;
                               }
    case KEY_LEVEL_UP_PIN   :  if(Key_Trg_Tmp != KEY_LEVEL_UP_PIN)
                               {
                                 Key_Level_Up_Proc(); break;
                               }else{
                                 break;
                               }
    case KEY_LEVEL_DOWN_PIN :  if(Key_Trg_Tmp != KEY_LEVEL_DOWN_PIN)
                               {
                                 Key_Level_Down_Proc(); break;
                               }else{
                                 break;
                               }
    case KEY_UNLOCK         :  if(Key_Trg_Tmp != KEY_UNLOCK)
                               {
                                 Key_Unlock_Proc(); break;
                               }else{
                                  break;
                               }
    default : break;
  }
}

void Key_Power_Proc(void)
{
  if(Power_Flag == OFF)
  {
    Key_count ++;
    if(Key_count == 200)
    {
      Power_Flag = ON;
      Key_count = 0;
    }
  }else{
    Key_count ++;
    if(Key_count == 200)
    {
      Power_Flag = OFF;
      Key_count = 0;
    }
  }
}

void Key_Auto_Proc(void)
{
  if(Lock_Flag = OFF)
  {
    if(Auto_Flag = OFF)
    {
      Key_count ++;
      if(Key_count == 200)
      {
        Auto_Flag = ON;
        Key_count = 0;
      }
    }else{
      Auto_Flag = OFF;
    }
  }
}

void Key_Level_Up_Proc(void)
{
  Key_count ++;
  if(Key_count == 200)
  {
    HV_Level ++;
    if(HV_Level == 8)
    {
      HV_Level = 8;
    }
    Key_count = 0;
  }
}

void Key_Level_Down_Proc(void)
{
  if(Lock_Flag = OFF)
  {
    if(Down_Flag == OFF)
    {
      Key_count ++;
      if(Key_count == 200)
      {
        Down_Flag = ON;
        Key_count = 0;
      }
    }else{
        Down_Flag = OFF;
      }
  }
}

void Key_Unlock_Proc(void)
{
  if(Power_Flag == ON)
  {
    Key_count ++;
    if(Key_count == 200)
    {
      Lock_Flag = OFF;
      Key_count = 0;
    }
  }else{
    Lock_Flag = ON;
  }
}

void Key_Process_Fun(void)
{
  if(POWER_Flag == ON)
  {
    SET_IO_HIGH(PROBE_POWER_PORT,PROBE_POWER_PIN);      /* open probe power */
    LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);         /* power LED ON*/
    LED_ON(LED_LOCK_PORT,LED_LOCK_PIN);         /* Lock LED ON*/
    ERROR_CHK_Fun();    /* error check, set error flag*/
    //EEPROM_Read_Data();        /* read EEPROM data of times, set falg */
    
  }else{
    
  }
}
    

void Delay_us(uint16_t N)
{
  while(N--)
  {
    TIM4->ARR = 0x04;     /* 8MHz Num = 4, 0.5us + 0.5us = 1us*/
    TIM4->IER = 0x00;     /* disable interrupt */
    TIM4->EGR = 0x01;     /* Re-initializes counter */
    TIM4->SR1 = 0x00;     /* clean flag */
    TIM4->PSCR = 0x00;    /* SET Clk, Fck = 16/2 MHz */
    TIM4->CR1 = 0x01;     /* Start */
    while(!(TIM4->SR1 & 0x01));
    TIM4->SR1 = 0x00;     /* clean flag */
    TIM4->CR1 = 0x00;     /* Stop */
  }
}



