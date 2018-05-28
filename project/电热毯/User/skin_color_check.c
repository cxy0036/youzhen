#include "skin_color_check.h"
#include "ADC.h"
#include "delay.h"

uint8_t Skin_Tpye = 0;

void Skin_Color_Check(void)
{
	uint16_t readdata;
	readdata = ADC_Read_Channel_Fun(ADC_RGB_CHANNEL);
	if((readdata < COLOR_TPYE_1_H) && (readdata > COLOR_TPYE_1_L))
	{	 
		Delay_N_5us(10);
	 	if((readdata < COLOR_TPYE_1_H) && (readdata > COLOR_TPYE_1_L))
	  {	
			Skin_Tpye = 1;
	  } 					
	}		
	
	if((readdata < COLOR_TPYE_3_H) && (readdata > COLOR_TPYE_3_L))
	{	 
		Delay_N_5us(10);
	 	if((readdata < COLOR_TPYE_3_H) && (readdata > COLOR_TPYE_3_L))
	  {	
			Skin_Tpye =3;
	  } 					
	}	
	if((readdata < COLOR_TPYE_4_H) && (readdata > COLOR_TPYE_4_L))
	{	 
		Delay_N_5us(10);
	 	if((readdata < COLOR_TPYE_4_H) && (readdata > COLOR_TPYE_4_L))
	  {	
			Skin_Tpye = 4;
	  } 					
	}	
	if((readdata < COLOR_TPYE_5_H) && (readdata > COLOR_TPYE_5_L))
	{	 
		Delay_N_5us(10);
	 	if((readdata < COLOR_TPYE_5_H) && (readdata > COLOR_TPYE_5_L))
	  {	
			Skin_Tpye = 5;
	  } 					
	}	
	
	
	
}