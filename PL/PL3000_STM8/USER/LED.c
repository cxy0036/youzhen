/**
  ******************************************************************************
  * @file    LED.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define LED Control Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LED.h"
#include "KEY_FTC332.h"
#include "GPIO_Config.h"
#include "delay.h"
#include "flash_times.h"

/* Global variable -----------------------------------------------------------*/
extern uint8_t Mode_Trans;
extern uint8_t Mode_Flag;
extern uint8_t Flash_Times_LED_Flag;
extern uint8_t Flash_Times_Level;
extern uint8_t Key_Trg;
extern uint16_t Key_count; 
extern uint8_t Key_Push;
extern uint8_t Key_Cont;
extern uint8_t ERROR_Flag;
extern uint8_t Lowlamp_Flag;
extern uint8_t Skin_Color_Type;
extern uint8_t HV_Level_Dowm_3 ;
extern uint8_t HV_Level_Dowm_5 ;
extern uint8_t Power_Flag;
extern uint8_t HV_Level;

uint8_t LED_Count = 0;
uint8_t k=1;
uint8_t LED_Flash_Long_Count = 0;
uint8_t LED_Flash_Count = 0;
uint16_t Flash_times_LED_Count = 0;


/* Public functions ----------------------------------------------------------*/

/*
********************************************************************************
*                           LED IO INIT FUNCTIONS
*
* Description: This function init LED Port and Pin.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void LED_GPIO_Init(void)
{
  GPIOA->ODR &= ~(LED_MULTI_PIN + LED_ADD_SW_PIN  + LED_MUTE_PIN+ LED_ERROR_PIN );  /* OUT 0 ,LED OFF */
  GPIOA->CR2 &= ~(LED_MULTI_PIN + LED_ADD_SW_PIN  + LED_MUTE_PIN+ LED_ERROR_PIN );  /* Low Speed 2MHz */
  GPIOA->CR1 |= LED_MULTI_PIN + LED_ADD_SW_PIN  + LED_MUTE_PIN+ LED_ERROR_PIN;  /* Push-pull */
  GPIOA->DDR |= LED_MULTI_PIN + LED_ADD_SW_PIN  + LED_MUTE_PIN+ LED_ERROR_PIN;  /* Output */
  
  GPIOB->ODR &= ~(LED_LV8_PIN + LED_AUTO_SW_PIN + LED_DEC_SW_PIN);
  GPIOB->CR2 &= ~(LED_LV8_PIN + LED_AUTO_SW_PIN + LED_DEC_SW_PIN);
  GPIOB->CR1 |= LED_LV8_PIN + LED_AUTO_SW_PIN + LED_DEC_SW_PIN;
  GPIOB->DDR |= LED_LV8_PIN + LED_AUTO_SW_PIN + LED_DEC_SW_PIN;
  
  GPIOC->ODR &= ~(LED_POWER_SW_PIN + LED_LOCK_PIN);
  GPIOC->CR2 &= ~(LED_POWER_SW_PIN + LED_LOCK_PIN);
  GPIOC->CR1 |= LED_POWER_SW_PIN + LED_LOCK_PIN;
  GPIOC->DDR |= LED_POWER_SW_PIN+ LED_LOCK_PIN;
  
  GPIOD->ODR &= ~(LED_LV5_PIN + LED_LV6_PIN + LED_LV7_PIN);
  GPIOD->CR2 &= ~(LED_LV5_PIN + LED_LV6_PIN + LED_LV7_PIN);
  GPIOD->CR1 |= LED_LV5_PIN + LED_LV6_PIN + LED_LV7_PIN;
  GPIOD->DDR |= LED_LV5_PIN + LED_LV6_PIN + LED_LV7_PIN;
  
  GPIOE->ODR &= ~(LED_LV4_PIN + LED_LV3_PIN+ LED_AUTO_PIN);
  GPIOE->CR2 &= ~(LED_LV4_PIN + LED_LV3_PIN+ LED_AUTO_PIN);
  GPIOE->CR1 |= LED_LV4_PIN + LED_LV3_PIN+ LED_AUTO_PIN;
  GPIOE->DDR |= LED_LV4_PIN + LED_LV3_PIN+ LED_AUTO_PIN;
  
  GPIOG->ODR &= ~(LED_LV2_PIN + LED_LV1_PIN);
  GPIOG->CR2 &= ~(LED_LV2_PIN + LED_LV1_PIN);
  GPIOG->CR1 |= LED_LV2_PIN + LED_LV1_PIN;
  GPIOG->DDR |= LED_LV2_PIN + LED_LV1_PIN;
}

/*
********************************************************************************
*                           LED OFF　ALL FUNCTIONS
*
* Description: This function OFF ALL　LED.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void LED_OFF_ALL(void)
{
  //LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
  
  LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
  LED_OFF(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN);
  LED_OFF(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
  LED_OFF(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
  
  LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
  if(Lowlamp_Flag==0)
  {
    LED_OFF(LED_ERROR_PORT,LED_ERROR_PIN);
  } 
  LED_OFF(LED_LOCK_PORT,LED_LOCK_PIN);
  LED_OFF(LED_MUTE_PORT,LED_MUTE_PIN);
  //LED_OFF(LED_LOWLAMP_PORT,LED_LOWLAMP_PIN);
  LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
  
  LED_Level(0);
}

/*
********************************************************************************
*                           LED ON　ALL FUNCTIONS
*
* Description: This function ON ALL　LED.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void LED_ALL_ON(void)
{
  
  LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
  LED_ON(LED_ERROR_PORT,LED_ERROR_PIN);
  LED_ON(LED_LOCK_PORT,LED_LOCK_PIN);
  LED_ON(LED_MUTE_PORT,LED_MUTE_PIN);
  LED_ON(LED_MULTI_PORT,LED_MULTI_PIN);
  //LED_ON(LED_LOWLAMP_PORT,LED_LOWLAMP_PIN);
  
  
  LED_Level(8);
}

/*
********************************************************************************
*                           LED  LEVEL FUNCTIONS
*
* Description: This function LED level.
*
* Arguments  : HV level.
*
* Note(s)    : none.
********************************************************************************
*/
void LED_Level(uint8_t hv_level)
{
  if((Skin_Color_Type == 1)&&(Power_Flag == ON)&&(!ERROR_Flag) && (!Lowlamp_Flag))
  {       
    return ;  
  }
  
  switch(hv_level){
  case 1:  
            if(Flash_Times_Level != 1)
            {
             LED_ON(LED_LV1_PORT,LED_LV1_PIN);
            }
            LED_OFF(LED_LV2_PORT,LED_LV2_PIN);
            LED_OFF(LED_LV3_PORT,LED_LV3_PIN);
            LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
            LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
            LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
            LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
            LED_OFF(LED_LV8_PORT,LED_LV8_PIN);   
          break;
  case 2:  
            if(Flash_Times_Level != 1)
            {
             LED_ON(LED_LV1_PORT,LED_LV1_PIN);
            }
            LED_ON(LED_LV2_PORT,LED_LV2_PIN);
            LED_OFF(LED_LV3_PORT,LED_LV3_PIN);
            LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
            LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
            LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
            LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
            LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 3: 
           if(HV_Level_Dowm_3 != 1)
           {
             if(Flash_Times_Level != 1)
              {
               LED_ON(LED_LV1_PORT,LED_LV1_PIN);
              }
              LED_ON(LED_LV2_PORT,LED_LV2_PIN);
              LED_ON(LED_LV3_PORT,LED_LV3_PIN);    
           }else{   
                  Flash_times_LED_Count++;  
                  if(Flash_times_LED_Count == 18)
                  { 
                    Flash_times_LED_Count = 0;
                    LED_TOG(LED_LV1_PORT,LED_LV1_PIN);
                    LED_TOG(LED_LV2_PORT,LED_LV2_PIN);
                    LED_TOG(LED_LV3_PORT,LED_LV3_PIN);
                  }
           }
            LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
            LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
            LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
            LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
            LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  case 4: 
            if(Flash_Times_Level != 1)
            {
             LED_ON(LED_LV1_PORT,LED_LV1_PIN);
            }
            LED_ON(LED_LV2_PORT,LED_LV2_PIN);
            LED_ON(LED_LV3_PORT,LED_LV3_PIN);
            LED_ON(LED_LV4_PORT,LED_LV4_PIN);
            LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
            LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
            LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
            LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
         
          break;
  case 5:
           if(HV_Level_Dowm_5 != 1)
           {
             if(Flash_Times_Level != 1)
              {
               LED_ON(LED_LV1_PORT,LED_LV1_PIN);
              }
              LED_ON(LED_LV2_PORT,LED_LV2_PIN);
              LED_ON(LED_LV3_PORT,LED_LV3_PIN);
              LED_ON(LED_LV4_PORT,LED_LV4_PIN);
              LED_ON(LED_LV5_PORT,LED_LV5_PIN);
           }else{
                  Flash_times_LED_Count++;  
                  if(Flash_times_LED_Count == 18)
                  { 
                    Flash_times_LED_Count = 0;
                    LED_TOG(LED_LV1_PORT,LED_LV1_PIN);
                    LED_TOG(LED_LV2_PORT,LED_LV2_PIN);
                    LED_TOG(LED_LV3_PORT,LED_LV3_PIN);
                    LED_TOG(LED_LV4_PORT,LED_LV4_PIN);
                    LED_TOG(LED_LV5_PORT,LED_LV5_PIN);
                  }
           }
            
            LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
            LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
            LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
         
          break;
  case 6: 
            if(Flash_Times_Level != 1)
            {
             LED_ON(LED_LV1_PORT,LED_LV1_PIN);
            }
            LED_ON(LED_LV2_PORT,LED_LV2_PIN);
            LED_ON(LED_LV3_PORT,LED_LV3_PIN);
            LED_ON(LED_LV4_PORT,LED_LV4_PIN);
            LED_ON(LED_LV5_PORT,LED_LV5_PIN);
            LED_ON(LED_LV6_PORT,LED_LV6_PIN);
            LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
            LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
         
          break;
  case 7:  
            if(Flash_Times_Level != 1)
            {
             LED_ON(LED_LV1_PORT,LED_LV1_PIN);
            }
            LED_ON(LED_LV2_PORT,LED_LV2_PIN);
            LED_ON(LED_LV3_PORT,LED_LV3_PIN);
            LED_ON(LED_LV4_PORT,LED_LV4_PIN);
            LED_ON(LED_LV5_PORT,LED_LV5_PIN);
            LED_ON(LED_LV6_PORT,LED_LV6_PIN);
            LED_ON(LED_LV7_PORT,LED_LV7_PIN);
            LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
         
          break;
  case 8: 
            if(Flash_Times_Level != 1)
            {
             LED_ON(LED_LV1_PORT,LED_LV1_PIN);
            }
            LED_ON(LED_LV2_PORT,LED_LV2_PIN);
            LED_ON(LED_LV3_PORT,LED_LV3_PIN);
            LED_ON(LED_LV4_PORT,LED_LV4_PIN);
            LED_ON(LED_LV5_PORT,LED_LV5_PIN);
            LED_ON(LED_LV6_PORT,LED_LV6_PIN);
            LED_ON(LED_LV7_PORT,LED_LV7_PIN);
            LED_ON(LED_LV8_PORT,LED_LV8_PIN);
         
          break;
  default:
             if(!(Lowlamp_Flag))
            {
             LED_OFF(LED_LV1_PORT,LED_LV1_PIN);
            }       
            LED_OFF(LED_LV2_PORT,LED_LV2_PIN);
            LED_OFF(LED_LV3_PORT,LED_LV3_PIN);
            LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
            LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
            LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
            LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
            LED_OFF(LED_LV8_PORT,LED_LV8_PIN);
          break;
  }
  
}

/*
********************************************************************************
*                         MULTI MODEL LED FLASH FUNCTIONS
*
* Description: This function multi mode LED flash .
*
* Arguments  : none.
*
* Note(s)    : flash three times ,each time about 1s.
********************************************************************************
*/
void LED_Multi_Disply(void)
{
  //if((Mode_Flag == AUTO_MULTI_MODE) || (Mode_Flag == STANDARD_MULTI_MODE))
if((Mode_Flag == AUTO_MULTI_MODE) || (Mode_Flag == STANDARD_MULTI_MODE) || (Mode_Trans == AUTO_MULTI_MODE) || (Mode_Trans == STANDARD_MULTI_MODE))
  {   
   if(Mode_Flag == AUTO_MULTI_MODE || (Mode_Trans == AUTO_MULTI_MODE))
    {
         if((Mode_Flag == AUTO_MODE)|| (Mode_Flag == AUTO_MULTI_MODE))
        {
            if(HV_Level >= 5)
            {
              HV_Level = 5; 
            }
        }
      LED_ON(LED_MULTI_PORT,LED_MULTI_PIN);
      LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
    }else{
     LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
     LED_ON(LED_MULTI_PORT,LED_MULTI_PIN);
    }    
  }else{
   if((Mode_Flag == AUTO_MODE) || (Mode_Trans == AUTO_MODE))
    {
              if((Mode_Flag == AUTO_MODE)|| (Mode_Flag == AUTO_MULTI_MODE))
        {
            if(HV_Level >= 5)
            {
              HV_Level = 5; 
            }
        }
      LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
      LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
    }
    
   //if(Mode_Flag == STANDARD_MODE) 
   if((Mode_Flag == STANDARD_MODE) || (Mode_Trans == STANDARD_MODE))
    {
      LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
      LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
    }
  }
}


void Flash_Times_LED(void)
{ 
   IWDG->KR = 0xAA ; 
   if(LED_Count != 20)
   {
     LED_Count ++ ; 
   }

  if((Skin_Color_Type == 5)&&(!ERROR_Flag) && (!Lowlamp_Flag)&&(LED_Count == 20) )
  {           
         //first off all level led
   /*      LED_OFF(LED_LV1_PORT,LED_LV1_PIN);
         LED_OFF(LED_LV2_PORT,LED_LV2_PIN);
         LED_OFF(LED_LV3_PORT,LED_LV3_PIN);
         LED_OFF(LED_LV4_PORT,LED_LV4_PIN);
         LED_OFF(LED_LV5_PORT,LED_LV5_PIN);
         LED_OFF(LED_LV6_PORT,LED_LV6_PIN);
         LED_OFF(LED_LV7_PORT,LED_LV7_PIN);
         LED_OFF(LED_LV8_PORT,LED_LV8_PIN);  
     */    Flash_times_LED_Count++; 
        if(Flash_times_LED_Count == 18)
      {   
      /*   LED_TOG(LED_LV1_PORT,LED_LV1_PIN);
         LED_TOG(LED_LV2_PORT,LED_LV2_PIN);
         LED_TOG(LED_LV3_PORT,LED_LV3_PIN);
         LED_TOG(LED_LV4_PORT,LED_LV4_PIN);
         LED_TOG(LED_LV5_PORT,LED_LV5_PIN);
         LED_TOG(LED_LV6_PORT,LED_LV6_PIN);
         LED_TOG(LED_LV7_PORT,LED_LV7_PIN);
         LED_TOG(LED_LV8_PORT,LED_LV8_PIN);
       */
        LED_Level(HV_Level);
        Flash_times_LED_Count = 0; 
         LED_Count = 1;
         Delay_ms(100); 
      }
  }
  if (Flash_Times_Level == 1)
  {           
        Flash_times_LED_Count++;  
        if(Flash_times_LED_Count == 18)
      {
        LED_TOG(LED_LV1_PORT,LED_LV1_PIN);
        //LED_OFF(LED_LV1_PORT,LED_LV1_PIN);
        Flash_times_LED_Count = 0;
        Flash_Times_LED_Flag = 0;
      }
  }
   //if ((Flash_Times_Level == 9) || (Lowlamp_Flag))
   if (Flash_Times_Level == 9)
   {
      Flash_times_LED_Count++;
      if(Flash_times_LED_Count == 18)
      {
        LED_TOG(LED_ERROR_PORT,LED_ERROR_PIN);
        LED_TOG(LED_LV1_PORT,LED_LV1_PIN);
        Flash_times_LED_Count = 0;
        //Flash_Times_LED_Flag = 0;
      }
    }
  //}
  if((Flash_Times_LED_Flag) &&(Flash_Times_Level > 1) &&(Flash_Times_Level < 9))
  {              
    for(k=1;k < Flash_Times_Level+1;k++)
    {  
     LED_Level(k);
     Delay_ms(90); 
     IWDG->KR = 0xAA;
     Key_Read();
    if(Key_Trg == KEY_POWER)  // first push 
    { 
         LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
         Key_count = 0;  // clean key delay count 
         Key_Push = KEY_POWER;  // set push flag is power key 
     }  
    }
    
    for(k=Flash_Times_Level;k > 0;k--)
    {  
     LED_Level(k);
     Delay_ms(90);
     IWDG->KR = 0xAA;
     Key_Read();
    if(Key_Trg == KEY_POWER)  // first push 
    { 
         LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
         Key_count = 0;  // clean key delay count 
         Key_Push = KEY_POWER;  // set push flag is power key 
     }  
     if(!(Key_Cont & KEY_POWER))  // KEY_POWER release
     {
        if(ERROR_Flag ==0)
        {
          LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
        }
      }
    }
   
     for(k=1;k < Flash_Times_Level+1;k++)
    {  
     LED_Level(k);
     Delay_ms(90); 
     IWDG->KR = 0xAA;
    Key_Read();
    if(Key_Trg == KEY_POWER)  // first push 
    { 
         LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
         Key_count = 0;  // clean key delay count 
         Key_Push = KEY_POWER;  // set push flag is power key 
     }  
     if(!(Key_Cont & KEY_POWER))  // KEY_POWER release
     {
        if(ERROR_Flag ==0)
        {
          LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
        }
      }
    }
    
    for(k=Flash_Times_Level;k > 0;k--)
    {  
     LED_Level(k);
     Delay_ms(90);
     IWDG->KR = 0xAA;
     Key_Read();
     if(Key_Trg == KEY_POWER)  // first push 
      { 
           LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
           Key_count = 0;  // clean key delay count 
           Key_Push = KEY_POWER;  // set push flag is power key 
       }  
      if(!(Key_Cont & KEY_POWER))  // KEY_POWER release
       {
          if(ERROR_Flag ==0)
          {
            LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
          }
        }
    }   
    Flash_Times_LED_Flag = 0;
    Flash_times_LED_Count = 0;
    LED_Level(0);
  }
}

/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/


