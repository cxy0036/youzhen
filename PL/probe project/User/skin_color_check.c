#include "skin_color_check.h"
#include "ADC.h"
#include "delay.h"

uint8_t Skin_Tpye = 0;
uint16_t readdata;
//uint16_t readdataR;
//uint16_t readdataG;
//uint16_t readdataB;
extern uint8_t ADD ,DEC,add_,dec_;
extern void Uart_SendByte(uint8_t Data);

void Skin_Color_Check(void)
{
	readdata = ADC_Read_Channel_Fun(ADC_RGB_R_CHANNEL);
	Uart_SendByte(readdata);
	Uart_SendByte(0x99);
	//readdataG = ADC_Read_Channel_Fun(ADC_RGB_G_CHANNEL);
	if(DEC){readdata = readdata - readdata_1V*dec_;DEC = 0;}
	if(ADD){readdata = readdata + readdata_1V*add_;ADD = 0;}
	Uart_SendByte(readdata);
	Uart_SendByte(0x66);
	if((readdata < COLOR_TPYE_1_H) && (readdata > COLOR_TPYE_1_L))
	{	 
			Skin_Tpye = 1;		
	}		
	if((readdata < COLOR_TPYE_3_H) && (readdata > COLOR_TPYE_3_L))
	{	 
			Skin_Tpye =3;					
	}	
	if((readdata < COLOR_TPYE_4_H) && (readdata > COLOR_TPYE_4_L))
	{	 
			Skin_Tpye = 4;
	}	
	if((readdata < COLOR_TPYE_5_H) && (readdata > COLOR_TPYE_5_L))
	{	 
			Skin_Tpye = 5;				
	}	
}