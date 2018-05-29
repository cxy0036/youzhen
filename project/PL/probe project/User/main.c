#include "main.h"
#include "SN8F5702.H"
#include "delay.h"
#include "protocol.h"
#include "ADC.h"
#include "EEPROM.h"
#include "init.h"
#include "Error_Check.h"
#include "UART.h"
#include "Flash_Control.h"
#include "key.h"

extern uint8_t Receive_Buffer_Full_Flag;

void main(void)
{
	//uint16_t rnum; //flash_count = 32,
	//uint8_t data_arry[4];
	//uint8_t t=0x80,i;
	uint8_t i =9;
	WDTR = 0x5A;
	Init_Fun();
	//Uart_Init();
	//EEPROM_IO();
	//Delay_Init();
	IEN0 |= 0x80;
	while(1)
	{
		
		Error_Check();
		Key_Process();
		Analysis_Request();		
    EEPROM_Write_Check();		
	}
}
