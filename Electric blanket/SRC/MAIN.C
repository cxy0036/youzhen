#include "SN8F5702.H"
//#include "delay.h"
#include "INIT.H"
//#include "key.h"


void main(void)
{
	uint8_t i =9;
	WDTR = 0x5A;
	Init_Fun();
	//Uart_Init();
	//EEPROM_IO();
	//Delay_Init();
	IEN0 |= 0x80;
	while(1)
	{
		
		//Error_Check();
		//Key_Process();
		//Analysis_Request();		
    //EEPROM_Write_Check();		
	}
}
