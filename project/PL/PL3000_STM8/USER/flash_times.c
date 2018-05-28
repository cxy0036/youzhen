#include "protocol.h"
#include "flash_times.h"
#include "LED.h"
#include "UART.h"
#include "delay.h"
#include "stdio.h"
#include "error_chk.h"

uint32_t Flash_Times = 0;
uint32_t Skin_Color_Data = 0;
uint8_t Flash_Times_Level = 0;
//uint8_t Flash_Times_Level_Count = 0;
extern uint8_t Receive_Buffer_Full_Flag;
extern uint8_t Receive_Buff[15];
extern uint8_t Receive_Date_Length;
extern uint8_t ERROR_Flag;
/*
void Get_Flash_Times(void)
{
  uint16_t Time_Out = TIME_OUT;
  uint8_t Data_Check_OK;
  Send_Request(FUN_No1,0,0);
  Delay_us(300);
  UART_DE_R;
  while(! Receive_Buffer_Full_Flag)
  {
   Time_Out--;
   if(Time_Out==0)
   {
     UART_DE_T;
     return ;
   }
  }
  Receive_Buffer_Full_Flag  = 0;
  Data_Check_OK = Crc_Caculate(Receive_Buff,Receive_Date_Length);
  if(!Data_Check_OK)
  {
   Flash_Times = (uint32_t)Receive_Buff[1];
   Flash_Times = Flash_Times | ((uint32_t)Receive_Buff[2]<<8);
   Flash_Times = Flash_Times | ((uint32_t)Receive_Buff[3]<<16);
   Flash_Times = Flash_Times | ((uint32_t)Receive_Buff[4]<<24);
  }
  UART_DE_T;
}*/

void Get_Flash_Times_Level(void)
{
  Send_Request(FUN_No2,0,0);
  Flash_Times_Level = Receive_Wait_Lost_First(TIME_OUT);
  //Flash_Times_Level = 0;
  if(Flash_Times_Level == 0)  
  {  
      ERROR_Flag = 1;
   }
}
/*
void Get_Skin_Color(void)
{
  uint16_t Time_Out = TIME_OUT;
  uint8_t Data_Check_OK;
  Send_Request(FUN_No3,0,0);
  Delay_us(300);
  UART_DE_R;
  while(! Receive_Buffer_Full_Flag)
  {
   Time_Out--;
   if(Time_Out==0)
   {
     UART_DE_T;
     return ;
   }
  }
  Receive_Buffer_Full_Flag  = 0;
  Data_Check_OK = Crc_Caculate(Receive_Buff,Receive_Date_Length);
  if(!Data_Check_OK)
  {
   Skin_Color_Data = (uint32_t)Receive_Buff[1];
   Skin_Color_Data = Skin_Color_Data | ((uint32_t)Receive_Buff[2]<<8);
   Skin_Color_Data = Skin_Color_Data | ((uint32_t)Receive_Buff[3]<<16);
   Skin_Color_Data = Skin_Color_Data | ((uint32_t)Receive_Buff[4]<<24);
  }
  UART_DE_T;
}*/