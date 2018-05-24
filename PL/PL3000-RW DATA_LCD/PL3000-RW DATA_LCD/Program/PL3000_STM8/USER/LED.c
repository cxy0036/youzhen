/**
  ******************************************************************************
  * @file    LED.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2018-3-22
  * @brief   define LED Control Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LED.h"
#include "KEY_FTC332.h"
#include "GPIO_Config.h"
#include "delay.h"
#include "flash_times.h"
#include "EEPROM.h"

/* Global variable -----------------------------------------------------------*/
uint8_t WeiValue;
uint8_t Weinum;
uint8_t WEI_Count;
uint8_t Duan_num = 0;
uint8_t Lamp_Type = 1; /*  */
uint32_t Play_count;
uint8_t Write_OK_flag = 0;
extern uint8_t Wkey_read ;
extern uint8_t key_read;
extern volatile uint32_t Flash_Times;
extern uint32_t flash_count;
/* Public functions ----------------------------------------------------------*/

/*
********************************************************************************
*                           LED IO INIT FUNCTIONS
*
* Description: This function init LED Port and Pin.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void LED_GPIO_Init(void)
{
  GPIOA->ODR &= ~(LED_lamp1_PIN + LED_lamp2_PIN  + LED_lamp3_PIN+ LED_lamp4_PIN );  /* OUT 0 ,LED OFF */
  GPIOA->CR2 &= ~(LED_lamp1_PIN + LED_lamp2_PIN  + LED_lamp3_PIN+ LED_lamp4_PIN  );  /* Low Speed 2MHz */
  GPIOA->CR1 |= LED_lamp1_PIN + LED_lamp2_PIN  + LED_lamp3_PIN+ LED_lamp4_PIN ;  /* Push-pull */
  GPIOA->DDR |= LED_lamp1_PIN + LED_lamp2_PIN  + LED_lamp3_PIN+ LED_lamp4_PIN ;  /* Output */
  
  GPIOB->ODR &= ~(LED_D0_PIN + LED_D1_PIN + LED_D2_PIN + LED_D3_PIN + LED_D4_PIN + LED_D5_PIN + LED_D6_PIN + LED_D7_PIN);
  GPIOB->CR2 &= ~(LED_D0_PIN + LED_D1_PIN + LED_D2_PIN + LED_D3_PIN + LED_D4_PIN + LED_D5_PIN + LED_D6_PIN + LED_D7_PIN);
  GPIOB->CR1 |= LED_D0_PIN + LED_D1_PIN + LED_D2_PIN + LED_D3_PIN + LED_D4_PIN + LED_D5_PIN + LED_D6_PIN + LED_D7_PIN;
  GPIOB->DDR |= LED_D0_PIN + LED_D1_PIN + LED_D2_PIN + LED_D3_PIN + LED_D4_PIN + LED_D5_PIN + LED_D6_PIN + LED_D7_PIN;
  
  GPIOD->ODR &= ~(LED_Write_OK_PIN);
  GPIOD->CR2 &= ~(LED_Write_OK_PIN);
  GPIOD->CR1 |= LED_Write_OK_PIN;
  GPIOD->DDR |= LED_Write_OK_PIN;
     
  //GPIOE->ODR |= LCD1602_RS + LCD1602_RW + LCD1602_E;
  //GPIOE->CR2 &= ~(LCD1602_RS + LCD1602_RW + LCD1602_E);
  //GPIOE->CR1 |= LCD1602_RS + LCD1602_RW + LCD1602_E;
  //GPIOE->DDR |= LCD1602_RS + LCD1602_RW + LCD1602_E;
  
  GPIOG->ODR &= ~LCD1602_RW;
  GPIOG->CR2 &= ~LCD1602_RW;
  GPIOG->CR1 |= LCD1602_RW;
  GPIOG->DDR |= LCD1602_RW;
  
  GPIOE->ODR |= LCD1602_RS  + LCD1602_E;
  GPIOE->CR2 &= ~(LCD1602_RS + LCD1602_E);
  GPIOE->CR1 |= LCD1602_RS + LCD1602_E;
  GPIOE->DDR |= LCD1602_RS + LCD1602_E;
}

/*
********************************************************************************
*                           LED OFF﹛ALL FUNCTIONS
*
* Description: This function OFF ALL﹛LED.
*
* Arguments  : none.
*
* Note(s)    : none.
********************************************************************************
*/
void LED_fun(void)
{
  WeiValue = 0x4F;
  if(Write_OK_flag)
  {  
    LED_ON(LED_Write_OK_PORT,LED_Write_OK_PIN); 
    Delay_ms(50);
    Write_OK_flag = 0; 
  }else{
    LED_OFF(LED_Write_OK_PORT,LED_Write_OK_PIN); 
  } 
  switch(Lamp_Type){
            case 1 : LED_ON(LED_lamp1_PORT,LED_lamp1_PIN); 
                     LED_OFF(LED_lamp2_PORT,LED_lamp2_PIN);
                     LED_OFF(LED_lamp3_PORT,LED_lamp3_PIN);
                     LED_OFF(LED_lamp4_PORT,LED_lamp4_PIN); 
                     break;
            case 2 : LED_ON(LED_lamp1_PORT,LED_lamp1_PIN); 
                     LED_ON(LED_lamp2_PORT,LED_lamp2_PIN);
                     LED_OFF(LED_lamp3_PORT,LED_lamp3_PIN);
                     LED_OFF(LED_lamp4_PORT,LED_lamp4_PIN); 
                     break;
            case 3 : LED_ON(LED_lamp1_PORT,LED_lamp1_PIN); 
                     LED_ON(LED_lamp2_PORT,LED_lamp2_PIN);
                     LED_ON(LED_lamp3_PORT,LED_lamp3_PIN);
                     LED_OFF(LED_lamp4_PORT,LED_lamp4_PIN); 
                     break;
            case 4 : LED_ON(LED_lamp1_PORT,LED_lamp1_PIN); 
                     LED_ON(LED_lamp2_PORT,LED_lamp2_PIN);
                     LED_ON(LED_lamp3_PORT,LED_lamp3_PIN);
                     LED_ON(LED_lamp4_PORT,LED_lamp4_PIN); 
                     break;
            default : break;
            }
   if(key_read == 1)
  {
    //Clear_fun();
    LcdWriteCom(0x01);  //???芍
    key_read = 0;
  }
  if(Flash_Times == 0)
    {
      LcdWriteCom(0x80+0x4F);
      LcdWriteData('0'); 
    }else{
      WeiValue = 0x4F;
      Play_count = Flash_Times;
      while(Play_count)
      { 
        Weinum = Play_count%10;
        Play_count = Play_count/10;
        LcdWriteCom(0x80+WeiValue);
        WeiValue--;
        LcdWriteData('0'+Weinum);  
      }
    }
  LcdWriteCom(0x80+0x40);
  LcdWriteData('0'+Lamp_Type);
  
}



void Clear_fun(void)
{
  WeiValue = 0x4F;
  while(WeiValue != 0x40)
  {
    LcdWriteCom(0x80+WeiValue);
    WeiValue--;
    LcdWriteData('0'); 
  }
}


/*******************************************************************************
* o‘ 那y ??         : LcdWriteCom
* o‘那y1|?邦		   : ?辰LCDD∩豕?辰???℅??迆米??邦芍?
* 那?    豕?         : com
* 那?    3?         : ?T
*******************************************************************************/
//#ifndef 	LCD1602_4PINS	 //米㊣??車D?“辰??a??LCD1602_4PINS那㊣
void LcdWriteCom(uint8_t com)	  //D∩豕??邦芍?
{
	LED_OFF(LED_LCD1602_E_PORT,LCD1602_E);   
	LED_OFF(LED_LCD1602_RS_PORT,LCD1602_RS);
	LED_OFF(LED_LCD1602_RW_PORT,LCD1602_RW);
	
        GPIO_Write(GPIOB, com);
	//LCD1602_DATAPINS = com;     //﹞?豕??邦芍?
	Delay_ms(1);		//米豕∩y那y?Y?豕?“

	LED_ON(LED_LCD1602_E_PORT,LCD1602_E);	          //D∩豕?那㊣D辰
	Delay_ms(5);	  //㊣㏒3?那㊣??
	LED_OFF(LED_LCD1602_E_PORT,LCD1602_E);
}

//#else 
/*
void LcdWriteCom(uint8_t com)	  //D∩豕??邦芍?
{
	LED_OFF(LED_LCD1602_E_PORT,GPIO_PIN_7);   
	LED_OFF(LED_LCD1602_RS_PORT,GPIO_PIN_5);
	LED_OFF(LED_LCD1602_RW_PORT,GPIO_PIN_6);

	//LCD1602_DATAPINS = com;	//車谷車迆
        GPIO_Write(GPIOB, com);
	Delay_ms(1);

	LED_ON(LED_LCD1602_E_PORT,GPIO_PIN_7);	 //D∩豕?那㊣D辰
	Delay_ms(5);
	LED_OFF(LED_LCD1602_E_PORT,GPIO_PIN_7);


	//LCD1602_DATAPINS = com << 4; //﹞⊿?赤米赤????
        GPIO_Write(GPIOB, com);
	Delay_ms(1);

	LED_ON(LED_LCD1602_E_PORT,LCD1602_E);	 //D∩豕?那㊣D辰
	Delay_ms(5);
	LED_OFF(LED_LCD1602_E_PORT,LCD1602_E);
}*/
//#endif

/*******************************************************************************
* o‘ 那y ??         : LcdWriteData
* o‘那y1|?邦		   : ?辰LCDD∩豕?辰???℅??迆米?那y?Y
* 那?    豕?         : dat
* 那?    3?         : ?T
*******************************************************************************/		   
//#ifndef 	LCD1602_4PINS		   
void LcdWriteData(uint8_t dat)			//D∩豕?那y?Y
{
	LED_OFF(LED_LCD1602_E_PORT,LCD1602_E);   
	LED_ON(LED_LCD1602_RS_PORT,LCD1602_RS);
	LED_OFF(LED_LCD1602_RW_PORT,LCD1602_RW);  

	//LCD1602_DATAPINS = dat; //D∩豕?那y?Y
        GPIO_Write(GPIOB, dat);
	Delay_ms(1);

	LED_ON(LED_LCD1602_E_PORT,LCD1602_E); //D∩豕?那㊣D辰
	Delay_ms(5);
	LED_OFF(LED_LCD1602_E_PORT,LCD1602_E);
}
/*
#else
void LcdWriteData(uint8_t dat)			//D∩豕?那y?Y
{
	LED_OFF(LED_LCD1602_E_PORT,LCD1602_E);   
	LED_ON(LED_LCD1602_RS_PORT,GPIO_PIN_5);
	LED_OFF(LED_LCD1602_RW_PORT,GPIO_PIN_6);

	//LCD1602_DATAPINS = dat;	//車谷車迆4??米??車??那??車米?P0?迆米???????㏒??迄辰?∩??赤??????2?車???
        GPIO_Write(GPIOB, dat);
	Delay_ms(1);

	LED_ON(LED_LCD1602_E_PORT,LCD1602_E);	 //D∩豕?那㊣D辰
	Delay_ms(5);
	LED_OFF(LED_LCD1602_E_PORT,LCD1602_E);

	//LCD1602_DATAPINS = dat << 4; //D∩豕?米赤????
        GPIO_Write(GPIOB, dat);
	Delay_ms(1);

	LED_ON(LED_LCD1602_E_PORT,LCD1602_E);	 //D∩豕?那㊣D辰
	Delay_ms(5);
	LED_OFF(LED_LCD1602_E_PORT,LCD1602_E);
}*/
//#endif

/*******************************************************************************
* o‘ 那y ??       : LcdInit()
* o‘那y1|?邦		 : 3?那??‘LCD?芍
* 那?    豕?       : ?T
* 那?    3?       : ?T
*******************************************************************************/		   
//#ifndef		LCD1602_4PINS
void LcdInit()						  //LCD3?那??‘℅車3足D辰
{
 	LcdWriteCom(0x38);  //?a??那?
	LcdWriteCom(0x0c);  //?a??那?2???那?1a㊣那
	LcdWriteCom(0x06);  //D∩辰????????車1
	LcdWriteCom(0x01);  //???芍
	LcdWriteCom(0x80);  //谷豕??那y?Y?????e米?
        
}
/*
#else
void LcdInit()						  //LCD3?那??‘℅車3足D辰
{
         //LED_ON(LED_lamp4_PORT,LED_lamp4_PIN);
	LcdWriteCom(0x32);	 //??8??℅邦??℅a?a4??℅邦??
	LcdWriteCom(0x28);	 //?迆????????米?3?那??‘
	LcdWriteCom(0x0c);  //?a??那?2???那?1a㊣那
	LcdWriteCom(0x06);  //D∩辰????????車1
	LcdWriteCom(0x01);  //???芍
	LcdWriteCom(0x80);  //谷豕??那y?Y?????e米?
       
}*/
//#endif
