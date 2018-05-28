/**
  ******************************************************************************
  * @file    LED.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define LED Pin, PORT and Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __KEY_H
#define __KEY_H

#include <stm8s.h>

#define ON                       1
#define OFF                      0

#define KEY_PORT                 GPIOB

#define KEY_LEVEL_DOWN_PIN       GPIO_PIN_7
#define KEY_AUTO_PIN             GPIO_PIN_6
#define KEY_LEVEL_UP_PIN         GPIO_PIN_5
#define KEY_POWER_PIN            GPIO_PIN_4

#define KEY_UNLOCK               0xA0       /* bit7 and bit5 */
           

void Key_Read_Fun(void);

#endif
