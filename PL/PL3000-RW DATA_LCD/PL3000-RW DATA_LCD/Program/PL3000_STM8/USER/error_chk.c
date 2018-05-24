/**
  ******************************************************************************
  * @file    eeror_chk.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   Implement error check Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "error_chk.h"
#include "ADC.h"
#include "KEY_FTC332.h"
#include "protocol.h"
#include "UART.h"
#include "delay.h"

/* Global variable -----------------------------------------------------------*/
uint8_t ERROR_Flag_MainFan = 0;  /* Main Fan error flag */
uint8_t LAMP_Type = 0;  /* Lamp Type */

extern uint8_t Flash_Times_Level;
extern uint8_t Probe_Error_Flag ;
extern uint8_t ERROR_Flag;  /* All error flag */
extern uint8_t Recevie_Data_Error; 
extern uint8_t Charge_TimeOut_Flag; /* charge timeout flag */
//extern uint8_t EEPROM_ERROR_Flag; /* eeprom read or write error flag */
extern uint8_t Charge_Overflow_error; /* charge overflow error */
extern uint8_t Flash_ERROR;
extern uint8_t Skin_Color_Type;

//extern uint8_t Receive_Buffer_Full_Flag;
//extern uint8_t Receive_Buff[8];
//extern uint8_t Receive_Date_Length;
//extern uint8_t Model_Flag;

/* Public functions ----------------------------------------------------------*/

/*
********************************************************************************
*                        MAIN FAN ERROR CHECK FUNCTIONS
*
* Description: This function check the main fan error.
*
* Arguments  : none
*
* Note(s)    : If main fan error, set the main fan error flag.
********************************************************************************
*/
void MainFan_Error_Chk_Fun(void)
{
  uint16_t MainFan_ReadData;
  MainFan_ReadData = ADC_Read_Channel_Fun(ADC_MAIN_FAN_CHANNEL);
  if((MainFan_ReadData >= MAIN_FAN_SHORT_ERROR_THR) || (MainFan_ReadData <= MAIN_FAN_OPEN_ERROR_THR))
  {
    ERROR_Flag_MainFan = 1;             /* set main fan error flag */
  }else{
    ERROR_Flag_MainFan = 0;
  }
}

/*
********************************************************************************
*        GLASS TYPE AND GLASS ERROR, PROBE FAN ERROR CHECK FUNCTIONS
*
* Description: This function check the probe fan error,glass type and error.
*
* Arguments  : none
*
* Note(s)    : If have error, set the galss error flag.
********************************************************************************
*/
/*
void Probe_Error_Chk_Fun(void)
{
   uint16_t Time_Out = TIME_OUT;
   uint8_t Data_Check_OK;
   Send_Request(FUN_No6,0,0);
   Delay_us(100);
   UART_DE_R;
   while(!Receive_Buffer_Full_Flag)
   {
     Time_Out--;
     if(Time_Out==0)
     {
       UART_DE_T;
       return ;
     }
   }
   Receive_Buffer_Full_Flag = 0;
   Data_Check_OK = Receive_Data_Check(Receive_Buff,Receive_Date_Length);
   if(Data_Check_OK)
   {
     Probe_Error_Flag = Receive_Buff[1];
   }
   UART_DE_T;
}
*/
/*
********************************************************************************
*                             ERROR CHECK FUNCTIONS
*
* Description: This function check All errors.
*
* Arguments  : none
*
* Note(s)    : If have any error, set the all error flag.
********************************************************************************
*/
void ERROR_CHK_Fun(void)
{
  MainFan_Error_Chk_Fun();
  //Probe_Error_Chk_Fun();
  //if(Charge_TimeOut_Flag|| Charge_Overflow_error|| Probe_Error_Flag || Flash_ERROR) /* ||  ERROR_Flag_MainFan || Probe_Error_Flag || Flash_ERROR || Charge_TimeOut_Flag || Charge_Over_error */ 
  if(Charge_TimeOut_Flag || Charge_Overflow_error|| ERROR_Flag_MainFan || Probe_Error_Flag || Flash_ERROR  )
  {
   ERROR_Flag = 1;  
  }
else{
    ERROR_Flag = 0;
  }
}

void Lamp_Type_Check(void)
{
  Send_Request(FUN_No11,0,0);
  LAMP_Type = Receive_Wait_Lost_First(TIME_OUT);
  if(LAMP_Type == 0)
  {
   //ERROR_Flag = 1;
  }
}
/*
void Skin_Color_Check(void)
{
  Send_Request(FUN_No3,0,0);
  Skin_Color_Type = Receive_Wait(TIME_OUT);
}

void Get_Mode_Type(void)
{
  uint8_t r;
  Send_Request(FUN_No12,0,0);
  r = Receive_Wait(TIME_OUT);
  if(r==0)
  {
    Model_Flag = Model_Flag;
  }else{
     Model_Flag = r;
  }
}
*/
/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/