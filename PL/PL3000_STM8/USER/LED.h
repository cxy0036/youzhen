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

/*
********************************************************************************
*                               PORT¡¡AND PIN
********************************************************************************
*/
#define LED_MULTI_PORT                  GPIOA
#define LED_MUTE_PORT                   GPIOA
#define LED_ADD_SW_PORT                 GPIOA
#define LED_LOCK_PORT                   GPIOC
#define LED_MULTI_PIN                   GPIO_PIN_4
#define LED_MUTE_PIN                    GPIO_PIN_3
#define LED_ADD_SW_PIN                  GPIO_PIN_6
#define LED_LOCK_PIN                    GPIO_PIN_4

#define LED_DEC_SW_PORT                 GPIOB
#define LED_LV8_PORT                    GPIOB
#define LED_AUTO_SW_PORT                GPIOB
#define LED_DEC_SW_PIN                  GPIO_PIN_7
#define LED_LV8_PIN                     GPIO_PIN_6
#define LED_AUTO_SW_PIN                 GPIO_PIN_3

#define LED_POWER_SW_PORT               GPIOC
#define LED_ERROR_PORT                  GPIOA
#define LED_POWER_SW_PIN                GPIO_PIN_7
#define LED_ERROR_PIN                   GPIO_PIN_5

#define LED_LV5_PORT                    GPIOD
#define LED_LV6_PORT                    GPIOD
#define LED_LV7_PORT                    GPIOD
#define LED_LV5_PIN                     GPIO_PIN_2
#define LED_LV6_PIN                     GPIO_PIN_7
#define LED_LV7_PIN                     GPIO_PIN_3

#define LED_AUTO_PORT                   GPIOE
#define LED_LV3_PORT                    GPIOE
#define LED_LV4_PORT                    GPIOE
#define LED_AUTO_PIN                    GPIO_PIN_5
#define LED_LV3_PIN                     GPIO_PIN_3
#define LED_LV4_PIN                     GPIO_PIN_0 

#define LED_LV2_PORT                    GPIOG
#define LED_LV1_PORT                    GPIOG
#define LED_LV2_PIN                     GPIO_PIN_1
#define LED_LV1_PIN                     GPIO_PIN_0

/*
********************************************************************************
*                              BEEP ON OFF FUNCTIONS
********************************************************************************
*/
#define BEEP_PORT                       GPIOD
#define BEEP_PIN                        GPIO_PIN_4

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
void LED_Level(uint8_t hv_level);
void LED_OFF_ALL(void);
void LED_Multi_Disply(void);
void LED_ALL_ON(void);
//void LED_Flash_1S(uint8_t lv);
//void LED_Flash_Times_Disply(uint8_t flash_times);
void Flash_Times_LED(void);


#endif /*__LED_H*/



/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/
