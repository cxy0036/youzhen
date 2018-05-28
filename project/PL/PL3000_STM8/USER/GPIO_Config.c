/**
  ******************************************************************************
  * @file    GPIO_Config.c
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define HV and Flash Pin, PORT and Functions,all init function.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "GPIO_Config.h"  
#include "delay.h"
#include "clock_config.h"
#include "HV_Charge.h"
#include "stm8s_gpio.h"
#include "LED.h"
#include "UART.h"
#include "KEY_FTC332.h"
#include "ADC.h"
#include "Bluetooth.h"
uint8_t Power_LED_Start_Flag = 1;
/* Public functions ----------------------------------------------------------*/

/*
********************************************************************************
*                           HV CONTROL IO INIT FUNCTIONS
*
* Description: This function Initial hv control IO PORT.
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/
void HV_Control_IO_Init_Fun(void)
{
  HV_CONTROL_PORT->ODR |= HV_CONTROL_PIN;  /* HV contorl pin high out, low speed */
  HV_CONTROL_PORT->DDR |= HV_CONTROL_PIN;
  HV_CONTROL_PORT->CR1 |= HV_CONTROL_PIN;
  HV_CONTROL_PORT->CR2 &= ~HV_CONTROL_PIN;
  
  MULTI_PLUS_CTR_PORT->ODR |= MULTI_PLUS_CTR_PIN;  /* multi congtrol pin high out */
  MULTI_PLUS_CTR_PORT->DDR |= MULTI_PLUS_CTR_PIN;
  MULTI_PLUS_CTR_PORT->CR1 |= MULTI_PLUS_CTR_PIN;
  MULTI_PLUS_CTR_PORT->CR2 |= MULTI_PLUS_CTR_PIN;  /* Fast Speed */
  
  PROBE_POWER_PORT->ODR &= ~PROBE_POWER_PIN; /* probe and full charge pin low out */
  PROBE_POWER_PORT->DDR |= PROBE_POWER_PIN;
  PROBE_POWER_PORT->CR1 |= PROBE_POWER_PIN;
  PROBE_POWER_PORT->CR2 &= ~PROBE_POWER_PIN;
  
  TRIG_PORT->ODR &= ~TRIG_PIN;  /* trig congtrol pin low */
  TRIG_PORT->DDR |= TRIG_PIN;
  TRIG_PORT->CR1 |= TRIG_PIN;
  TRIG_PORT->CR2 |= TRIG_PIN;  /* Fast Speed */

}

/*
********************************************************************************
*                           IWDG INIT FUNCTIONS
*
* Description: This function Initial IWDG.
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/
void IWDG_Init(void)
{
  IWDG->KR = 0xCC; //����IWDG
  IWDG->KR = 0x55; //��� PR �� RLR ��д����
  IWDG->RLR = 0xff; //���Ź���������װ����ֵ       
  IWDG->PR = 0x06; //��Ƶϵ��Ϊ256�����ʱʱ��Ϊ��1.02S
  IWDG->KR = 0xAA; //ˢ��IDDG������������Ź���λ��ͬʱ�ָ� PR �� RLR ��д����״̬
}

/*
********************************************************************************
*                           ALL INIT FUNCTIONS
*
* Description: This function Initial All function.
*
* Arguments  : none
*
* Note(s)    : none.
********************************************************************************
*/
void Init_Fun(void)
{
  Clock_Config();  /* clock initial, Fmarst = 16Mhz */
  HV_Control_IO_Init_Fun();  /* Hv control IO initial */
  LED_GPIO_Init();
  Delay_ms(600);
  //Beep_Init();
  BEEP->CSR=0x09; //16��Ƶ��128Khz/16=8Khz
  //BEEP->CSR|=0x00; //���Ƶ��8Khz/8=1Khz
  BEEP->CSR|=0x20; //ʹ�ܷ�����   
  DEBUG_USART_Config(); 
  Delay_ms(500);
  BEEP_Cmd(DISABLE);//BEEP_OFF(BEEP_PORT,BEEP_PIN);
  Bluetooth_Init(); 
  HV_PID_Init(); 
  Key_Init_Fun();
  ADC_Init_Fun();
  GPIO_Init(BEEP_PORT,BEEP_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
  //BEEP_Cmd(ENABLE);//BEEP_ON(BEEP_PORT,BEEP_PIN);
  if(Power_LED_Start_Flag)
    {
     LED_ON(LED_POWER_SW_PORT,LED_POWER_SW_PIN);      /* power LED ON*/
     //Power_LED_Start_Flag = 0 ;
    }
  IWDG_Init();
}

/*
void Beep_Init(void)
{
        BEEP->CSR=0x09; //16��Ƶ��128Khz/16=8Khz
        BEEP->CSR|=0x00; //���Ƶ��8Khz/8=1Khz
        BEEP->CSR|=0x20; //ʹ�ܷ�����
       // Delay_ms(500);
        //BEEP->CSR&=~0x20; //�ط�����
}
*/

/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/