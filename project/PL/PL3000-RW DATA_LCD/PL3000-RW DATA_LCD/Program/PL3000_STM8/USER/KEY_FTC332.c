/**
  ******************************************************************************
  * @file    KEY_FTC332.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define KEY Functions.
   ******************************************************************************
  * @attention
  * this file is FTC332 driver and Key function
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "KEY_FTC332.h"
#include "UART.h"
#include "stdio.h"
#include "GPIO_Config.h"
#include "LED.h"
#include "ADC.h"
#include "HV_Charge.h"
#include "delay.h"
#include "error_chk.h"
#include "flash_times.h"
#include "protocol.h"

/* Global variable -----------------------------------------------------------*/
uint8_t Key_Value_Error = 0;  /* Key read data error flag */
uint8_t Start_Flag = NO;  /* FTC332 data Start Flag */
uint8_t Key_Read_OK = 0; /* Read One Fram Key_Value OK flag */
uint8_t Bit_Time_H, Bit_Time_L; /* one key value bit timer count data */
uint8_t Bit_Count = 0;  /* count read bit num */
//uint8_t Skin_Color_Type_Count = 0;
//uint8_t Skin_Color_Type_Count_0 = 0;
uint8_t Skin_Color_Type_Count_1 = 0;
uint8_t Skin_Color_Type_Count_3 = 0;
uint8_t Skin_Color_Type_Count_4 = 0;
uint16_t Skin_Color_Type_Count_5 = 0;
//uint8_t Flash_Ready_count=0;

//uint8_t Flash_Times_Level_10_count = 0;

uint8_t Key_Int = 0;  /* Key into Interrupt flag */

uint8_t Key_Trg = 0;
volatile uint8_t Key_Cont = 0;
uint16_t Key_count = 0;  /* key read Function var */
//uint8_t Key_count_Long = 0;

uint8_t Key_Byte_Value = 0x00; /* key value to a byte */

uint8_t Power_Flag = OFF;  /* POWER Flag, defualt OFF */
uint8_t Mute_Flag = OFF;  /* MUTE Flag, defualt OFF */
uint8_t Lock_Flag = ON;  /* LOCK Flag, defualt ON */
uint8_t Mode_Flag = STANDARD_MODE;   /* Mode Flag, defualt Standard mode */

uint16_t time_out_power_off = 0;  /* Timeout Power off count */
uint8_t Fisrt_Push = 1; /* First push flag */

uint8_t HV_Level = 0;         /* HV Level count */
uint32_t Flash_Count ;  /* flash times */
uint8_t Lowlamp_Flag = 0; /* lowlam flag */

uint8_t ERROR_Flag = 0; /* all error flag */
uint8_t Key_Push = 0; /* key push flag it is use to long push judgment */
uint16_t Read_HV;  /* read HV vlaue */

//uint8_t Off_Flash_Flag=1;
uint8_t First_Flash;
uint8_t Flash_Times_LED_Flag = 0;
extern uint8_t Power_LED_Start_Flag;
uint8_t Flash_Flag = 0;
uint8_t Mode_Trans=0;
uint8_t Level_Change = 0;

extern uint8_t Skin_Color_Type ;
extern uint8_t ERROR_Probe_FAN_Flag; /* probe fan error flag */
extern uint8_t Full_Charge_Flag; /* full charge flag */
extern uint8_t Flash_ERROR;  /*Flash error flag*/
extern uint8_t Full_Charge_Beep_Flag;
extern uint8_t Charge_Time_Count;  /* clean charge time count */
extern uint8_t Discharge_Flag;
extern uint8_t Probe_Error_Flag;
extern uint8_t Lamp_Type;
extern uint8_t Flash_Ready;
extern uint8_t Flash_Flag ;
extern uint8_t Flash_Times_Level;
extern uint8_t LED_Flash_Long_Count;
extern uint8_t LED_Flash_Count;
extern uint8_t Flash_times_LED_Count;
extern uint8_t Recevie_Data_Error;
extern uint8_t ERROR_Flag_MainFan;
extern uint8_t Charge_TimeOut_Flag;
extern uint8_t Charge_Overflow_error;

uint8_t plus_times;

/* Public functions ----------------------------------------------------------*/

/*
********************************************************************************
*                           KEY INIT FUNCTIONS
*
* Description: This function init key port and timer.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Key_Init_Fun(void)
{
  disableInterrupts();
  
  KEY_PORT->DDR &= ~KEY_PIN; //PB5 input
  KEY_PORT->CR1 |=  KEY_PIN; //pull up
  KEY_PORT->CR2 &= ~KEY_PIN; //disable externnal interrupt  
  
 // ITC->ISPR2 &= (3<<0);         /* clean priority */
 // ITC->ISPR2 |= (1<<0);         /* set PB Port priority 1 */
  
  EXTI->CR1 = 0x04;                /* set PB rising edge */
  Key_Timer_Init_Fun();
  enableInterrupts();
}

/*
********************************************************************************
*                           KEY TIMER INIT　FUNCTIONS
*
* Description: This function init key　timer　config.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Key_Timer_Init_Fun(void)
{
  KEY_TIMER->IER = 0x00;           /* disable all interrupt */
  KEY_TIMER->CNTRH = 0x00;
  KEY_TIMER->CNTRL = 0x00; 
  KEY_TIMER->PSCR = 0x04;          /* set prescaler : 4 Fck_cnt = 16MHz/32 = 0.5MHz */
  KEY_TIMER->EGR |=0x01;           /* Re-initializes counter and prescaler */      
  KEY_TIMER->CR1 = 0x08;           /* set auto reload, disable counter */
}

/*
********************************************************************************
*                           KEY READ　FUNCTIONS
*
* Description: This function read a frame key value form FTC332.
*              And read flash key and touch key.
*
* Arguments  : none.
*
* Note(s)    : if read key value error, set key read error flag.
********************************************************************************
*/
void Key_Read(void)
{
  uint8_t t = 0x01;
  uint16_t timeout = KEY_READ_TIME_OUT;
  uint16_t Bit_Time = 0;  /* 16 bit timer count data */

  KEY_PORT->CR2 |= KEY_PIN; //enable externnal interrupt
  while(!Key_Read_OK)
  { 
   timeout--;
   if(timeout <= 0)
    {
      KEY_PORT->CR2 &= ~KEY_PIN; //disable externnal interrupt 
      Key_Read_OK = 0;
      break;
    }
    if(Key_Int == 1)
    {
      Key_Int  = 0;    
      Bit_Time = Bit_Time_H;
      Bit_Time = (Bit_Time << 8) | Bit_Time_L;     /* read count data */       
      if(!Start_Flag)
      {
        if((Bit_Time > KEY_START_BIT_THRL) && (Bit_Time < KEY_START_BIT_THRH))
        { 
          Start_Flag = YES;
        }
      }else{
        if(Bit_Count < 8)
        {
          if((Bit_Time > KEY_H_BIT_THRL) && (Bit_Time < KEY_H_BIT_THRH))
          {
            Key_Byte_Value |= t;
          }else{
            if((Bit_Time > KEY_L_BIT_THRL) && (Bit_Time < KEY_L_BIT_THRH))
            {
              Key_Byte_Value &=~t;
            }else{
              Key_Byte_Value &=~t;   /* if read data error, set key not push */
            }
          }
          t = t << 1; 
          Bit_Count ++;
        }else{
          Bit_Count = 0;
          Key_Read_OK = 1;
          Start_Flag = NO;
          KEY_TIMER->CNTRH = 0;
          KEY_TIMER->CNTRL = 0;
          KEY_TIMER->CR1 = 0x08;           /* disable counter, stop count */
        }
      }
    }
  }
  KEY_PORT->CR2 &= ~KEY_PIN; //disable externnal interrupt 
  Key_Read_OK = 0;
  Key_Analysis();
}

/*
********************************************************************************
*                           KEY ANALYSIS　FUNCTIONS
*
* Description: This function analysis the key value form FTC332 whether error.
*
* Arguments  : none.
*
* Note(s)    : if read key value error, set key read error flag.
********************************************************************************
*/
void Key_Analysis(void)
{
  uint8_t h;
  h = (Key_Byte_Value & 0xf0) >> 4;   /* analysis ftc322 key value, if error set error flag */
  if(h != (~Key_Byte_Value & 0x0f))
  {
    Key_Value_Error = 1;
  }else{  /* read value not error  */
    Key_Value_Error = 0;
    Key_Byte_Value = Key_Byte_Value & 0x0f;
    Key_Trg = Key_Byte_Value & (Key_Byte_Value ^ Key_Cont);
    Key_Cont = Key_Byte_Value;
  }
}

/*
********************************************************************************
*                           KEY PROCESS　FUNCTIONS
*
* Description: This function Process key read value.
*
* Arguments  : none.
*
* Note(s)    : if read key value error, set key read error flag.
********************************************************************************
*/
void Key_Process_Fun(void)
{ 
  Level_Change = 0;
  if(Key_Trg == KEY_POWER)  /* first push */
  { 
       LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
       Key_count = 0;  /* clean key delay count */
       Key_Push = KEY_POWER;  /* set push flag is power key */
   }
  if(!(Key_Cont & KEY_POWER))  /* KEY_POWER release*/
      {
         if(ERROR_Flag ==0)
         {
           LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
         }
      }
  if(Power_Flag == ON)  /* if power on, process another key */
  {       
    if(Key_Cont == KEY_POWER) /* if power key push */
    {     
      if(Key_Trg == KEY_POWER)  /* first push */
      { 
       LED_OFF(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
       Key_count = 0;  /* clean key delay count */
       Key_Push = KEY_POWER;  /* set push flag is power key */
      }else{
       if(Key_Push == KEY_POWER)
       {
         Key_Power_Off_Proc();  
       }
      }
    }
    if(Lock_Flag == ON)  /* if lock on, only process unlock key */
    {
      if(!(Key_Cont & KEY_POWER))  /* KEY_POWER release*/
      {
         if(ERROR_Flag ==0)
         {
           LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
         }
      }
      
      if(Key_Cont == KEY_UNLOCK)
      {       
         if((Key_Trg == KEY_ADD) ||(Key_Trg == KEY_DEC) || (Key_Trg == KEY_UNLOCK))
         {
           Key_count = 0;
           Key_Push = KEY_UNLOCK;
           LED_OFF(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
           LED_OFF(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
         }else{
           if(Key_Push == KEY_UNLOCK)
           {
             Key_Unlock_Proc();
           }
         }
      }
      if((Key_Push != KEY_POWER)&&(ERROR_Flag ==0)) /* if power key not push ,if unlock, can power off*/
      {       
        if(!(Key_Cont & KEY_UNLOCK) || !(Key_Cont & KEY_ADD) || !(Key_Cont & KEY_DEC))
        {  /* if unlock realse */
          Key_Push = 0;  
          LED_ON(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
          LED_ON(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
        }
      }
    }else{  /* if lock OFF, process all key */
      switch(Key_Cont)
      {
        case KEY_AUTO   :  if(Key_Trg == KEY_AUTO)  /* auto  key, first push*/ 
                           { 
                             Key_count = 0;
                             Key_Push = KEY_AUTO;
                             /*
                            if(Mute_Flag == OFF)
                             {
                               BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
                             } */
                             BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
                             LED_OFF(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN);
                           }else{  /* auto key, long push*/                                 
                             if(Key_Push == KEY_AUTO)
                             {
                               Key_Mode_Proc();
                             }
                             if(!Full_Charge_Beep_Flag)  /* if full charge beep on not off */
                             {
                               BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);
                             }                             
                           } break;
        case KEY_ADD    :  if(Key_Trg == KEY_ADD)  /* ADD key, first push*/ 
                           {
                             Key_count = 0;
                             /*
                            if(Mute_Flag == OFF)
                             {
                               BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
                             } */
                             BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
                             LED_OFF(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
                             Key_Level_Up_Proc();
                           }else{  /* add key, long push*/
                             if(!Full_Charge_Beep_Flag)
                             {
                               BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);
                             }
                           }break;
        case KEY_DEC    :  if(Key_Trg == KEY_DEC)
                           {
                             Key_count = 0;
                             Key_Push = KEY_DEC;
                             /*
                            if(Mute_Flag == OFF)
                             {
                               BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
                             } */
                             BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
                             LED_OFF(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
                            // Key_Level_Down_Proc(); 
                           }else{
                             if(Key_Push == KEY_DEC)
                             {
                               Key_Mute_Proc();
                             }
                             if(!Full_Charge_Beep_Flag)
                             {
                               BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);  
                             }                             
                           }break;
        default : //Key_count = 0;
                  if(!Full_Charge_Beep_Flag)  /* if full charge beep on not off */
                  {
                    BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);/* another status off beep */
                  }
                  break;
      }
      if(!(Key_Cont & KEY_AUTO))  /* auto key release*/
      {
        if(ERROR_Flag ==0)
        {
          LED_ON(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN);
        }
         
        if(Key_Push == KEY_AUTO) 
        {
          Key_Push = 0;
          if(Key_count < 80) 
          {
            if(Lock_Flag == OFF)
            {             
             // printf("mode is : %d\r\n",Model_Flag);
              Level_Change = 1;
              Full_Charge_Flag = 0;
              Charge_Time_Count = 0;  
              LED_Flash_Long_Count = 0;
              LED_Flash_Count = 0;
              switch(Mode_Flag){
              case STANDARD_MODE:  
                          Mode_Flag = AUTO_MODE;
                          //LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
                          //LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
                          if(HV_Level > 5)
                          {
                            HV_Level = 5;
                          };break;
              case STANDARD_MULTI_MODE: 
                         Mode_Flag = STANDARD_MODE;
                         //LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
                         //LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
                         break;
              case AUTO_MODE:
                          Mode_Flag = STANDARD_MODE;
                          //LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
                          //LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);      
                          break;
              case AUTO_MULTI_MODE: 
                          Mode_Flag = STANDARD_MODE;
                          //LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
                          //LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
                          break;
              default : break;
              }   
            }
          }      
        }
        
      }  
       if(!(Key_Cont & KEY_POWER))  /* KEY_POWER release*/
      {
        if(ERROR_Flag ==0)
        {
         LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);
        }
      }
      
      
      if(!(Key_Cont & KEY_ADD))  /* ADD key release*/
      {
        if(ERROR_Flag ==0)
        {
        LED_ON(LED_ADD_SW_PORT,LED_ADD_SW_PIN);
        }
      }
      
      if(!(Key_Cont & KEY_DEC))  /* DEC key release*/
      {
        if(ERROR_Flag ==0)
        {
        LED_ON(LED_DEC_SW_PORT,LED_DEC_SW_PIN);
        }
        if(Key_Push == KEY_DEC) /* befor auto key push */
        {
          Key_Push = 0;
          if(Key_count < 60)  /* push time short push, auto key function is model change*/
          {
            if(Lock_Flag == OFF)
            {             
              Key_Level_Down_Proc();              
            }
          }      
        }
      }
    }
    if(Key_Cont == 0)/* if power on, not any operation, delay 10min power off */
    {  
      time_out_power_off++;
      if(time_out_power_off == TIME_OUT_10M)
      {
        //time_out_power_off = 0;
        Key_count = 34;  /* trig power off */
        Key_Power_Off_Proc();
        
      }
    }else{
      time_out_power_off = 0;
    }
  }else{  /* if power off, check power on key */
    if(Key_Cont == KEY_POWER)  /* power key push */
    {
     if(Key_Trg == KEY_POWER)  /* first push */
     {
       Key_count = 0;    /* clean key delay count */
       Key_Push = 0;    
      // Key_Push = KEY_POWER;  /* set push flag is power key */
     }else{  /* power key long push */
       if(!Key_Push) /* if power off not realese, dont power on */
       {
        Key_Power_On_Proc();   /* power on function */        
       }                    
     }
    }
  }
}

/*
********************************************************************************
*                           KEY POWER ON FUNCTIONS
*
* Description: This function Process power on.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Key_Power_On_Proc(void)
{
  Key_count ++;
  if(Key_count == 40)  /* 1s long push */
  {
    //LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);      /* power LED ON*/
    BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN); /* beep on 0.2s */
    SET_IO_HIGH(PROBE_POWER_PORT,PROBE_POWER_PIN);  /* open probe power */
    LED_ALL_ON();  /* all led on 1s */
    IWDG->KR = 0xAA;
    Delay_ms(200);
    IWDG->KR = 0xAA;
    BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);
    Delay_ms(800);
    IWDG->KR = 0xAA;    
    LED_OFF_ALL();
    //LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);      /* power LED ON*/
   /* if(ERROR_Flag == 0)
     {  
      LED_ON(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN);
     }*/
    LED_ON(LED_LOCK_PORT,LED_LOCK_PIN);         /* Lock LED ON*/
    SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */  
    Power_Flag = ON;
    First_Flash = 1;
    Mode_Flag = STANDARD_MODE;         
    Recevie_Data_Error = 0;
    ERROR_Flag_MainFan = 0;
    Probe_Error_Flag  = 0;
    Flash_ERROR = 0;
    Charge_TimeOut_Flag = 0;
    Charge_Overflow_error = 0;
    Delay_ms(50);
    Lamp_Type_Check();
    Flash_times_LED_Count = 0 ;
    Delay_ms(2);
    Get_Flash_Times_Level();
    /*if(Flash_Times_Level == 9)
    {                   
        Lowlamp_Flag = 1;
     
    }else{
      Lowlamp_Flag = 0;
    }*/
    Flash_Times_LED_Flag = 1;
    //Power_LED_Start_Flag = 0;
    //Flash_Times_LED_Indicator();
    Key_count = 0;
    Key_Push = 0;
  }
}

/*
********************************************************************************
*                           KEY POWER OFF FUNCTIONS
*
* Description: This function Process power OFF.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Key_Power_Off_Proc(void)
{
  Key_count ++;
  if(Key_count == 35)  /* 1s long push */
  {
    Power_Flag = OFF;    
    SET_IO_LOW(PROBE_POWER_PORT,PROBE_POWER_PIN);      /* Off probe power */
    SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);      /* OFF HV */
    SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); /* OFF Flash */

    TIM1->SR1 = 0x00; //disable pid timer
    TIM1->CR1 = 0x80; // disable counter
    //init all var;
    Full_Charge_Flag = 0;
    Power_Flag = OFF;       /* POWER Flag, defualt OFF */
    Mute_Flag = OFF;        /* MUTE Flag, defualt OFF */
    Mode_Flag = STANDARD_MODE; /* AUTO Flag, defualt Standard model */
    Charge_Time_Count = 0;
    Lock_Flag = ON;         /* LOCK Flag, defualt ON */
    ERROR_Flag = 0;  
    HV_Level = 0;
    Key_count = 0;
    Flash_times_LED_Count = 0;
    Flash_Times_LED_Flag = 0;
    BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
    if(time_out_power_off)
    {
      Delay_ms(500);
      IWDG->KR = 0xAA;
      Delay_ms(500);
      IWDG->KR = 0xAA;
      Delay_ms(700);
      IWDG->KR = 0xAA;
    }
    Delay_ms(200);
    BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);
    SET_IO_LOW(BEEP_PORT,BEEP_PIN);
    Flash_Times_Level = 0;
    time_out_power_off = 0;
    Power_LED_Start_Flag = 1 ;
    LED_OFF_ALL();
  }
}


/*
********************************************************************************
*                           ERROR POWER OFF FUNCTIONS
*
* Description: This function Process power OFF.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Error_Power_Off_Proc(void)
{
  
  if(ERROR_Flag)  
  {
   // Power_Flag = OFF;    
    SET_IO_LOW(PROBE_POWER_PORT,PROBE_POWER_PIN);      /* Off probe power */
    SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);      /* OFF HV */
    SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); /* OFF Flash */

    LED_OFF_ALL();
  }
}





/*
********************************************************************************
*                           KEY LEVEL ADD FUNCTIONS
*
* Description: This function Process level add.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Key_Level_Up_Proc(void)
{    
  //Discharge_Flag = 0;
  //if((Mode_Flag == STANDARD_MULTI_MODE) || (Mode_Flag == AUTO_MULTI_MODE))
  //{
    //HV_Level = 1;
 // }else{
   Level_Change = 1;
   if(First_Flash || (Mode_Flag == AUTO_MODE)|| (Mode_Flag == AUTO_MULTI_MODE))
    {
     if(HV_Level >= 5)
     {
        HV_Level = 5;
        //beep two;
      }else{
        HV_Level ++;
        Charge_Time_Count = 0;  /* clean charge time count */
        Full_Charge_Flag = 0;
      }        
    }else{
      if(HV_Level == 8)
      {
        HV_Level = 8;
      }else{
        HV_Level ++;
        Charge_Time_Count = 0;  /* clean charge time count */
        Full_Charge_Flag = 0;
      }
    }
  //}
}

/*
********************************************************************************
*                           KEY LEVEL DEC FUNCTIONS
*
* Description: This function Process level DEC.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Key_Level_Down_Proc(void)
{ 
  Level_Change = 1;
  if(HV_Level == 1)
  {
    HV_Level = 1;
    //Discharge_Flag = 0;
  }else{
    HV_Level --;
    Full_Charge_Flag = 0;
    Charge_Time_Count = 0;  /* clean charge time count */
    //Discharge_Flag = 1;
  }
}

/*
********************************************************************************
*                           KEY MUTE FUNCTIONS
*
* Description: This function Process MUTE.
*
* Arguments  : none.
*
* Note(s)    : if mute, set mute flag.
********************************************************************************
*/
void Key_Mute_Proc(void)
{
  Key_count++;
  if(Key_count == 60)
  {
    Key_Push = 0;
    if(Mute_Flag == ON)
    {
      Mute_Flag = OFF;
      LED_OFF(LED_MUTE_PORT,LED_MUTE_PIN);
      BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN); 
      Delay_ms(20);
    }else{
      Mute_Flag = ON;
      LED_ON(LED_MUTE_PORT,LED_MUTE_PIN);
    }    
  }
}

void Key_Mode_Proc(void)
{
  Key_count++;     
   /* if(Key_count < 35)
    {
       if(Lock_Flag == OFF)
        {                      
         Full_Charge_Flag = 0;
         Charge_Time_Count = 0;  
         LED_Flash_Long_Count = 0;
         LED_Flash_Count = 0;
          if(Mode_Flag == STANDARD_MULTI_MODE)
          {
            Mode_Flag = STANDARD_MODE;
            LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
          }
          if(Mode_Flag == AUTO_MULTI_MODE)
          {
           LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
           LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
           Mode_Flag = AUTO_MODE; 
           if(HV_Level > 5)
            {
              HV_Level = 5;
            }    
          }             
        }       
     }      
   
  if(Key_count < 35)
  {
    if(Lock_Flag == OFF)
    {             
             // printf("mode is : %d\r\n",Model_Flag);
              //Mode_Change = 1;
              Full_Charge_Flag = 0;
              Charge_Time_Count = 0;  
              LED_Flash_Long_Count = 0;
              LED_Flash_Count = 0;              
    switch(Mode_Flag){
      case STANDARD_MODE:  
                  Mode_Flag = AUTO_MODE;
                  LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
                  LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
                  if(HV_Level > 5)
                  {
                    HV_Level = 5;
                  }  ;break;
      case STANDARD_MULTI_MODE: 
                 Mode_Flag = STANDARD_MODE;
                 LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
                 break;
      case AUTO_MODE:
                  LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
                  LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
                  Mode_Flag = STANDARD_MODE;
                  break;
      case AUTO_MULTI_MODE: 
                  Mode_Flag = AUTO_MODE;
                  LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN);
                  if(HV_Level > 5)
                  {
                    HV_Level = 5;
                  }
                  break;
      default : break;
      }    
     if(Mute_Flag == OFF)
      {
       BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
       Delay_ms(20);
      
       }
    }
  }     */ 
  if(Key_count == 80)
  {
    Level_Change = 1;
    Key_Push = 0;
    switch(Mode_Flag){
    case STANDARD_MODE: Mode_Flag = STANDARD_MULTI_MODE;
                        //LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
                        //LED_ON(LED_MULTI_PORT,LED_MULTI_PIN);            
                        break;
    //case STANDARD_MULTI_MODE: Mode_Flag = STANDARD_MODE;break;
    case AUTO_MODE: Mode_Flag = AUTO_MULTI_MODE;
                    //LED_ON(LED_MULTI_PORT,LED_MULTI_PIN);
                    //LED_ON(LED_AUTO_PORT,LED_AUTO_PIN);
                    if(HV_Level > 5)
                    {
                     HV_Level = 5;
                     };
                    break;
    //case AUTO_MULTI_MODE: Mode_Flag = AUTO_MODE;break;
    default : Mode_Flag = STANDARD_MODE;
              //LED_OFF(LED_AUTO_PORT,LED_AUTO_PIN);
              //LED_OFF(LED_MULTI_PORT,LED_MULTI_PIN); break;
    }
    LED_Flash_Long_Count = 0;
    LED_Flash_Count = 0;
    Full_Charge_Flag = 0;
    Charge_Time_Count = 0;
    if(Mute_Flag == OFF)
    {
      BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
      Delay_ms(40);
      if(Mode_Flag != STANDARD_MODE)
      { 
        BEEP_Cmd(DISABLE);
        Delay_ms(40);
        BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN); 
        Delay_ms(40);
      }
    }
  } 
}

/*
********************************************************************************
*                           KEY UNLOCK FUNCTIONS
*
* Description: This function Process unlock.
*
* Arguments  : none.
*
* Note(s)    : Unlock lock flag.
********************************************************************************
*/
void Key_Unlock_Proc(void)
{
  if((Lock_Flag == ON) && (!ERROR_Flag) && (!Lowlamp_Flag))
  {
    Key_count ++;
    if(Key_count == 40)
    {
      Lock_Flag = OFF;
      Key_Push = 0;
      BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
      Delay_ms(100);
      BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);
      LED_OFF(LED_LOCK_PORT,LED_LOCK_PIN);
      HV_Level = 1;    
      Key_count = 0;
    }
  }
}

/*
********************************************************************************
*                           FUNCTION PROCESSE FUNCTIONS
*
* Description: This function Process function.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Function_Processe(void)
{
  
  if(Power_Flag == ON)
  {
    //UART_DE_T;
    Flash_Times_LED();  
    /*
    if(Power_LED_Start_Flag)
    {
     LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);      // power LED ON
     Power_LED_Start_Flag =0 ;
    }
   
    if(Power_LED_Start_Flag)
     {  
      LED_ON(LED_AUTO_SW_PORT,LED_AUTO_SW_PIN); //AUTO LED ON
      Power_LED_Start_Flag = 0 ;
     }*/
    if((!ERROR_Flag) && (!Lowlamp_Flag))
    {
      ERROR_CHK_Fun();    /* error check, set error flag*/     
      if(Lock_Flag == OFF)
      {        
           //Skin_Color_Type_Count++;
           //if(Skin_Color_Type_Count==2)
           //{
             //Skin_Color_Type_Count = 0;
             switch(Skin_Color_Type){           
                  case 3 :Skin_Color_Type_Count_5 = 0;
                          Skin_Color_Type_Count_4 = 0;
                          if(Mode_Trans)
                          {  
                            Mode_Flag=Mode_Trans;
                            Mode_Trans=0;         
                          } 
                           Skin_Color_Type_Count_3++;
                           if(Skin_Color_Type_Count_3==250)
                          {
                            Skin_Color_Type_Count_3 = 0;
                           if(HV_Level > 5)
                             {
                               HV_Level = 5;                        
                             
                             }
                          };break;
                  case 4 :Skin_Color_Type_Count_5 = 0;
                          Skin_Color_Type_Count_3 = 0;
                         if(Mode_Trans)
                          {  
                            Mode_Flag=Mode_Trans;
                            Mode_Trans=0;         
                          }                    
                           Skin_Color_Type_Count_4++;
                           if(Skin_Color_Type_Count_4==250)
                          {
                           Skin_Color_Type_Count_4 = 0; 
                           if(HV_Level > 3)
                             {
                               HV_Level = 3;                                                      
                             }
                          };break;
                   
                  case 5 :  Skin_Color_Type_Count_5++;
                            Skin_Color_Type_Count_3 = 0;
                            Skin_Color_Type_Count_4 = 0;
                           if(Skin_Color_Type_Count_5 < 2)
                           {
                             Mode_Trans =  Mode_Flag;
                              
                           }  
                           if(Skin_Color_Type_Count_5 == 5)
                           {
                             Mode_Flag = OFF_FLASH_MODE;
                              
                           }         
                           // Mode_Flag = OFF_FLASH_MODE;
                           //Flash_Ready = 0;
                           //ERROR_Flag=0;
                           ; break;                        
                  default :Skin_Color_Type_Count_5 = 0;
                           Skin_Color_Type_Count_3 = 0;
                           Skin_Color_Type_Count_4 = 0;
                          if(Mode_Trans)
                          {  
                            Mode_Flag = Mode_Trans;
                            Mode_Trans=0;         
                          }  break;
                  } 
          // }
       
        Charge_Control_Fun(HV_Level);  /* charge control */     
        if(Full_Charge_Flag)
        {  
          //if((Flash_Ready) && (Skin_Color_Type != 5))
         if(Flash_Ready)
          {           
              Flash_Ready = 0;
              Flash_Flag = 0;
              switch(Mode_Flag){
              case AUTO_MODE : Auto_Mode_Flash();break;
              case STANDARD_MODE : Standard_Mode_Flash();break;
              case AUTO_MULTI_MODE : Multi_Auto_Mode_Flash();break;
              case STANDARD_MULTI_MODE : Multi_Standard_Mode_Flash();break;
              //case OFF_FLASH_MODE :  break;
              default : break;
              }
            }
         if(Flash_Flag)
          {
            Flash_Ready = 1;            
          }
        }
      /* 
        if(Mode_Trans)
        {  
          Mode_Flag=Mode_Trans;
          Mode_Trans=0;         
        } 
        */
        LED_Multi_Disply(); /* multi mode LED indicate */
        if(!Flash_Times_LED_Flag)
        {
          LED_Level(HV_Level);
        }
      }      
      LED_OFF(LED_ERROR_PORT,LED_ERROR_PIN);
    }else{
      TIM1->SR1 = 0x00;
      TIM1->CR1 = 0x80; // disable counter
      SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
      
      if((Lowlamp_Flag) &&(!ERROR_Flag))
      {
        ERROR_CHK_Fun();
        //LED_ON(LED_LOWLAMP_PORT,LED_LOWLAMP_PIN);
        Lock_Flag = ON;
        Error_Power_Off_Proc();
      }
      if(ERROR_Flag)
      {
        
        Error_Power_Off_Proc();
        Delay_us(500);
        LED_ON(LED_ERROR_PORT,LED_ERROR_PIN);
      }
     // LED_ON(LED_LOCK_PORT,LED_LOCK_PIN);
    }        
  }
  UART_DE_T;
  
}

/*
********************************************************************************
*                           AUTO MODEL　FLASH FUNCTIONS
*
* Description: This function Process auto model flash function.
*
* Arguments  : none.
*
* Note(s)    : flash time one.
********************************************************************************
*/
void Auto_Mode_Flash(void)
{
    First_Flash = 0;
    Full_Charge_Flag = 0;
    
    Flash_Control(1);  //flash     
}

/*
********************************************************************************
*                           STANDARD MODEL　FLASH FUNCTIONS
*
* Description: This function Process standard model flash function.
*
* Arguments  : none.
*
* Note(s)    : flash time one.
********************************************************************************
*/
void Standard_Mode_Flash(void)
{
  First_Flash = 0;    
  Full_Charge_Flag = 0;
  Flash_Control(1); 
}

/*
********************************************************************************
*                         AUTO MULTI MODEL　FLASH FUNCTIONS
*
* Description: This function Process auto multi model flash function.
*
* Arguments  : none.
*
* Note(s)    : flash time three.
********************************************************************************
*/
void Multi_Auto_Mode_Flash(void)
{  
    First_Flash = 0;            
    Full_Charge_Flag = 0;
    Flash_Control(4);   
}

/*
********************************************************************************
*                       STANDARD MULTI MODEL　FLASH FUNCTIONS
*
* Description: This function Process standard multi model flash function.
*
* Arguments  : none.
*
* Note(s)    : flash time three.
********************************************************************************
*/
void Multi_Standard_Mode_Flash(void)
{
    First_Flash = 0;           
    Full_Charge_Flag = 0;
    Flash_Control(4);   
}


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/
