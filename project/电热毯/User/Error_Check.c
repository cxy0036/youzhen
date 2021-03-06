#include "Error_Check.h"
#include "ADC.H"
#include "Error_Check.h"
#include "EEPROM.h"
#include "delay.h"

uint8_t Fan_Error_Flag = 0;
uint8_t Cartridge_Error_Flag = 0;
uint8_t Error_Flag = 0;
uint8_t Lamp_Type = 0;

extern uint8_t EEPROM_Error;

void Fan_Error_Check(void)
{
	uint16_t readdata;
	readdata = ADC_Read_Channel_Fun(ADC_FAN_CHANNEL);
	if((readdata < FAN_OPEN_ERROR_THR) || (readdata > FAN_SHORT_ERROR_THR))
	{
		
		Delay_N_5us(10);
		readdata = ADC_Read_Channel_Fun(ADC_FAN_CHANNEL);
		if((readdata < FAN_OPEN_ERROR_THR) || (readdata > FAN_SHORT_ERROR_THR))
		{
			Fan_Error_Flag = 1;
		}
	}else{
		Fan_Error_Flag = 0;
	}
}

void Cartridge_Error_Check(void)
{
	uint16_t readdata;
	readdata = ADC_Read_Channel_Fun(ADC_CARTRIDG_CHANNEL);
	if(readdata < CARTRIDGE_ERROR_THR)
	{
		Delay_N_5us(10);
		readdata = ADC_Read_Channel_Fun(ADC_CARTRIDG_CHANNEL);
		if(readdata < CARTRIDGE_ERROR_THR)
		{
		  Cartridge_Error_Flag = 1;
		}
	}else{
		Cartridge_Error_Flag = 0;
	}
}

void Cartridge_Type_Check(void)
{
	uint16_t readdata;
	readdata = ADC_Read_Channel_Fun(ADC_CARTRIDG_CHANNEL);
	if((readdata < CARTRIDGE_TYPE_1_H) && (readdata > CARTRIDGE_TYPE_1_L))
	{
		Lamp_Type = 1;
	}else{
		if(readdata > CARTRIDGE_TYPE_2)
		{
			Lamp_Type = 2;
		}
		if((readdata < CARTRIDGE_TYPE_3_H) && (readdata > CARTRIDGE_TYPE_3_L))
		{
			Lamp_Type = 3;
		}
	}
}

void Error_Check(void)
{
	Fan_Error_Check();
	Cartridge_Error_Check();
	//if(Cartridge_Error_Flag || EEPROM_Error|| Fan_Error_Flag)
	if(Cartridge_Error_Flag || Fan_Error_Flag || EEPROM_Error)	
	{
		Error_Flag = 1;
	}else{
		Error_Flag = 0;
	}
}