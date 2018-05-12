#include "protocol.h"
#include "UART.h"
#include "stm8s_it.h"
#include "delay.h"
#include "KEY_FTC332.h"
#include "HV_Charge.h"
#include "flash_times.h"
#include "stm8s_beep.h"

extern uint8_t Receive_Buff[15];
extern uint8_t Receive_Date_Length;
extern uint8_t Receive_Buffer_Full_Flag;
//extern uint8_t Mode_Trans;
extern uint8_t Flash_Flag;
extern uint8_t Full_Charge_Flag;

extern uint8_t Mode_Flag;
extern uint8_t Lock_Flag;
extern uint8_t Flash_Times_Level;
//extern uint8_t Mode_Change;
extern uint8_t Power_Flag;
extern uint8_t ERROR_Flag;
extern uint8_t Lowlamp_Flag;
extern uint8_t HV_Level;
extern uint8_t Full_Charge_Beep_Flag;
extern uint8_t Mute_Flag;
extern uint8_t Charge_Time_Count;
extern uint8_t Key_Trg;
extern uint8_t time_out_power_off;
extern uint8_t HV_Level_Dowm_3;
extern uint8_t HV_Level_Dowm_5;

uint8_t Probe_Error_Flag = 0;
uint8_t Skin_Color_Type = 0;
uint8_t Flash_Ready = 0;
uint8_t Get_Mode_Flag = 1;
uint8_t Lamp_Type;
uint8_t Data_Check_OK = 0;

uint8_t Recevie_Data_Error = 0;
uint8_t Communication_Error_Count = 0;


static const uint8_t CRC_Table[256]={
    0x00,0x31,0x62,0x53,0xc4,0xf5,0xa6,0x97,0xb9,0x88,0xdb,0xea,0x7d,0x4c,0x1f,0x2e,
    0x43,0x72,0x21,0x10,0x87,0xb6,0xe5,0xd4,0xfa,0xcb,0x98,0xa9,0x3e,0x0f,0x5c,0x6d,
    0x86,0xb7,0xe4,0xd5,0x42,0x73,0x20,0x11,0x3f,0x0e,0x5d,0x6c,0xfb,0xca,0x99,0xa8,
    0xc5,0xf4,0xa7,0x96,0x01,0x30,0x63,0x52,0x7c,0x4d,0x1e,0x2f,0xb8,0x89,0xda,0xeb,
    0x3d,0x0c,0x5f,0x6e,0xf9,0xc8,0x9b,0xaa,0x84,0xb5,0xe6,0xd7,0x40,0x71,0x22,0x13,
    0x7e,0x4f,0x1c,0x2d,0xba,0x8b,0xd8,0xe9,0xc7,0xf6,0xa5,0x94,0x03,0x32,0x61,0x50,
    0xbb,0x8a,0xd9,0xe8,0x7f,0x4e,0x1d,0x2c,0x02,0x33,0x60,0x51,0xc6,0xf7,0xa4,0x95,
    0xf8,0xc9,0x9a,0xab,0x3c,0x0d,0x5e,0x6f,0x41,0x70,0x23,0x12,0x85,0xb4,0xe7,0xd6,
    0x7a,0x4b,0x18,0x29,0xbe,0x8f,0xdc,0xed,0xc3,0xf2,0xa1,0x90,0x07,0x36,0x65,0x54,
    0x39,0x08,0x5b,0x6a,0xfd,0xcc,0x9f,0xae,0x80,0xb1,0xe2,0xd3,0x44,0x75,0x26,0x17,
    0xfc,0xcd,0x9e,0xaf,0x38,0x09,0x5a,0x6b,0x45,0x74,0x27,0x16,0x81,0xb0,0xe3,0xd2,
    0xbf,0x8e,0xdd,0xec,0x7b,0x4a,0x19,0x28,0x06,0x37,0x64,0x55,0xc2,0xf3,0xa0,0x91,
    0x47,0x76,0x25,0x14,0x83,0xb2,0xe1,0xd0,0xfe,0xcf,0x9c,0xad,0x3a,0x0b,0x58,0x69,
    0x04,0x35,0x66,0x57,0xc0,0xf1,0xa2,0x93,0xbd,0x8c,0xdf,0xee,0x79,0x48,0x1b,0x2a,
    0xc1,0xf0,0xa3,0x92,0x05,0x34,0x67,0x56,0x78,0x49,0x1a,0x2b,0xbc,0x8d,0xde,0xef,
    0x82,0xb3,0xe0,0xd1,0x46,0x77,0x24,0x15,0x3b,0x0a,0x59,0x68,0xff,0xce,0x9d,0xac
};


void Send_Request(uint8_t fun_num, uint8_t *Data,uint8_t length)
{
  uint8_t i,crc,temp[8];
  temp[0] = fun_num;
  temp[1] = length;
  for(i=2;i<length+2;i++)
  {
    temp[i] = *Data++;
  }
  crc = Crc_Caculate(temp,length+2); 
  temp[length+2] = crc;
  USARTx_SendByte(BUS_HEADER);	
  for(i=0;i<length+3;i++)
  {
    USARTx_SendByte(temp[i]);
    Delay_us(100);
  }
  USARTx_SendByte(BUS_END);
}
      
uint8_t Crc_Caculate(uint8_t *d,uint8_t length)
{
  uint8_t crc = 0;
  while(length--)
  {
    crc = CRC_Table[crc ^ *d++];
  }
  return crc;
}

uint8_t  Receive_Wait_Lost_First(uint16_t wait_times)
{
  uint8_t Data_Check_OK,R_Data;
  Delay_us(300);
  UART_DE_R; 
  while(wait_times--)
  {
    if(Receive_Buffer_Full_Flag)
    {
      Receive_Buffer_Full_Flag = 0;
      Data_Check_OK = Crc_Caculate(Receive_Buff,Receive_Date_Length);
      if(!Data_Check_OK)
      {
        if(Receive_Buff[0] == 0x01)//
        {
         R_Data = Receive_Buff[1];
         Delay_us(900);
         UART_DE_T;
         return  R_Data;
        }
      }else{
         UART_DE_T;
         return  0;
      }
    }
  }
  Delay_us(300);
  UART_DE_T
  return  0;
}

void Communication_Fun(void)
{
  uint8_t  Data_Check_OK,da[5];
  if((Power_Flag == ON))
  //if(1)
  {
    if(!ERROR_Flag)// && (!Lowlamp_Flag))
    {
      UART_DE_T;      
      if(Receive_Buffer_Full_Flag)
      {
        Communication_Error_Count = 0;
        Receive_Buffer_Full_Flag = 0;
        Data_Check_OK = Crc_Caculate(Receive_Buff,Receive_Date_Length);
        if(!Data_Check_OK)
        {
          if(Receive_Buff[0]==0X06)
          {
           Probe_Error_Flag = Receive_Buff[1];
          }
          Skin_Color_Type = Receive_Buff[2];
          //Get_Mode_Flag = Receive_Buff[3]; //not use;
          Lamp_Type = Receive_Buff[3];
          Flash_Flag = Receive_Buff[4];
          Flash_Times_Level = Receive_Buff[5];
          if(Flash_Times_Level == 9)
          {
            Lowlamp_Flag = 1;
          }else{
            Lowlamp_Flag = 0;
          }
          Lowlamp_Flag = Receive_Buff[6];
          if((Flash_Flag)&&( Mode_Flag != OFF_FLASH_MODE)&&(Full_Charge_Flag))
          {
            Flash_Ready = 1;
            time_out_power_off = 0;
          }else{
            Flash_Ready = 0;
          }
        }
      }else{
        Communication_Error_Count++;
        if(Communication_Error_Count == 100)
        {
          Recevie_Data_Error = 1;
        }
      }
      //Delay_us(300);  
      da[0] = Lock_Flag;
      da[1] = Full_Charge_Flag;
      if((Mode_Flag != OFF_FLASH_MODE)&&(Mode_Flag !=0))
      {
        da[2] = Mode_Flag;
      }else{
        da[2] = STANDARD_MODE;
      }
      da[3] = Flash_Ready;
      if((HV_Level_Dowm_3 == 1)||(HV_Level_Dowm_5 == 1))//灯头led与level_led同步闪烁
      {
         da[4] = 9;
      }else{ 
             da[4] = HV_Level; 
           }
      Send_Request(FUN_No15,da,5); 
      Delay_us(300);
      UART_DE_R;
    }
  }
}