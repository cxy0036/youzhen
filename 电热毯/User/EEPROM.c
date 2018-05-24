/**
  ******************************************************************************
  * @file    EEPROM.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   Implement 11LC010 UN/IO BUS EEPROM Read , Write ,command Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "EEPROM.h"
#include "delay.h"
#include "Flash_Control.h"

/* Global variable -----------------------------------------------------------*/
uint8_t EEPROM_ERROR_Flag = 0;  /* EEPROM Read, Write, Timeout Error Flag */
uint8_t EEPROM_Error = 0;
uint8_t EEPROM_Error_Count = 0;
extern uint8_t EEPROM_Write_Number_Error;
extern volatile uint32_t Flash_Times;

/* Public functions ----------------------------------------------------------*/

/*
********************************************************************************
*                           EEPROM INITIALIZATION FUNCTIONS
*
* Description: This function Initial EEPROM IO PORT.
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/
void EEPROM_IO(void)
{
	P07 = 1;
	//P0OC |= 0x04;
	P0UR &= ~0x80;  //disable pull up;
	P0M |= 0x80; //P07 output mode;	
}

/*
********************************************************************************
*                           EEPROM MAK FUNCTIONS
*
* Description: This function implement EEPROM Master Ask Function.
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/

void EEPROM_MAK(void)                    
{ 
  SET_IO_LOW(EEPROM_UNIO_PIN);        /* SCIO Output 0*/
  //Delay_N_5us(6);
	Delay_Hfbit();
  SET_IO_HIGH(EEPROM_UNIO_PIN);        /* SCIO Output 1*/
  //Delay_N_5us(6);
	Delay_Hfbit();
}

/*
********************************************************************************
*                           EEPROM NoMAK FUNCTIONS
*
* Description: This function implement EEPROM NoMAK Function.
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/

void EEPROM_NoMAK(void)                  
{ 
  SET_IO_HIGH(EEPROM_UNIO_PIN);        /* SCIO Output 1*/
  //Delay_N_5us(6);
	Delay_Hfbit();
  SET_IO_LOW(EEPROM_UNIO_PIN);        /* SCIO Output 0*/
  //Delay_N_5us(6);
	Delay_Hfbit();
}

/*
********************************************************************************
*                           EEPROM SAK FUNCTIONS
*
* Description: This function implement EEPROM Slave Ask Function.
*
* Arguments  : none
*
* Note(s)    : If ask timeout, Than set EEPROM Error Flag.
********************************************************************************
*/
void EEPROM_SAK(void)
{ 
  uint8_t timeout = EEPROM_WAIT;
	P07 = 1;
	P0M &= ~0x80; //P07 Input mode;
  while(EEPROM_UNIO_PIN)
  {
    timeout--;
    if(timeout == 0)
    {
      //EEPROM_ERROR("SAK Time OUT HI!! errorcode=%d \n",0);
      EEPROM_ERROR_Flag = 1;
			P0M |= 0x80;
      return ;			
    }else{
      EEPROM_ERROR_Flag = 0;
    }
  }
  //Delay_N_10us(6);  //60us
	Delay_Hfbit();
	Delay_Hfbit();
  if(EEPROM_UNIO_PIN == 0)
  {
    EEPROM_ERROR_Flag = 2;
		P0M |= 0x80;
    return ;
  }else{
    EEPROM_ERROR_Flag = 0;		
    //SET_IO_HIGH(EEPROM_UNIO_PIN);
		//P13 = 1;
		P0M |= 0x80; //P07 output mode;
  }
}

/*
********************************************************************************
*                           EEPROM NoSAK FUNCTIONS
*
* Description: This function implement EEPROM NoSAK Function.
*
* Arguments  : none
*
* Note(s)    : If NoAsk timeout, Than set EEPROM Error Flag..
********************************************************************************
*/
void EEPROM_NoSAK(void)
{ 
  uint16_t timeout = EEPROM_WAIT;
	P07 = 1;
	P0M &= ~0x80; //P07 Input mode;
  while(!(EEPROM_UNIO_PIN))        /* if SCIO is 0 */
  {
    timeout--;
    if(timeout == 0)
    {
      EEPROM_ERROR_Flag = 3;
			P0M |= 0x80;
      return  ;
    }else{
      EEPROM_ERROR_Flag = 0;
    }
  }
  //Delay_N_10us(6); //60us
	Delay_Hfbit();
	Delay_Hfbit();
	if(EEPROM_UNIO_PIN == 0)
	{
		EEPROM_ERROR_Flag = 4;
		return  ;
	}else{
		EEPROM_ERROR_Flag = 0;
		P0M |= 0x80;
	}
	//P13 = 1;
	//P2M |= 0x01; //P20 output mode;
  //SET_IO_DIR_OUT(EEPORM_UNIO_PORT,EEPROM_UNIO_PIN);     /* change direction Output */
}

/*
********************************************************************************
*                        WRITE ONE BYTE TO EEPROM FUNCTIONS
*
* Description: This function implement write one byte to EEPROM Timing Function,
* Is called by all need write Data functions.
*
* Arguments  : Write Data, one Byte
*
* Note(s)    : none.
********************************************************************************
*/
void EEPROM_Write_Byte(uint8_t write_data)
{
  uint8_t t = 0x80;
  while(t)
  {
    if(write_data & t)
    {
      SET_IO_LOW(EEPROM_UNIO_PIN);
      //Delay_N_5us(6);
			Delay_Hfbit();
      SET_IO_HIGH(EEPROM_UNIO_PIN);
      Delay_Hfbit();
			//Delay_N_5us(6);
    }else{
      SET_IO_HIGH(EEPROM_UNIO_PIN);
      //Delay_N_5us(6);
			Delay_Hfbit();
      SET_IO_LOW(EEPROM_UNIO_PIN);
      //Delay_N_5us(6);
			Delay_Hfbit();
    }
    t = t >> 1;
  }  
}

/*
********************************************************************************
*                        READ ONE BYTE FROM EEPROM FUNCTIONS
*
* Description: This function implement read one byte from EEPROM Function,
* Is called by read Data functions.
*
* Arguments  : none.
*
* Return : Read Data, One byte.
*
* Note(s)    : If read timeout, Than set EEPROM ERROR Flag.
********************************************************************************
*/
uint8_t EEPROM_Read_Byte(void)
{
	uint8_t t = 0x80;
	uint8_t readtmp = 0;
	P07 = 1;
	P0M &= ~0x80; //P20 Input mode;
  while(t)
  {
		Delay_Qbit2();
    if(EEPROM_UNIO_PIN == 0)
    {
			Delay_Hfbit();
      if(EEPROM_UNIO_PIN == 0)
      {
        EEPROM_ERROR_Flag = 5;
				P0M |= 0x80;
        return 0;
      }else{
        EEPROM_ERROR_Flag = 0;
        readtmp |= t;
      }
    }else{
		   	Delay_Hfbit();
        if(EEPROM_UNIO_PIN == 1)
        {
          EEPROM_ERROR_Flag = 6;
					P0M |= 0x80;
          return 0;
        }else{
          EEPROM_ERROR_Flag = 0;
          readtmp &= ~t;
        }
    }
		Delay_Qbit();
    t = t >> 1;
  }
	//P13 = 1;
	P0M |= 0x80; //P20 Output mode;
  return readtmp;
}

/*
********************************************************************************
*                        EEPROM  STAERT HEADER FUNCTIONS
*
* Description: This function implement EEPROM start header Function,
* Is called all EERPOM operation.
*
* Arguments  : none.
*
* Return : none.
*
* Note(s)    : If timeout, Than set EEPROM ERROR Flag.
********************************************************************************
*/

void EEPROM_StartHeader(void)
{
	SET_IO_LOW(EEPROM_UNIO_PIN);
  Delay_N_5us(4);
  SET_IO_HIGH(EEPROM_UNIO_PIN);
  Delay_N_10us(80);
  SET_IO_LOW(EEPROM_UNIO_PIN);
  Delay_N_5us(4);
  EEPROM_Write_Byte(EEPROM_STARTHEADER);
  EEPROM_MAK();
  EEPROM_NoSAK();
}


/*
********************************************************************************
*                        EEPROM READ DATA FUNCTIONS
*
* Description: This function implementEEPROM_Read_Data Function.
* Read N Data from EEPROM address to detination address.
*
* Arguments  : detination address, EEPROM Data addres, Number of Data.
*
* Return : none.
*
* Note(s)    : If timeout, Than set EEPROM ERROR Flag.
********************************************************************************
*/
/*
void EEPROM_Read_Data(uint8_t *data_arry, uint16_t read_addr, uint8_t read_num)
{
  uint8_t i,read_addr_tmpH,read_addr_tmpL;
  
  read_addr_tmpH = (read_addr >> 8) & 0xFF;
  read_addr_tmpL = read_addr & 0xFF;
  EEPROM_StartHeader();
  EEPROM_Write_Byte(EEPROM_DEVICE_ADDR);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(CMD_READ);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(read_addr_tmpH);
  EEPROM_MAK();
  EEPROM_SAK(); 
  EEPROM_Write_Byte(read_addr_tmpL);
  EEPROM_MAK();
  EEPROM_SAK();
	data_arry[0] = EEPROM_Read_Byte();
	EEPROM_MAK();
	EEPROM_SAK();
	data_arry[1] = EEPROM_Read_Byte();
	EEPROM_MAK();
	EEPROM_SAK();
	data_arry[2] = EEPROM_Read_Byte();
	EEPROM_MAK();
	EEPROM_SAK();
	data_arry[3] = EEPROM_Read_Byte();
	EEPROM_MAK();
	EEPROM_SAK();
	
  for(i=0;i<read_num;i++)
  {
    data_arry[i] = EEPROM_Read_Byte();
    if(i == (read_num-1))
    {
      EEPROM_NoMAK();
    }else{
			EEPROM_MAK();
    }		
    EEPROM_SAK();
  }
}

*/
/*
********************************************************************************
*                        EEPROM WRITE DATA FUNCTIONS
*
* Description: EEPROM_Write_Data Function.
* Wirte N Data to EEPROM address.
*
* Arguments  : Data address, EEPROM detination address, Number of Data.
*
* Return : none.
*
* Note(s)    : If timeout, Than set EEPROM ERROR Flag.
********************************************************************************
*/

void EEPROM_Write_Data(uint8_t *data_arry, uint16_t write_addr, uint8_t write_num)
{
  uint8_t i,write_addr_tmpH,write_addr_tmpL;
  
  write_addr_tmpH = (write_addr >> 8) & 0xFF;
  write_addr_tmpL = write_addr & 0xFF;
  EEPROM_Write_CMD(CMD_WREN);
  EEPROM_StartHeader();
  EEPROM_Write_Byte(EEPROM_DEVICE_ADDR);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(CMD_WRITE);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(write_addr_tmpH);
  EEPROM_MAK();
  EEPROM_SAK(); 
  EEPROM_Write_Byte(write_addr_tmpL);
  EEPROM_MAK();
  EEPROM_SAK();
	
  for(i=0;i<write_num;i++)
  {
    EEPROM_Write_Byte(data_arry[i]);
    if(i == (write_num - 1))
    {
      EEPROM_NoMAK();
    }else{
      EEPROM_MAK();
    }
    EEPROM_SAK();
  }
  Delay_ms(20);
}

/*
********************************************************************************
*                        EEPROM WRITE COMMAND FUNCTIONS
*
* Description: EEPROM_Write_CMD Function.
*
* Arguments  : Command.
*
* Return : none.
*
* Note(s)    : If timeout, Than set EEPROM ERROR Flag.
********************************************************************************
*/
void EEPROM_Write_CMD(uint8_t cmd)
{
  EEPROM_StartHeader();
  EEPROM_Write_Byte(EEPROM_DEVICE_ADDR);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(cmd);
  EEPROM_NoMAK();
  EEPROM_SAK();
  if((cmd == CMD_ERAL) || (cmd == CMD_SETAL))
  {
    Delay_ms(20);
  }
}

/*
********************************************************************************
*                        EEPROM READ STATUS FUNCTIONS
*
* Description: EEPROM_Read_SR Function.
*
* Arguments  : none.
*
* Return : EEPROM status Register Data.
*
* Note(s)    : If timeout, Than set EEPROM ERROR Flag.
********************************************************************************
*/
/*
uint8_t EEPROM_Read_SR(void)
{
  uint8_t Bit;
  
  EEPROM_StartHeader();
  
  EEPROM_Write_Byte(EEPROM_DEVICE_ADDR);
  EEPROM_MAK();
  EEPROM_SAK();
  
  EEPROM_Write_Byte(CMD_RDSR); /* write read status regist commad */
 /* EEPROM_MAK();
  EEPROM_SAK();
  Bit = EEPROM_Read_Byte();
  EEPROM_NoMAK();
  EEPROM_SAK();
  return Bit;
}
*/

/*
********************************************************************************
*                        EEPROM WRITE STATUS FUNCTIONS
*
* Description: EEPROM_Write_SR Function.
*
* Arguments  : The status Data.
*
* Return : none.
*
* Note(s)    : If timeout, Than set EEPROM ERROR Flag.
********************************************************************************
*/
/*
void EEPROM_Write_SR(uint8_t srdata)
{
  EEPROM_StartHeader(); /* start head */
  
/*  EEPROM_Write_Byte(EEPROM_DEVICE_ADDR);  /* write device address */
/*  EEPROM_MAK();
  EEPROM_SAK();
  
  EEPROM_Write_Byte(CMD_WRSR); /* write commad */
/*  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(srdata); /* write status Data */
/*  EEPROM_NoMAK();
  EEPROM_SAK();
  Delay_ms(10); /* delay Twc(5-10ms) time */
/*}
*/

/*
********************************************************************************
*                        WRITE NUMBER TO EEPROM FUNCTIONS
*
* Description: Write a 32 Bit(0-4294967295) number to EEPROM, Is use to write flash 
* times to EEPORM.
*
* Arguments  : The 32bit number Data.
*
* Return : none.
*
* Note(s)    : If timeout, Than set EEPROM ERROR Flag.  
*              If you want change the write address in EEPROM, You can change 
*              the macro DATA_ADDR in EEPROM.h.
********************************************************************************
*/
/*
void Write_Num_TO_EEPROM(uint32_t flash_count)
{
  uint8_t data_arry[4];
	uint8_t i;
  data_arry[3] = (uint8_t)(flash_count >> 24);  /* 32 Bit unmerge two 8 Bit Data */
 /* data_arry[2] = (uint8_t)(flash_count >> 16);
  data_arry[1] = (uint8_t)(flash_count >> 8);
  data_arry[0] = (uint8_t)(flash_count);
   /* write Data to DATA_ADDR in EEPROM */

	/*EEPROM_Write_CMD(CMD_WREN);
  EEPROM_StartHeader();
  EEPROM_Write_Byte(EEPROM_DEVICE_ADDR);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(CMD_WRITE);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(DATA_ADDR_H);
  EEPROM_MAK();
  EEPROM_SAK(); 
  EEPROM_Write_Byte(DATA_ADDR_L);
  EEPROM_MAK();
  EEPROM_SAK();
	
  for(i=0;i<4;i++)
  {
    EEPROM_Write_Byte(data_arry[i]);
    if(i == 3)
    {
      EEPROM_NoMAK();
    }else{
      EEPROM_MAK();
    }
    EEPROM_SAK();
  }
  Delay_ms(20);
  EEPROM_Write_CMD(CMD_WRDI);  /* Disable write, protect EEPROM Data*/
/*}*/

/*
********************************************************************************
*                        READ NUMBER FROM EEPROM FUNCTIONS
*
* Description: Read a 32 Bit(0-4294967295) number from EEPROM, Is use to read flash 
* times from EEPORM.
*
* Arguments  : none.
*
* Return : Read Number Data.
*
* Note(s)    : If timeout, Than set EEPROM ERROR Flag.
*              If you want change the write address in EEPROM, You can change 
*              the macro DATA_ADDR in EEPROM.h.
********************************************************************************
*/

void Write_Num_TO_EEPROM(uint32_t flash_count)
{
  uint8_t dataHH,dataHL,dataLH,dataLL,data_arry[4];
  dataHH = (uint8_t)(flash_count >> 24);  /* 32 bit unmerge two 8 bit ddata */
  dataHL = (uint8_t)(flash_count >> 16);
  dataLH = (uint8_t)(flash_count >> 8);
  dataLL = (uint8_t)(flash_count);
  data_arry[0] = dataLL;
  data_arry[1] = dataLH;
  data_arry[2] = dataHL;
  data_arry[3] = dataHH;
  EEPROM_Write_Data(data_arry,DATA_ADDR,4);  /* write data to DATA_ADDR in EEPROM */
  EEPROM_Write_CMD(CMD_WRDI);  /* Disable write, protect EEPROM Data*/
}

uint32_t Read_Num_From_EEPROM(void)
{
  uint8_t read_arry[4];
  uint32_t read_data;
	EEPROM_StartHeader();
  EEPROM_Write_Byte(EEPROM_DEVICE_ADDR);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(CMD_READ);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(DATA_ADDR_H);
  EEPROM_MAK();
  EEPROM_SAK(); 
  EEPROM_Write_Byte(DATA_ADDR_L);
  EEPROM_MAK();
  EEPROM_SAK();
	read_arry[0] = EEPROM_Read_Byte();
	EEPROM_MAK();
	EEPROM_SAK();
	read_arry[1] = EEPROM_Read_Byte();
	EEPROM_MAK();
	EEPROM_SAK();
	read_arry[2] = EEPROM_Read_Byte();
	EEPROM_MAK();
	EEPROM_SAK();
	read_arry[3] = EEPROM_Read_Byte();
	EEPROM_NoMAK();
	EEPROM_SAK();
	
  //EEPROM_Read_Data(read_arry,DATA_ADDR,4);  /* read two data from DATA_ADDR in EEPORM */
  //read_data = read_arry[3];  /* Two 8 bit data merge one 32 bit data */
	
  read_data = ((uint32_t)read_arry[3] << 24) | ((uint32_t)read_arry[2] << 16) | \
              ((uint32_t)read_arry[1] << 8) | (uint32_t)read_arry[0];
							
  return read_data;  /* return read data */
}
/*
uint8_t Read_Lamp_Type_From_EEPROM(void)
{
	uint8_t read_data;
	EEPROM_StartHeader();
  EEPROM_Write_Byte(EEPROM_DEVICE_ADDR);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(CMD_READ);
  EEPROM_MAK();
  EEPROM_SAK();
  EEPROM_Write_Byte(LAMP_TYPE_ADDR_H);
  EEPROM_MAK();
  EEPROM_SAK(); 
  EEPROM_Write_Byte(LAMP_TYPE_ADDR_L);
  EEPROM_MAK();
  EEPROM_SAK();
	read_data = EEPROM_Read_Byte();
	EEPROM_NoMAK();
	EEPROM_SAK();
	return read_data;
}
*/
void EEPROM_Write_Check(void)
{	
	if(EEPROM_Write_Number_Error)
	{		
		//IEN0 &= ~0x80;
//disable uart interrupt
		Write_Num_TO_EEPROM(Flash_Times);		
		if(Flash_Times == Read_Num_From_EEPROM())
		{
			EEPROM_Write_Number_Error = 0;
			EEPROM_Error_Count = 0;
			EEPROM_Error = 0;
		}else{
			EEPROM_Error_Count++;
			if(EEPROM_Error_Count == 20)
			{
				EEPROM_Error_Count = 0;
				EEPROM_Error = 1;
			}
		}
		//IEN0 |= 0x80;  //enable uart interrupt
	}	
}


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/