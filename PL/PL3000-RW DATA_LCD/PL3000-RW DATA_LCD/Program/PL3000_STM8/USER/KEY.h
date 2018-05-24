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
#include "EEPROM.h"

#define ON                       1
#define OFF                      0

#define KEY_READ_PORT            GPIOD
#define KEY_WRITE_PORT           GPIOD
#define KEY_UP_PORT              GPIOD
#define KEY_DOWM_PORT            GPIOE
#define KEY_LAMP_COUNT_PORT      GPIOE
#define KEY_READ_PIN             GPIO_PIN_2
#define KEY_WRITE_PIN            GPIO_PIN_4
#define KEY_UP_PIN               GPIO_PIN_0
#define KEY_DOWM_PIN             GPIO_PIN_0
#define KEY_LAMP_COUNT_PIN       GPIO_PIN_3

#define ADC_PORT                    GPIOE   /* ADC IO PORT */
#define ADC_LAMP_PIN                GPIO_PIN_6  /* Main Fan Error Pin */
#define ADC_LAMP_CHANNEL            ADC1_CHANNEL_9  /* Main Fan Error Channel */

#define CARTRIDGE_ERROR_THR               204  /* cartridge open: 0v */         
#define CARTRIDGE_TYPE_4_H                409 /* One Lamp: 1v--2v */
#define CARTRIDGE_TYPE_4_L                205 
#define CARTRIDGE_TYPE_3_H                614 /* One Lamp: 2v--3v */
#define CARTRIDGE_TYPE_3_L                410
#define CARTRIDGE_TYPE_2_H                819 /* One Lamp: 3v--4v */
#define CARTRIDGE_TYPE_2_L                615
#define CARTRIDGE_TYPE_1                  820 /* two lamp: 4v--5V*/

void KEY_GPIO_Init(void);           
void Key_Fun(void);
void Key_Read_Fun(void);
void Key_Write_Fun(void);
uint16_t ADC_Read_Channel_Fun(uint8_t channel);
void LAMP_Type_Check(void);

#endif
