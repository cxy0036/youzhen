#include "init.h"
#include "Error_Check.h"
#include "UART.h"
#include "EEPROM.h"
#include "delay.h"
#include "ADC.h"
#include "Flash_Control.h"
#include "key.h"
//#include ""

extern volatile uint32_t Flash_Times;
extern volatile uint32_t Flash_Times1;
extern volatile uint32_t Flash_Times2;
extern volatile uint32_t Flash_Times3;

extern uint8_t Lamp_Type;
extern uint8_t Flash_Times_Level;
extern uint8_t Low_Lamp;
extern uint8_t Mode_Type;
extern uint8_t EEPROM_ERROR_Flag;
//uint8_t Low_Lamp_count=0;
void Init_Fun(void)
{
	//uint8_t read_count= 0;
  //	uint8_t temp[4];
	UART_SEND; 
	Key_IO_Init();	
	Flash_IO_Init();
	EEPROM_IO();
	Delay_Init();
	Uart_Init();
	ADC_Init_Fun();		
	Cartridge_Type_Check();                     
	UART_RECEIVE;
	Mode_Type = Standard_Mode;
	Full_Charge_LED_On;	
	Delay_ms(1000);
	Full_Charge_LED_Off;
  //Write_Num_TO_EEPROM(89990);
	//EEPROM_Read_Data(temp,0x0000,4);
  Flash_Times1 = Read_Num_From_EEPROM();  
	Flash_Times2 = Read_Num_From_EEPROM();
	Flash_Times3 = Read_Num_From_EEPROM();
	if(Flash_Times1==Flash_Times2)
	{
		Flash_Times = Flash_Times1;
	}else{
			if(Flash_Times3==Flash_Times2)
			{
				Flash_Times = Flash_Times2;
			}else{
				Flash_Times = Read_Num_From_EEPROM();
			}
		}
	
  //while(Flash_Times == 0)
	//{
		//Flash_Times = Read_Num_From_EEPROM(); 
    //read_count++;
    //if(read_count==10)		
		//{
			//break;
			//set error;
		//}
	//}	
	if(!EEPROM_ERROR_Flag)
	{	
		if(Flash_Times > Flash_Times_Level9)  //>99999
		{			 			  
				Low_Lamp = 1;
			  Flash_Times_Level = 9;
		}else{
			Low_Lamp = 0;
			if(Flash_Times > Flash_Times_Level8) //>89999 : 90000 ~ 99999
			{
				Flash_Times_Level = Flash_Times_Level_1;
			}else{
				if(Flash_Times > Flash_Times_Level7) // >79999 : 80000 ~ 89999
				{
					Flash_Times_Level = Flash_Times_Level_2;
				}else{
					if(Flash_Times > Flash_Times_Level6) //>69999 : 70000 ~ 79999
					{
						Flash_Times_Level = Flash_Times_Level_3;
					}else{
						if(Flash_Times >= Flash_Times_Level5) //>59999 : 60000 ~ 69999
						{
							Flash_Times_Level = Flash_Times_Level_4;
						}else{
							if(Flash_Times >= Flash_Times_Level4) //>49999 : 50000 ~ 59999
							{
								Flash_Times_Level = Flash_Times_Level_5;
							}else{
								if(Flash_Times >= Flash_Times_Level3) //>39999 : 40000 ~ 49999
								{
									Flash_Times_Level = Flash_Times_Level_6;
								}else{
									if(Flash_Times >= Flash_Times_Level2) //>29999 : 30000 ~ 39999
									{
										Flash_Times_Level = Flash_Times_Level_7;
									}else{
										if(Flash_Times > Flash_Times_Level1) //>19999 : 20000 ~ 29999
										{
											Flash_Times_Level = Flash_Times_Level_8;
										}else{                               //>=0 : 0~19999
											Flash_Times_Level = Flash_Times_Level_9;
										}									
									}
								}
							}
						}
					}
				}
			}
		}	
	}
}
