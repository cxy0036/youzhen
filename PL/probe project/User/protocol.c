#include "protocol.h"
#include "UART.h"
#include "delay.h"
#include "Error_Check.h"
#include "Flash_Control.h"
#include "skin_color_check.h"
#include "key.h"
#include "ADC.h"

uint8_t Analysis_Lock = 0;
uint8_t Flash_Times_Level = 0;
uint8_t Lock_Flag = 1;
//uint8_t Get_Mode_Type = 1;
uint8_t Flash_Ready = 0;

extern uint8_t No_Skin_Flag;
extern uint8_t Mode_Type;
extern uint8_t Key_Count;
extern volatile uint32_t Flash_Times;
extern uint8_t Error_Flag;
extern uint8_t Full_Charge_Flag;
extern uint8_t Flash_Level;
extern uint8_t Lamp_Type;
extern uint8_t Skin_Tpye;
extern uint8_t Flash_Flag;
extern uint8_t Mode_Change;
extern uint8_t Low_Lamp;
extern uint16_t readdata;

static const uint8_t code CRC_Table[256]={
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


extern uint8_t Receive_Buff[15];
extern uint8_t Receive_Buffer_Full_Flag;
extern uint8_t Receive_Data_Length;


uint8_t Crc_Caculate(uint8_t *d,uint8_t length)
{
	uint8_t crc = 0;
	while(length--)
	{
	  crc = CRC_Table[crc ^ *d++];
	}
	return crc;
}


void Send_Data(uint8_t *Data,uint8_t length)
{
	uint8_t i,crc,temp[8];
	temp[0] = length;
	UART_SEND;
	Delay_N_10us(30);
  IEN0 &= ~0x10;  /* disable uart Interrupt */
	for(i=1;i<length+1;i++)
	{
		temp[i] = *Data;
		Data++;
	}
	crc = Crc_Caculate(&temp,length+1);
	temp[length+1] = crc;
	Uart_SendByte(BUS_HEADER);
  for(i=0;i<length+2;i++)
  {
    Uart_SendByte(temp[i]);
	}		
	Uart_SendByte(BUS_END);
	IEN0 |= 0x10;  /* enable uart Interrupt */ 
	Delay_N_10us(30);
  UART_RECEIVE;
}

uint8_t Data_Check(void)
{
	uint8_t Data_Check_OK;
	Analysis_Lock = 1;
	Data_Check_OK = Crc_Caculate(&Receive_Buff,Receive_Data_Length);
	Analysis_Lock = 0;
	return Data_Check_OK;
}
//tong xun
void Analysis_Request(void)
{
	uint8_t Data_Check_OK;
	if(Receive_Buffer_Full_Flag)
	{
		Receive_Buffer_Full_Flag = 0;
		Analysis_Lock = 1;
		Data_Check_OK = Data_Check();
		if(!Data_Check_OK)
		{		
			Data_Check_OK = 1;  
			switch(Receive_Buff[0]){
				case FUN_No1 : No1_Fun();break;
				case FUN_No2 : No2_Fun();break;//Get_Flash_Times_Level
				case FUN_No3 : No3_Fun();break;/* get skin color  */
				//case FUN_No7 : No7_Fun();break;
				//case FUN_No8 : No8_Fun();break;
				case FUN_No11: No11_Fun();break;/* Get Lamp Type */
				//case FUN_No12: No12_Fun();break;
				//case FUN_No13: No13_Fun();break;
				//case FUN_No14: No14_Fun();break;  /* skin type */
				case FUN_No15: No15_Fun();break;
				default : break;
			}
		}
		Analysis_Lock = 0;
	}
}
 
void No1_Fun(void)    /* get flash times */
{
	uint8_t temp[4];
	temp[0] = (uint8_t)Flash_Times;
	temp[1] = (uint8_t)(Flash_Times>>8);
	temp[2] = (uint8_t)(Flash_Times>>16);
	temp[3] = (uint8_t)(Flash_Times>>24);
	Send_Data(&temp,4);
}


void No2_Fun(void)   /* get flash times level */
{
	Send_Data(&Flash_Times_Level,1);
}

void No3_Fun(void)   /* get skin color  */
{
	//readdata = ADC_Read_Channel_Fun(ADC_RGB_CHANNEL);
	uint8_t temp[4];
	temp[0] = (uint8_t)readdata;
	temp[1] = (uint8_t)(readdata>>8);
	temp[2] = (uint8_t)(readdata>>16);
	temp[3] = (uint8_t)(readdata>>24);
	Send_Data(&temp,4);
}



/*void No6_Fun(void)   /* get ERROR flag */
/*{
	Send_Data(&Error_Flag,1);
}*/
/*
void No7_Fun(void)   /* Full Charge flag */
/*{
	Full_Charge_Flag = Receive_Buff[2];	
}*/
/*
void No8_Fun(void)  /* Mode Type */
/*{
	Mode_Type = Receive_Buff[2];
}*/

void No11_Fun(void)  /* Get Lamp Type */
{
	Send_Data(&Lamp_Type,1);
}
/*
void No12_Fun(void)  /* get Mode Type */
/*{
	Send_Data(&Mode_Type,1);
}*/

/*void No13_Fun(void)  /* Lock Flag */
/*{
	Lock_Flag = Receive_Buff[2];
}*/
 /*
void No14_Fun(void)  /* skin type */
/*{
	Send_Data(&Skin_Tpye,1);
}*/

void No15_Fun(void)  /*all */
{
	uint8_t da[6];
	Lock_Flag = Receive_Buff[2];
	Full_Charge_Flag = Receive_Buff[3];
	Mode_Type = Receive_Buff[4];
	Flash_Ready = Receive_Buff[5];
	Flash_Level = Receive_Buff[6];
	if(Full_Charge_Flag)
	{
		if(!(No_Skin_Flag))//����ɫ��˸����
    {
			Full_Charge_LED_Off;
    }else{
		  Full_Charge_LED_On;
		}			
	}else{
		Full_Charge_LED_Off;
	}
	da[0] = Error_Flag;
	da[1] = Skin_Tpye;
	//da[2] = Lamp_Type;
	da[2] = Lamp_Type;
	da[3] = Flash_Flag;
	da[4] = Flash_Times_Level;
	da[5] = Low_Lamp;
	Send_Data(&da,6);
}
