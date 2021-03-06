#include "stm8s_uart2.h"
#include "stdio.h"
#include "KEY_FTC332.h"
#include "EEPROM.h"
#include "stm8s_it.h"
#include "protocol.h"
#include "UART.h"
#include "delay.h"
#include "Bluetooth.h"

//uint8_t Blue_Error_count=0;
uint8_t temp[3],i;
//uint8_t Blue_SLEEP_flag=0;
uint8_t wait_times_count=0;
extern uint8_t Bluetooth_Error_Flag ;
extern uint32_t Flash_Count;
extern uint8_t Lamp_Type;
extern uint8_t Bluetooth_Request_Flag;
extern uint8_t Bluetooth_Request_Skin_Flag;
extern uint8_t Bluetooth_Request_lamptype_Flag;
extern uint8_t Receive_Buffer_Full_Flag;
extern uint8_t Receive_Buff[15];
extern uint8_t Receive_Date_Length;

//暂时取消蓝牙模块功能
void Bluetooth_Init(void)
{
  uint8_t check_flag = 0;
  UART_DE_R;
  Delay_us(500);
  printf("AT");
  Delay_ms(5);
  UART_DE_T; 
  Delay_ms(3);  
  check_flag = Bluetooth_Command_Check();
  if(!check_flag)//换波特率38400
  {
    UART2_Init(USART_DEBUG_BAUTRATE, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, 
             UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
    UART_DE_R;
    Delay_us(500);
    printf("AT");
    Delay_ms(5);
    UART_DE_T; 
   // Delay_ms(13);  
    check_flag = Bluetooth_Command_Check();
   
  }
  if(check_flag)
  { 
    Delay_ms(4);
    UART_DE_R;
    Delay_us(300);
    printf("AT+NAMEPL3000");
    Delay_ms(8);
    UART_DE_T;
    //Delay_ms(13);
    check_flag = Bluetooth_Command_Check();
    if(check_flag)
    {
      Delay_ms(4);
      UART_DE_R;
      Delay_us(300);
      printf("AT+BAUD2");
      Delay_ms(7);
      UART_DE_T;
      //Delay_ms(13);
      check_flag = Bluetooth_Command_Check();
    }
    Delay_ms(20);
    UART_DE_R;
    Delay_us(300);
    printf("AT+RESET");
    Delay_ms(8);
    UART_DE_T;
  } 
  UART2_Init(USART_DEBUG_BAUTRATE, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, 
             UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
 
}

uint8_t Bluetooth_Command_Check(void)
{
  uint16_t time_out;
  uint8_t tt;
  for(i=0;i<3;i++)
  {
    time_out = 0;
    tt = 0;
    while(UART2_GetFlagStatus(UART2_FLAG_RXNE)==RESET)
    {
      time_out ++;
      if(time_out == 500)
      {                    
        tt++;
        time_out = 0;
        if(tt == 80)
        {
          break;
        }
      }
    } 
    UART2_ClearFlag(UART2_FLAG_RXNE);
    temp[i] = UART2_ReceiveData8();
  }
 //if(temp[1] == 'O' && temp[2] == 'K')
   if(temp[1] == 'O' || temp[2] == 'K'||temp[0] == 'O' || temp[1] == 'K') 
  {
    return 1;
  }else{
    return 0;
  }  
 
}

void Bluetooth_Processe(void)
{/*
  uint16_t wait_times = 65000;
  uint8_t temp[4];
  uint8_t Data_Check_OK ;
  if(Bluetooth_Request_Flag)
  {
    Bluetooth_Request_Flag = 0;
    UART_DE_T;
    Delay_us(300);
    Send_Request(FUN_No1,0,0);
    Delay_us(500);
    UART_DE_R;
    Delay_us(2500);
    while(!Receive_Buffer_Full_Flag)
    {
      wait_times --;
      if(wait_times==0)
      {
        printf("Get Times Error,Please Try Again\n");
        return ;
      }
    }
    Receive_Buffer_Full_Flag = 0;
    Data_Check_OK = Crc_Caculate(Receive_Buff,Receive_Date_Length);
    if(!Data_Check_OK)
    {
      if(Receive_Buff[0] == 0x04)//
      {
       temp[0] = Receive_Buff[1];
       temp[1] = Receive_Buff[2];
       temp[2] = Receive_Buff[3];
       temp[3] = Receive_Buff[4];
       Flash_Count = ((uint32_t)temp[3] << 24) | ((uint32_t)temp[2] << 16) | \
              ((uint32_t)temp[1] << 8) | (uint32_t)temp[0];
       printf("Flash Times : %ld",Flash_Count);
      }      
    }else{
            printf("Get Times Error,Please Try Again\r\n"); 
    }
    Delay_us(1000);
    UART_DE_T;
  }

 if(Bluetooth_Request_Skin_Flag)
  {
    Bluetooth_Request_Skin_Flag = 0;
    UART_DE_T;
    Delay_us(300);
    Send_Request(FUN_No3,0,0);
    Delay_us(500);
    UART_DE_R;
    Delay_us(2500);
    while(!Receive_Buffer_Full_Flag)
    {
      wait_times --;
      if(wait_times==0)
      {
        printf("Get Skin Error,Please Try Again\n");
        return ;
      }
    }
    Receive_Buffer_Full_Flag = 0;
    Data_Check_OK = Crc_Caculate(Receive_Buff,Receive_Date_Length);
    if(!Data_Check_OK)
    {
      if(Receive_Buff[0] == 0x04)//
      {
         temp[0] = Receive_Buff[1];
         temp[1] = Receive_Buff[2];
         temp[2] = Receive_Buff[3];
         temp[3] = Receive_Buff[4];
         Flash_Count = ((uint32_t)temp[3] << 24) | ((uint32_t)temp[2] << 16) | \
                ((uint32_t)temp[1] << 8) | (uint32_t)temp[0];
        if(Flash_Count > 1000)
        {
          printf("Skin_Color_Data : %ld",(Flash_Count-1000)/20);
          //printf("Skin_Color_Data : %ld",Flash_Count);
        }else{  
            printf("Skin_Color_Data : %d",0);
        }
      }      
    }//else{
           // printf("Get Skin Error,Please Try Again\r\n"); 
    //}
    Delay_us(1000);
    UART_DE_T;
  }
  if(Bluetooth_Request_lamptype_Flag)
  {
    Bluetooth_Request_lamptype_Flag = 0;
    UART_DE_R;
    Delay_us(500);
    printf("Lamp_Type : %d",Lamp_Type);
    Delay_us(1000);
    UART_DE_T;
  }*/
}