/**
  ******************************************************************************
  * @file    LED.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2018-3-22
  * @brief   define LED Pin, PORT and Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#include "KEY.h"
#include "LED.h"
#include "EEPROM.h"
#include "delay.h"

uint32_t flash_count = 0;
uint8_t key_read;
extern uint8_t Duan_num;
extern uint8_t Lamp_Type; /*  */
extern uint8_t Write_OK_flag;
volatile uint32_t Flash_Times=0;
uint8_t lamp_level = 0; /*  */
uint32_t Flash_Times1;
uint32_t Flash_Times2;
uint32_t Flash_Times3;
uint8_t key_read ;
uint8_t LAMP_low_flag = 0;
uint8_t Up_low_flag = 0;
uint8_t Dowm_low_flag = 0;
uint8_t Read_low_flag = 0;
uint8_t Write_low_flag = 0;
uint8_t Cartridge_Error_Flag = 0;
extern uint32_t Play_count;
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Read Key Function.
  * @note   
  *         
  *         
  * @param  None 
  * @retval None
  */

void KEY_GPIO_Init(void)
{
  
  //GPIOE->CR2 &= ~(KEY_READ_PIN + KEY_DOWM_PIN + KEY_WRITE_PIN + KEY_LAMP_COUNT_PIN);
  //GPIOE->CR1 |= KEY_READ_PIN + KEY_DOWM_PIN + KEY_WRITE_PIN + KEY_LAMP_COUNT_PIN;
  //GPIOE->DDR &= ~(KEY_READ_PIN + KEY_DOWM_PIN + KEY_WRITE_PIN + KEY_LAMP_COUNT_PIN);//PE5 input
  GPIOE->CR2 &= ~(KEY_DOWM_PIN + KEY_LAMP_COUNT_PIN);
  GPIOE->CR1 |= KEY_DOWM_PIN + KEY_LAMP_COUNT_PIN;
  GPIOE->DDR &= ~(KEY_DOWM_PIN +KEY_LAMP_COUNT_PIN);//PE5 input
  
  GPIOD->CR1 |=  KEY_UP_PIN + KEY_READ_PIN +KEY_WRITE_PIN;
  GPIOD->CR2 &= ~ (KEY_UP_PIN +  KEY_READ_PIN +KEY_WRITE_PIN);
  GPIOD->DDR &= ~ (KEY_UP_PIN +  KEY_READ_PIN +KEY_WRITE_PIN);//PD0 input
  
   ADC_PORT->ODR |= ADC_LAMP_PIN;  /* Out Hight */
   ADC_PORT->CR1 &= ~ADC_LAMP_PIN; /* Floating Input */
   ADC_PORT->CR2 &= ~ADC_LAMP_PIN; /* Disable External Interrupt */
   ADC_PORT->DDR &= ~ADC_LAMP_PIN; /* Input Model */
   
   ADC1->CR1 = 0x02;     /* Fadc = Fmaster/2, Continue Conversion Mode, Disable ADC Conversion */
   ADC1->CR2 = 0x00;     /* Disable External Event Eonversion, Left Alignment, Disable Scan Mode */
   ADC1->CR3 = 0x00;     /* Disable Data Buffer */
   ADC1->CSR = 0x01;     /* Disable ALL ADC Interrupt, Select Channel AIN3(HV_IN) */
   ADC1->TDRH |= 0x02;    /* Disable Schmitt Trigger AN9*/
   ADC1->TDRL |= 0x01;    /* Disable Schmitt Trigger AN2*/
}

void Key_Fun(void)//Scan key
{
// Read key scan 
    if(GPIO_ReadInputPin(KEY_READ_PORT, KEY_READ_PIN))//firth time eff
      {
        Read_low_flag = 1;
      }
    if((GPIO_ReadInputPin(KEY_READ_PORT, KEY_READ_PIN) == 0)&&(Read_low_flag == 1))
     {
       Delay_ms(2);
       if((GPIO_ReadInputPin(KEY_READ_PORT, KEY_READ_PIN) == 0)&&(Read_low_flag == 1))
       {
         Read_low_flag = 0;
         key_read = 1;
         Key_Read_Fun();
         //LED_ON(LED_lamp4_PORT,LED_lamp4_PIN); 
         Delay_ms(50);
       }
     }
    
    
// Write key scan   
    if(GPIO_ReadInputPin(KEY_WRITE_PORT, KEY_WRITE_PIN))//firth time eff
      {
        Write_low_flag = 1;
      }
     if((GPIO_ReadInputPin(KEY_WRITE_PORT, KEY_WRITE_PIN) == 0)&&(Write_low_flag == 1))
      {
       Delay_ms(2);
       if((GPIO_ReadInputPin(KEY_WRITE_PORT, KEY_WRITE_PIN) == 0)&&(Write_low_flag == 1))
       {
          Write_low_flag = 0; 
          lamp_level = 0;
          key_read = 1;
          //Write_Num_TO_EEPROM(0);
          Key_Write_Fun();
         //LED_ON(LED_lamp4_PORT,LED_lamp4_PIN); 
         Delay_ms(50);
          }
        }
//  up key scan  
    if(GPIO_ReadInputPin(KEY_UP_PORT, KEY_UP_PIN))//firth time eff
      {
        Up_low_flag = 1;
      }
    if((GPIO_ReadInputPin(KEY_UP_PORT, KEY_UP_PIN) == 0)&&(Up_low_flag == 1))
       {
         Delay_ms(2);
         if((GPIO_ReadInputPin(KEY_UP_PORT, KEY_UP_PIN) == 0)&&(Up_low_flag == 1))
           {  
             Up_low_flag = 0;
             lamp_level++;
             key_read = 1;
             if(lamp_level >= 10)
              {
                lamp_level = 10;
              }
            switch(lamp_level){
            case 0 : flash_count = 0 ;break;
            case 1 : flash_count = 99990  ;break;
            case 2 : flash_count = 199990 ;break;
            case 3 : flash_count = 299990  ;break;
            case 4 : flash_count = 399990  ;break;
            case 5 : flash_count = 499990  ;break;
            case 6 : flash_count = 599990  ;break;
            case 7 : flash_count = 699990  ;break;
            case 8 : flash_count = 799990  ;break;
            case 9 : flash_count = 899990  ;break;
            case 10 : flash_count = 999990 ;break;
            //case 11 : flash_count = 999990  ;break;
            default : break;
            }
            Flash_Times = flash_count;
             //Key_Write_Fun();
             //Key_Read_Fun();
             Delay_ms(100);
           }
       }
//  dowm key scan   
    if(GPIO_ReadInputPin(KEY_DOWM_PORT, KEY_DOWM_PIN))//firth time eff
      {
        Dowm_low_flag = 1;
      }
    if((GPIO_ReadInputPin(KEY_DOWM_PORT, KEY_DOWM_PIN) == 0)&&(Dowm_low_flag == 1))
       {
         Delay_ms(2);
         if((GPIO_ReadInputPin(KEY_DOWM_PORT, KEY_DOWM_PIN) == 0)&&(Dowm_low_flag == 1))
           {
             Dowm_low_flag = 0;
             key_read = 1; 
             //Key_Write_Fun();
             //Key_Read_Fun();
              if(lamp_level <= 0)
               {
                 lamp_level = 1;
               }
               lamp_level--;
            switch(lamp_level){
            case 0 : flash_count = 0 ;break;
            case 1 : flash_count = 99990  ;break;
            case 2 : flash_count = 199990 ;break;
            case 3 : flash_count = 299990  ;break;
            case 4 : flash_count = 399990  ;break;
            case 5 : flash_count = 499990  ;break;
            case 6 : flash_count = 599990  ;break;
            case 7 : flash_count = 699990  ;break;
            case 8 : flash_count = 799990  ;break;
            case 9 : flash_count = 899990  ;break;
            case 10 : flash_count = 999990 ;break;
            //case 11 : flash_count = 999990  ;break;
            default : break;
            }
            Flash_Times = flash_count;
             Delay_ms(100);
           }
       }
// lamp_count key scan          
     if(GPIO_ReadInputPin(KEY_LAMP_COUNT_PORT, KEY_LAMP_COUNT_PIN))//firth time eff
      {
        LAMP_low_flag = 1;
      }
    if((GPIO_ReadInputPin(KEY_LAMP_COUNT_PORT, KEY_LAMP_COUNT_PIN) == 0)&&(LAMP_low_flag == 1))//Lamp Type choice  
     {  
       Delay_ms(2);
       if((GPIO_ReadInputPin(KEY_LAMP_COUNT_PORT, KEY_LAMP_COUNT_PIN) == 0)&&(LAMP_low_flag == 1))//Lamp Type choice 
       {
         LAMP_low_flag = 0;
         Lamp_Type ++;
         Delay_ms(50);
         if(Lamp_Type==5)
         {
           Lamp_Type = 1;
         }
       }
     }  
}

void Key_Read_Fun(void)
{
   Flash_Times1 = Read_Num_TO_EEPROM(); 
   Delay_ms(5);
   Flash_Times2 = Read_Num_TO_EEPROM();
   Delay_ms(5);
   Flash_Times3 = Read_Num_TO_EEPROM();
   if(Flash_Times1==Flash_Times2)
    { 
      Flash_Times = Flash_Times1;
    }else{
	  if(Flash_Times3==Flash_Times2)
	   {
	     Flash_Times = Flash_Times2;
	   }
         }
   
}

void Key_Write_Fun(void)
{
   uint8_t Check_Write_EEPROM_Timeout = 0;  
   Flash_Times = flash_count;
   Write_Num_TO_EEPROM(Flash_Times);
   Delay_ms(20);
   //Flash_Times = Read_Num_TO_EEPROM();  
   while(Flash_Times != Read_Num_TO_EEPROM())
	{
	  Check_Write_EEPROM_Timeout++;
	  Write_Num_TO_EEPROM(flash_count);
	  Delay_ms(5);
	  if(Check_Write_EEPROM_Timeout == 8)
	   {
	     Check_Write_EEPROM_Timeout = 0;
	     return;
	   }
	}
   Write_OK_flag = 1;
}

/*
********************************************************************************
*                                       READ ONE ADC CHANNEL VALUE FUNCTIONS
*
* Description: This function read one ADC channel value,return the 16 bit converter value.
*
* Arguments  : wangt convert ADC channel
*
* Return  : Return the convert value of 16 Bit for the channel
*
* Note(s)    : none.
********************************************************************************
*/
uint16_t ADC_Read_Channel_Fun(uint8_t channel)
{
  uint16_t readdata,read_arry[6];  
  uint8_t i,j;
  ADC1->CSR  = channel;    /* Clean Flag, Select Channel  */
  ADC1->CR1 |= 0x01;    /* Start Conversion */
  Delay_us(10);   /* Must Need Delay 7us More Than */
  ADC1->CR1 |= 0x01;    /* Start Conversion */
  for(i=0;i<5;i++)
  {   
    while(!(ADC1->CSR & 0x80));   /* Wait Conversion Complete ,Convert 5 Times*/
    read_arry[i+1] = (uint16_t)ADC1->DRH;     /* Read Data Convertsion to 16 Bit*/
    read_arry[i+1] = (read_arry[i+1] << 2) | ADC1->DRL;
    ADC1->CSR &= ~0x80;  /* Clear Complete Flag */
  }
  ADC1->CR1 &= ~0x01;   /* Stop Conversion */
  
/* Filter Funtion**************************************************************/
  for(i=1;i<6;i++)  /* Insert sort */
  {
    read_arry[0] = read_arry[i];
    for(j=i-1; read_arry[0]<read_arry[j]; j--)
    {
        read_arry[j+1] = read_arry[j];
    }
    read_arry[j+1] = read_arry[0];
  }
  readdata = read_arry[3]; /*  Midle Data to Retrun */
/******************************************************************************/
  return readdata;
}



void LAMP_Type_Check(void)
{
  uint16_t readdata;
  readdata = ADC_Read_Channel_Fun(ADC_LAMP_CHANNEL);
	if(readdata > CARTRIDGE_TYPE_1)
	{
	  Lamp_Type = 1;
	}else{
		if((readdata < CARTRIDGE_TYPE_2_H) && (readdata > CARTRIDGE_TYPE_2_L))
		{
			Lamp_Type = 2;
		}
		if((readdata < CARTRIDGE_TYPE_3_H) && (readdata > CARTRIDGE_TYPE_3_L))
		{
			Lamp_Type = 3;
		}
		if((readdata < CARTRIDGE_TYPE_4_H) && (readdata > CARTRIDGE_TYPE_4_L))
		{
			Lamp_Type = 4;
		}
		if(readdata < CARTRIDGE_ERROR_THR)
	         {
                    Delay_us(50);
                    readdata = ADC_Read_Channel_Fun(ADC_LAMP_CHANNEL);
                    if(readdata < CARTRIDGE_ERROR_THR)
                      {
                        Cartridge_Error_Flag = 1;
                      }
                 }else{
                          Cartridge_Error_Flag = 0;
                  }
            }
        
}