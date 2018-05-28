/**
  ******************************************************************************
  * @file    ADC.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define ADC Pin, PORT, Channel and Functions.
   *****************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AD_KEY_H
#define __AD_KEY_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>

/*
********************************************************************************
*                               ADC  Channel
********************************************************************************
*/
#define ADC_PORT                       GPIOB   /* ADC IO PORT */
#define ADC_TOUCH_SW_PIN               GPIO_PIN_4  /* Touch And Flash SW Pin */

#define ADC_TOUCH_SW_CHANNEL           ADC1_CHANNEL_4 /* Touch And Flash SW Channel */

#define TOUCH_SW_RELEASE               0x365     /* 4.25V-------> No touch, no SW */ 
#define TOUCH_PUSH_H                   0x299     /* 3.25V-------> Touch push */
#define TOUCH_PUSH_L                   0x1A3     /* 2.05V-------> Touch push */
#define FLASH_SW_PUSH                  0x066     /* 0.5V-------> Flash SW push */