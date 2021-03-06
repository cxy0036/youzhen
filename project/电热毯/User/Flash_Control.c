#include "Flash_Control.h"
#include "protocol.h"
#include "EEPROM.h"
#include "UART.h"
#include "delay.h"
#include "queue.h"
#include "key.h"
#include "skin_color_check.h"

volatile uint32_t Flash_Times = 0;
volatile uint32_t Flash_Times1 = 0;
volatile uint32_t Flash_Times2 = 0;
volatile uint32_t Flash_Times3 = 0;

//uint8_t i = 0;
uint8_t Flash_Level = 1;
uint8_t Flash_Off_Flag = 0;
uint8_t EEPROM_Write_Number_Error;
uint8_t Check_Write_EEPROM_Timeout = 0;

extern uint8_t Flash_Times_Level;
extern uint8_t Receive_Buff[15];
extern uint8_t Receive_Buffer_Full_Flag;
extern uint8_t Low_Lamp;
extern uint8_t Mode_Type;

void Flash_IO_Init(void)
{
	P07 = 0; //Trig Off;
	P01 = 0; //Full_charge LED Off;
	P0M |= 0x82;  //trig pin, full charge pin out mode;
	P0UR &= ~0x82; //disable pull up; 
}

void Flash_Control(uint8_t plus_times)
{
	uint16_t Time_Out;
	uint16_t delay_count = 0;
	uint8_t Data_Check_OK;
	uint8_t flash_request = 0;
	uint8_t Flash_Start = 1;
  uint8_t Trig_Flag = 1;
	uint8_t Flash_OK_Flag = 0;
	uint8_t Charge_OK_Flag =0;
	uint8_t Flash_Ready_OK_Flag = 0;
	uint8_t Touch_Key_Not_Release = 1;
	uint8_t Multi_Flag;
	if(plus_times > 1)
	{
		Multi_Flag = 1;
	}else{
		Multi_Flag = 0;
	}
	//Delay_ms(3);
	Send_Data(&Flash_Start,1);
	while(plus_times)
	{				
		Time_Out = 65530;
		delay_count = 0;
		while(!Flash_OK_Flag)
		{
			Time_Out--;
			if(P1&0x01)  //Touch Release
			{	
				//Delay_N_5us(10);
				if(P1&0x01)
				{
					Touch_Key_Not_Release = 0;
					break;
				}
			}
			if(Receive_Buffer_Full_Flag)
			{
				Receive_Buffer_Full_Flag = 0;
				Data_Check_OK = Data_Check();
				if(!Data_Check_OK)
				{
					if(Receive_Buff[0] == FUN_No5)
					{
						Flash_OK_Flag = 1;
						//Flash_OK_Flag = Receive_Buff[2];
					}
				}
			}
			if(Time_Out==0)
			{
				delay_count++;
				Time_Out = 65530;
				if(delay_count==2500)
				{
					//Trig_Off;
					return ;
				}
			}
		}		
		Flash_OK_Flag = 0;
		plus_times--;		
		Send_Data(&Touch_Key_Not_Release,1);
		if(!Touch_Key_Not_Release)
		{
			break;
		}
	}
	
	if(Flash_Level <= 3)
	{
		if(Multi_Flag)
		{
		 Multi_Flash_Times(plus_times,1,Touch_Key_Not_Release);
		}else{
			Flash_Times ++;
		}
	}else{
		if(Flash_Level <= 5)
		{
      if(Multi_Flag)
		  {			
			 Multi_Flash_Times(plus_times,2,Touch_Key_Not_Release);	
			}else{
				Flash_Times += 3;
			}
		}else{
			if(Multi_Flag)
			{
			 Multi_Flash_Times(plus_times,3,Touch_Key_Not_Release);
			}else{
				Flash_Times += 5;
			}
		}
	}
	if(Flash_Times > Flash_Times_Level8) //>89999 : 90000 ~ 99999
	{
		Flash_Times_Level = Flash_Times_Level_1;
	}
	if(Flash_Times > Flash_Times_Level9)
	{
		Low_Lamp = 1;
		Flash_Times_Level = 9;
	}else{
		Low_Lamp = 0;
	}
	Delay_ms(5);
	//IEN0 &= ~0x80; //disable uart interrupt  IEN0 |= 0x80;
	Write_Num_TO_EEPROM(Flash_Times);//write Data to eeprom, disable write;
	while(Flash_Times != Read_Num_From_EEPROM())
	{
		Check_Write_EEPROM_Timeout++;
		Write_Num_TO_EEPROM(Flash_Times);
		if(Check_Write_EEPROM_Timeout == 3)
		{
			Check_Write_EEPROM_Timeout = 0;
			EEPROM_Write_Number_Error = 1;
			return;
		}
	}
	//IEN0 |= 0x80;  //enable uart interrupt
	EEPROM_Write_Number_Error = 0;
}

void Multi_Flash_Times(uint8_t plus,uint8_t level_rang,\
	                     uint8_t touch_status)
{
	if(!touch_status)
	{
		switch(plus){
			case 0: Flash_Times += 3*level_rang;break;
			case 1: Flash_Times += 2*level_rang;break;
			case 2: Flash_Times += 1*level_rang;break;
			default:break;
		}
	}else{
		Flash_Times += 4*level_rang;
	}
}