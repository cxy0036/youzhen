#include "skin_color_check.h"
#include "ADC.h"
#include "delay.h"

uint8_t Skin_Tpye = 0;
uint16_t readdata;
//uint16_t readdataR;
//uint16_t readdataG;
//uint16_t readdataB;
extern uint8_t adc_level,adc_level_flag;
void ADC_adjust(void);

void Skin_Color_Check(void)
{
	readdata = ADC_Read_Channel_Fun(ADC_RGB_R_CHANNEL);
		if(adc_level>0x0f)
			readdata = readdata + readdata_1V*(adc_level-0x0f);
		else if(adc_level<0x0f)
			readdata = readdata - readdata_1V*(0x0f-adc_level);
//		readdata = readdata - 500;
	//readdataG = ADC_Read_Channel_Fun(ADC_RGB_G_CHANNEL);
	//readdata = readdataG - readdataR;
	//ADC_adjust();
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

//void ADC_adjust(void)
//{
//		if(adc_level_flag & 0x81)
//			readdata = readdata + readdata_1V*adc_level;
//		else if(adc_level_flag & 0x82)
//			readdata = readdata - readdata_1V*adc_level;
//}
	