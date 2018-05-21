/**
  ******************************************************************************
  * @file    HV_Charge.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define HV and Flash , PID Control Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "HV_Charge.h"
#include "ADC.h"
#include "GPIO_Config.h"
#include "KEY_FTC332.h"
#include "delay.h"
#include "stdio.h"
#include "stm8s.h"
#include "LED.h"
#include "UART.h"
#include "stm8s_it.h"
#include "protocol.h"

/* Global variable -----------------------------------------------------------*/
uint8_t Full_Charge_Flag = 0;  /* full charge flag */
uint16_t read_hv;
uint16_t charge_time_Count= 0 ;
uint16_t flash_over_thr;
uint16_t Flash_Time_Out = 0; /* Flash timeout var */

extern uint8_t Mode_Change;
extern uint8_t Mute_Flag;
extern uint8_t First_Flash;
extern uint8_t HV_Level;
extern uint8_t Flash_Times_LED_Flag;
extern uint8_t Mode_Flag;
extern uint8_t Skin_Color_Type;
extern uint8_t Receive_Buffer_Full_Flag;
extern uint8_t Receive_Buff[15];
extern uint8_t Receive_Date_Length;
extern uint8_t Level_Change;
extern uint8_t Flash_Times_Level;
//uint8_t Flash_Trig_Flag=0;

uint8_t Charge_Overflow_count = 0;
uint8_t Charge_TimeOut_Flag = 0; /* charge timeout flag */

uint16_t Charge_Time_Count = 0;  /* charge timeout count */
uint8_t Charge_Overflow_error = 0; /* charge overflow error flag */
uint8_t Flash_ERROR = 0;  /*Flash error flag*/
//uint8_t last_hv_level = 0;  /* charge timeout flag */
uint8_t Discharge_Flag = 0;  /* last hv level and discharge flag */
uint8_t Judgment_Times = 0;  /* hv charge judgment times count,use to set full charge flag */
uint8_t Full_Charge_Beep_Flag = 0; /* full charge beep on flag */
//uint8_t Befor_Level = 0; 
PID HV_PID; /* global PID variable */
//uint8_t plus_times=0;
uint16_t Charge_Wait_Time = 123 ;
/* Public functions ----------------------------------------------------------*/

/*
********************************************************************************
*                           CHARGE CONTROL FUNCTIONS
*
* Description: This function control HV charge.
*
* Arguments  : hv level value.
*
* Note(s)    : If Charge overflow or charge time out , set error flag.
********************************************************************************
*/
void Charge_Control_Fun(uint8_t hv_level)
{
  if (Flash_Times_Level == 1)
  {
    HV_PID.SetValue = FULL_CHARGE_LEVEL1;
  }else{
    switch(hv_level){
    case 1:HV_PID.SetValue = FULL_CHARGE_LEVEL1;break;
    case 2:HV_PID.SetValue = FULL_CHARGE_LEVEL2;break;
    case 3:HV_PID.SetValue = FULL_CHARGE_LEVEL3;break;
    case 4:HV_PID.SetValue = FULL_CHARGE_LEVEL4;break;
    case 5:HV_PID.SetValue = FULL_CHARGE_LEVEL5;break;
    case 6:HV_PID.SetValue = FULL_CHARGE_LEVEL6;break;
    case 7:HV_PID.SetValue = FULL_CHARGE_LEVEL7;break;
    case 8:HV_PID.SetValue = FULL_CHARGE_LEVEL8;break;
    default : HV_PID.SetValue = 0;break;
    }
  }
  read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
  //if(read_hv <= PID_START_THR)
  if(read_hv <= (HV_PID.SetValue-3))
  {
     SET_IO_LOW(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* ON HV */
    
  }else{
    PID_Control(&HV_PID,(int16_t)read_hv);
  }
  Full_Charge_Judgment(read_hv);
}

/*
********************************************************************************
*                           FLASH CONTROL FUNCTIONS
*
* Description: This function control Flash.
*
* Arguments  : Flash times,if 1,use standard model,if >1 sue multi model.
*
* Note(s)    : If discharge or charge time out , set error flag.
*              you can judgment the time of flash.
********************************************************************************
*/
void Flash_Control(uint8_t plus_times)
{  
  uint8_t Flash_Start = 0;  //Tig_Flag = 0,,Data_Check_OK
  //uint8_t Flash_Ready_OK = 1;
  uint8_t Flash_OK = 1,multi_flag;
  uint16_t read_hv;  /* read hv value in AD */

  //uint16_t Flash_Interval_Time;
  volatile uint8_t Touch_Key_Not_Release = 1;
  //uint16_t flash_delay = 400;
  //UART_DE_R;
  Charge_Time_Count = 0;  /* clean charge time count */
  Discharge_Flag = 0;
  TIM1->SR1 = 0x00;
  TIM1->CR1 = 0x80; // disable counter
  
  if(plus_times > 1)
  {
    multi_flag = 1;
      flash_over_thr = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);         
  }else{
    multi_flag = 0;
    flash_over_thr = FLASH_STANDAR_OVER_THR;
  }
  SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
  while(!Flash_Start)  /* wait flash start */
  {
    Flash_Time_Out++;
    IWDG->KR = 0xAA;
    Flash_Start = Receive_Wait_Lost_First(3000);
    if(Flash_Time_Out == 400)
    {
      SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash OFF;
      return;
    }
  }   
  while(plus_times)
  {
    if(Touch_Key_Not_Release)
    {
      SET_IO_LOW(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash ON;
      Delay_us(2200);
      SET_IO_HIGH(TRIG_PORT,TRIG_PIN); //flash ON;
    }else{
      return ;
    }
    if(multi_flag)//多闪
    {
      //Delay_us(146);
      Delay_us(86);
      if(HV_Level != 1)
      {
        Delay_us(40);
        Delay_us((HV_Level-1)*10);
        //Delay_us((HV_Level-1)*2);
      }      
      SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash OFF;
      SET_IO_LOW(TRIG_PORT,TRIG_PIN); //flash OFF;
    }else{//单闪
      if(HV_Level == 1)
      {
         Delay_us(300);
      }else{ 
         Delay_ms(10);
      }
      SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash OFF;
      SET_IO_LOW(TRIG_PORT,TRIG_PIN); //flash OFF;
    }    
    read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL); 
    Flash_Time_Out = 0;
    while(read_hv > flash_over_thr)
    {
      Flash_Time_Out++;
      read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
      IWDG->KR = 0xAA;
      if(Flash_Time_Out == FLASH_TIME_OUT)  // if not down, set error and return 
      {
        Flash_ERROR = 1;//error;
        Flash_Time_Out = 0;
        SET_IO_HIGH(MULTI_PLUS_CTR_PORT,MULTI_PLUS_CTR_PIN); //flash OFF;
        SET_IO_LOW(TRIG_PORT,TRIG_PIN); //flash OFF;
        return;
      }
    }
    Flash_ERROR = 0;
    Delay_us(100);
    Send_Request(FUN_No5,&Flash_OK,1);   // send flash ok flag    
    Delay_us(300);
    UART_DE_R;
    IWDG->KR = 0xAA;
    plus_times--;
    if(plus_times>0)
    {     
      IWDG->KR = 0xAA;
      
      switch(HV_Level){
      case 1:Multi_Charge_Control(900,00);break;  //100ms
      case 2:Multi_Charge_Control(1300,20);break;  //120ms
      case 3:Multi_Charge_Control(1750,20);;break;  //150ms
      case 4:Multi_Charge_Control(2200,30);;break;  //200ms
      case 5:Multi_Charge_Control(2800,40);;break;  //260ms
      case 6:Multi_Charge_Control(3300,50);;break;  //330ms
      case 7:Multi_Charge_Control(3800,60);;break;  //410ms
      case 8:Multi_Charge_Control(4300,70);;break;  //500ms
      default:break;
      }     
      SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  // OFF HV, charge over,to start next flash 
      IWDG->KR = 0xAA;
     }
    Touch_Key_Not_Release = Receive_Wait_Lost_First(3000);
    }
}

void Multi_Charge_Control(uint16_t charge_time, uint16_t delay_time)
{
  uint16_t Charge_LEVEL = 0;
  //uint16_t delay_time_Count = 0;
  charge_time_Count = charge_time;
  if (Flash_Times_Level == 1)
  {
    Charge_LEVEL = FULL_CHARGE_LEVEL1;
  }else{
  switch(HV_Level){
      case 1:Charge_LEVEL = FULL_CHARGE_LEVEL1;break;
      case 2:Charge_LEVEL = FULL_CHARGE_LEVEL2;break;
      case 3:Charge_LEVEL = FULL_CHARGE_LEVEL3;break;
      case 4:Charge_LEVEL = FULL_CHARGE_LEVEL4;break;
      case 5:Charge_LEVEL = FULL_CHARGE_LEVEL5;break;
      case 6:Charge_LEVEL = FULL_CHARGE_LEVEL6;break;
      case 7:Charge_LEVEL = FULL_CHARGE_LEVEL7;break;
      case 8:Charge_LEVEL = FULL_CHARGE_LEVEL8;break;
      default :break;
      }
  }
  SET_IO_LOW(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* ON HV , charge */
  Delay_ms(50);
  IWDG->KR = 0xAA; 
  while(charge_time_Count)
  {
    charge_time_Count--;
    read_hv = ADC_Read_Channel_Fun(ADC_HV_IN_CHANNEL);
    if(read_hv > Charge_LEVEL)
    {
      break;
    }
  } 
  SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV , charge */
  while(charge_time_Count)
  {
    charge_time_Count--;
    Delay_us(80);
  } 
}
/*
********************************************************************************
*                           PID CACULATE FUNCTIONS
*
* Description: This function PID calculate.
*
* Arguments  : PID struct and new value.
* 
* Return : PID caculate value.
*
* Note(s)    : none.
********************************************************************************
*/
int16_t PID_Caculate(PID *pid,int16_t NewValue)
{
  int16_t error,derror;
  error = pid->SetValue - NewValue;
  //pid->SumError += error;
  derror = error - pid->LastError;
  pid->LastError = error;
  //pid->PrevError = pid->LastError;
  return (pid->Kp*error + pid->Kd*derror); //pid->Ki*pid->SumError
}

/*
********************************************************************************
*                           PID CONTROL FUNCTIONS
*
* Description: This function use PID control the charge time.
*
* Arguments  : PID struct and new value.
*
* Note(s)    : none.
********************************************************************************
*/
void PID_Control(PID *pid,int16_t newvalue)
{
  int16_t adjust_value; 
  adjust_value = PID_Caculate(pid,newvalue);
  if(adjust_value <= 0)
  {
    SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
  }else{
    SET_IO_LOW(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* ON HV */
    Timer_PID_Control(adjust_value);  /* judgment chagre time */
  } 
}

/*
********************************************************************************
*                           PID CONTROL TIMER FUNCTIONS
*
* Description: This function control charge time is called by PID_Control function.
*
* Arguments  : count data.
*
* Note(s)    : none.
********************************************************************************
*/
void Timer_PID_Control(uint16_t count_data)
{
//TIM1->EGR |=0x01;           /* Re-initializes counter and prescaler */ 
  TIM1->ARRH = (uint8_t)(count_data >> 8);
  TIM1->ARRL = (uint8_t)count_data;
  TIM1->CR1 |= 0x01; //start counter
}

/*
********************************************************************************
*                           TIMER1 INIT FUNCTIONS
*
* Description: This function init timer1.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void Timer1_Init(void)
{
  disableInterrupts();
  TIM1->EGR |=0x01;           /* Re-initializes counter and prescaler */ 
  TIM1->PSCRH = 0x06; // 16M系统时钟经预分频f=fck/(PSCR+1)
  TIM1->PSCRL = 0x3F; // PSCR=0x1F3F，f=16M/(0x063F+1)=10000Hz，每个计数周期0.1ms
  TIM1->CNTRH = 0;
  TIM1->CNTRL = 0;
 //TIM1->ARRH = 0xFF; // 自动重载寄存器ARR=0xFFFF=65535
 //TIM1->ARRL = 0xFF; // 每记数500次产生一次中断，即6.5535s
  TIM1->IER = 0x01; // 允许更新中断
  TIM1->SR1 = 0x00;
  TIM1->CR1 = 0x80; // disable counter
  enableInterrupts();
}

/*
********************************************************************************
*                           CHARGE CONTROL FUNCTIONS
*
* Description: This function control HV charge.
*
* Arguments  : hv level value.
*
* Note(s)    : If Charge overflow or charge time out , set error flag.
********************************************************************************
*/
void HV_PID_Init(void)
{
  HV_PID.Kp = 14;
  HV_PID.Ki = 0;
  HV_PID.Kd = 4;
  HV_PID.LastError = 0;
  HV_PID.SumError = 0;
  Timer1_Init();
}

/*
********************************************************************************
*                           CHARGE CONTROL FUNCTIONS
*
* Description: This function judgment full charge .
*
* Arguments  : hv level value.
*
* Note(s)    : If Charge overflow or charge time out , set error flag.
********************************************************************************
*/
void Full_Charge_Judgment(uint16_t read_hv)
{
  if(!Full_Charge_Flag)//不满电
  {
    Full_Charge_Beep_Flag = 0;
    if(read_hv >= HV_PID.SetValue+3)
    {
      Discharge_Flag = 1;
    }else{
      Discharge_Flag = 0;
    }
    Charge_Time_Count++;
  }
   if((Level_Change == 1) || (Mode_Change == 1))//模式或档位改变的话，计数清零
   {
    Charge_Time_Count = 0;
   }
  if((Mode_Flag == AUTO_MULTI_MODE) || (Mode_Flag == AUTO_MODE))//if(Mode_Flag == AUTO_MODE)//
  {
    Charge_Wait_Time = 88;  //3S   140
  }
  else{
        if(HV_Level <= 2)
        {
          Charge_Wait_Time=77; //2s   90
        }else{
               if(HV_Level <= 5)
               {
                 Charge_Wait_Time=120; //3s  135
               }else{
                     if(HV_Level <= 7)
                     {
                       Charge_Wait_Time=213; //5s  235
                     }else{
                           Charge_Wait_Time=259; //6s  285
                          }
                    }
             }
    }
  
  //if((Charge_Time_Count >= Charge_Wait_Time) || (Full_Charge_Flag))
  if(Charge_Time_Count >= Charge_Wait_Time)
  {
    if((read_hv <= HV_PID.SetValue+11) && (read_hv >= HV_PID.SetValue-11))
    {     
      if(!Full_Charge_Flag)  /* befor not full charge */
      {
        Judgment_Times++;   
        //Charge_Time_Count = 0;
        Charge_Overflow_error = 0;
        if(Judgment_Times == 5)
        {
          if(Mute_Flag == OFF)
          {
            BEEP->CSR =0X09;
            BEEP->CSR|=0x20;
            //BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
          }
          Full_Charge_Beep_Flag = 1;
          
        }
        if(Judgment_Times == 11)  /* delay some time set full charge flag */
        {
          Full_Charge_Flag = 1;
          Full_Charge_Beep_Flag = 0;
          Discharge_Flag = 0;
          //LED_ON(FULL_CHARGE_PORT,FULL_CHARGE_PIN);        
          BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);
          Judgment_Times = 0;
          Charge_Time_Count = 0;
        }
      }  
    }else{
      if(Discharge_Flag)
      {
        if(!Full_Charge_Flag)
        {
          if(Charge_Time_Count == Charge_Wait_Time)
          {
            if(Mute_Flag == OFF)
            {
              BEEP->CSR =0X09;
              BEEP->CSR|=0x20;
              //BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);             
            }
            Full_Charge_Beep_Flag = 1; 
          }
          if(Charge_Time_Count==(Charge_Wait_Time+4))
          {
            Full_Charge_Flag = 1;
            Charge_Time_Count = 0;
            Full_Charge_Beep_Flag = 0;
            //Discharge_Flag = 0;
            //LED_ON(FULL_CHARGE_PORT,FULL_CHARGE_PIN);  
            BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);
          }
          if(read_hv <= HV_PID.SetValue - 20)
          {
            Discharge_Flag = 0;
            //SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
           // return;
          }
        }
      }else{
        Judgment_Times = 0;
        //Full_Charge_Flag = 0;
        Full_Charge_Beep_Flag = 0;
        //Charge_Time_Count = 0;
        //LED_OFF(FULL_CHARGE_PORT,FULL_CHARGE_PIN);
        if(Charge_Time_Count>=300)
        {
          //Charge_TimeOut_Flag = 1;
          Charge_Time_Count = 0;
          SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
          return;
        }
        
        if((read_hv >= HV_PID.SetValue + 50)||(read_hv >=950))
       
        {
            Charge_Overflow_count ++;
            if( Charge_Overflow_count == 12)
          {
            Charge_Overflow_count = 0;
            //Charge_Overflow_error = 1;
            SET_IO_HIGH(HV_CONTROL_PORT,HV_CONTROL_PIN);  /* OFF HV */
            return;
          } 
        }
      }
    }
  }
}

/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/