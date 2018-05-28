/**
  ******************************************************************************
  * @file    LED.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define BEEP and LED Pin, PORT and Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>
//#define LCD1602_4PINS
/*
********************************************************************************
*                               PORT¡¡AND PIN
********************************************************************************
*/
#define LED_lamp1_PORT                  GPIOA
#define LED_lamp2_PORT                  GPIOA
#define LED_lamp3_PORT                  GPIOA
#define LED_lamp4_PORT                  GPIOA
#define LED_Write_OK_PORT               GPIOD
#define LED_lamp1_PIN                   GPIO_PIN_6
#define LED_lamp2_PIN                   GPIO_PIN_5
#define LED_lamp3_PIN                   GPIO_PIN_4
#define LED_lamp4_PIN                   GPIO_PIN_3
#define LED_Write_OK_PIN                GPIO_PIN_6


#define LED_D0_PORT                  GPIOB
#define LED_D1_PORT                  GPIOB
#define LED_D2_PORT                  GPIOB
#define LED_D3_PORT                  GPIOB
#define LED_D4_PORT                  GPIOB
#define LED_D5_PORT                  GPIOB
#define LED_D6_PORT                  GPIOB
#define LED_D7_PORT                  GPIOB
#define LED_D0_PIN                   GPIO_PIN_0
#define LED_D1_PIN                   GPIO_PIN_1
#define LED_D2_PIN                   GPIO_PIN_2
#define LED_D3_PIN                   GPIO_PIN_3
#define LED_D4_PIN                   GPIO_PIN_4
#define LED_D5_PIN                   GPIO_PIN_5
#define LED_D6_PIN                   GPIO_PIN_6
#define LED_D7_PIN                   GPIO_PIN_7

#define LED_LCD1602_RS_PORT            GPIOE
#define LED_LCD1602_RW_PORT            GPIOG
#define LED_LCD1602_E_PORT             GPIOE
#define LCD1602_RS                     GPIO_PIN_5
#define LCD1602_RW                     GPIO_PIN_1
#define LCD1602_E                      GPIO_PIN_7

/*
********************************************************************************
*                              LED ON OFF FUNCTIONS
********************************************************************************
*/
#define LED_ON(p,i)                     {p->ODR |= i;}
#define LED_OFF(p,i)                    {p->ODR &= ~i;}
#define LED_TOG(p,i)                    {p->ODR ^= i;}

/*
********************************************************************************
*                              BEEP ON OFF FUNCTIONS
********************************************************************************
*/
#define BEEP_ON(p,i)                    {p->ODR |= i;}
#define BEEP_OFF(p,i)                   {p->ODR &= ~i;}
#define BEEP_TOG(p,i)                   {p->ODR ^= i;}
    
/*
********************************************************************************
*                                FUNCTIONS
********************************************************************************
*/
void LED_GPIO_Init(void);
void LED_fun(void);
void LcdWriteCom(uint8_t com);
void LcdWriteData(uint8_t dat);
void LcdInit();	
void Clear_fun(void);
#endif /*__LED_H*/



/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/
