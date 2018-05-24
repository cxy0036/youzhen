#include "key.h"
#include "Flash_Control.h"
#include "Error_Check.h"
#include "skin_color_check.h"
#include "protocol.h"
#include "UART.h"
#include "delay.h"

volatile uint8_t Key_Read = 0;
uint8_t Key_Trig = 0;
uint8_t Key_Count = 0;
uint8_t No_Skin_Flag = 0;
uint8_t Standard_Flash_Lock = 0;
uint8_t  Flash_Key_Push = 0;
//uint16_t Key_Delay_Count = 0;
//uint16_t Flash_Key_Delay_Count = 0;

uint8_t Key_Push = 0;
uint8_t Skin_Tpye5_Count = 0;
uint8_t Full_Charge_Flag = 0;
uint8_t Low_Lamp;
uint8_t First_Push = 1;
uint8_t Flash_Flag = 0;
uint16_t Flash_Delay_Count = 0;
//uint8_t Skin_Tpye5_Count;
//uint8_t Mode_Change = 0;

uint8_t Mode_Type = Standard_Mode;

extern uint8_t Flash_Times_Level;
extern uint8_t Low_Lamp ;
extern uint8_t Skin_Tpye;

extern uint8_t Lock_Flag;
extern uint8_t Error_Flag;
//extern uint8_t Get_Mode_Type;
extern uint8_t Flash_Ready;


void Key_IO_Init(void)
{
	P1M &= ~0x01;   /* P10:Touch Input */ 
	P0M &= ~0x01;   /* P00:Flash SW Input */
	P1UR &= ~0x01;   /* P10:Disable Pull up */
	P0UR |= 0x01;   /* P00:enable Pull up */
}


void Key_Scan(void)
{
	uint8_t temp;
	temp = P1 << 1;
	Key_Read = ((P0 & 0x01) | (temp & 0x02)) ^ 0xFF;  /* not */
	Key_Trig = Key_Read & (Key_Read ^ Key_Count);
	Key_Count = Key_Read;
}


void Key_Process(void)
{
	if(!Lock_Flag)
	{
		Key_Scan();
		if((Key_Count & TOUCH_PIN)&&(!Flash_Key_Push))
		{
			Skin_Color_Check();
			if(Key_Count & FLASH_SW_PIN)
			{
				if(Key_Trig & FLASH_SW_PIN)
				{
					Flash_Key_Push = 1;
					Standard_Flash_Lock = 1;
				}
			}
		}else{
      if(Key_Count & TOUCH_PIN)	
			{
				Skin_Color_Check();	
			}
			else{
				Skin_Tpye = 0;
			}				
			if(!(Key_Count & FLASH_SW_PIN))
			{
				Standard_Flash_Lock = 0;
				Flash_Key_Push = 0;
			}
		}
			//Skin_Color_Check();	
			if((!(P1&0x01))&&(Full_Charge_Flag)&&(Skin_Tpye == 5))
			{
		      Skin_Color_Check();	
					Skin_Tpye5_Count++;
					if(Skin_Tpye5_Count == 150)
					{
						//Full_Charge_LED_TOG;
						Skin_Tpye5_Count = 0;
						No_Skin_Flag ^= 1;
						//Delay_ms(200);
					}
			}else{
				No_Skin_Flag = 1;
			}
		 if(Key_Count & TOUCH_PIN)	
			{
				Skin_Color_Check();	
			}
			else{
				Skin_Tpye = 0;
			}				
		
		if(Full_Charge_Flag && (!(Error_Flag)) && (!(Low_Lamp)) && (!(Skin_Tpye == 5))) 
		{		
			switch(Mode_Type){
			case Auto_Mode : Auto_Flash();break;
			case Auto_Multi_Mode : Auto_Multi_Flash();break;
			case Standard_Mode : Standard_Flash();break;
			case Standard_Multi_Mode : Standard_Multi_Flash();break;
			default : break;			 
			}
		}
	}
}


void Auto_Flash(void)
{
	uint16_t delay_count;
  if(Key_Count & TOUCH_PIN)
	{
		Skin_Color_Check();
		if(First_Push)
		{
			delay_count = 400;			
		}else{
			delay_count = 400;
		}
		Flash_Delay_Count ++;
		if(Flash_Delay_Count == delay_count)
		{			
			Flash_Delay_Count = delay_count - 1;
			Flash_Flag = 1;
			if(Flash_Ready)
			{
				First_Push = 0;
				Flash_Ready = 0;
				Flash_Flag = 0;
				Full_Charge_Flag = 0;
				Flash_Delay_Count = 0;
				Full_Charge_LED_Off;
				Flash_Control(1);						  		  
			}			
		}	 
  }else{
		Flash_Ready = 0;
		Flash_Flag = 0;
		Skin_Tpye = 0;
		First_Push = 1;
		Flash_Delay_Count = 0;
	}
}

void Auto_Multi_Flash(void)
{
	uint16_t delay_count;
  if(Key_Count & TOUCH_PIN)
	{
		Skin_Color_Check();
		if(First_Push)
		{
			delay_count = 400;			
		}else{
			delay_count = 400;
		}
		Flash_Delay_Count ++;
		if(Flash_Delay_Count == delay_count)
		{			
			Flash_Delay_Count = delay_count - 1;		
			Flash_Flag = 1;
			if(Flash_Ready)
			{
				First_Push = 0;
				Flash_Ready = 0;
				Flash_Flag = 0;
				Full_Charge_Flag = 0;
				Flash_Delay_Count = 0;
				Full_Charge_LED_Off;
		    Flash_Control(4);	  
			}
		}
	}else{
		Flash_Ready = 0;
		Flash_Flag = 0;
		Skin_Tpye = 0;
		First_Push = 1;
		Flash_Delay_Count = 0;
	}
}

void Standard_Flash(void)
{
	if(Standard_Flash_Lock)
	{
		Flash_Flag = 1;
		if(Flash_Ready)
		{
			Standard_Flash_Lock = 0;
			Flash_Ready = 0;
			Flash_Flag = 0;
			Full_Charge_Flag = 0;
			Full_Charge_LED_Off;
			Flash_Control(1);					
		}
	}
}


void Standard_Multi_Flash(void)
{
	if(Standard_Flash_Lock)
	{
		Flash_Flag = 1;
		if(Flash_Ready)
		{
			Standard_Flash_Lock = 0;
			Flash_Ready = 0;
			Flash_Flag = 0;
			Full_Charge_Flag = 0;
			Full_Charge_LED_Off;
			Flash_Control(4);										
		}
	}
}
