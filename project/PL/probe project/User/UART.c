#include "UART.h"
#include "protocol.h"
#include "queue.h"
#include "delay.h"

uint8_t Receive_Buff[15];
uint8_t Receive_Count = 0;
uint8_t Receive_Data_Length;
uint8_t Head_Flag = 0;
uint8_t Receive_Buffer_Full_Flag = 0;

extern uint8_t Analysis_Lock; 

void Uart_Init(void)
{
	/* IO Initialize */
	P05 = 1;
	UART_SEND;   /* transmission */
	P0M |= 0x24;  /* P0.5-->TX:OUT,DIRECTION_PIN P0.2:OUT */
	P0M &= ~0x40;  /* P0.6-->RX:IN */
	P0OC &= ~0x03;  /* P0.5 P0.6 OD disable */
	P0UR &= ~0x24;  /* P0.5,P0.2 Pull_up disable */
	P0UR |= 0x40;  /* P0.6 Pull_up able */
	
	PCON &= ~0x80;
	S0CON = 0x50;  /* Uart model 1, enable recvied, clean flag*/
	S0CON2 = 0x80;  /* baud rate controlled by S0RELH,S0RELL */
	S0RELH = 0x03;  /* Baud rate 38400 */
	S0RELL = 0xF3;
	UART_RECEIVE;   /* transmission */
  IEN0 |= 0x10;   /* enable uart Interrupt */
}

void Uart_SendByte(uint8_t Data)
{
 
 S0BUF = Data;
 while(!(S0CON & 0x02));
 //S0BUF = Data;
 S0CON &= ~0x02;
 //Delay_N_10us(10);

}


void UartInterrupt(void) interrupt ISRUart
{
	if(RI0 == 1)
	{
		RI0 = 0;			
		if(!Head_Flag)
		{
			if(S0BUF == BUS_HEADER)
			{
				Head_Flag = 1;
			}
		}else{
			if(S0BUF != BUS_END)
			{
       if(!Analysis_Lock)
			 {				 
				if(Receive_Count < 15)
				{
					Receive_Buffer_Full_Flag = 0;
					Receive_Buff[Receive_Count] = S0BUF;
					Receive_Count++;
				}else{
					Head_Flag = 0;
				  Receive_Count = 0;
				}
			 }else{
				 Head_Flag = 0;
				 Receive_Count = 0;
			 }				
			}else{		 
				 Receive_Data_Length = Receive_Count;
				 Head_Flag = 0;
				 Receive_Count = 0;
				 Receive_Buffer_Full_Flag = 1;
			}	 
		}
	}
}	