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
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>

/*
********************************************************************************
*                               ADC  Channel
********************************************************************************
*/
#define ADC_PORT                       GPIOB   /* ADC IO PORT */
#define ADC_MAN_FAN_PIN                GPIO_PIN_2  /* Main Fan Error Pin */

#define ADC_HV_IN_PORT                 GPIOE  /* Hight Voltage Sample ADC IO PORT */
#define ADC_HV_IN_PIN                  GPIO_PIN_6  /* Hight Voltage Sample Pin */

#define ADC_MAIN_FAN_CHANNEL           ADC1_CHANNEL_2  /* Main Fan Error Channel */
#define ADC_HV_IN_CHANNEL              ADC1_CHANNEL_9  /* Hight Voltage Sample Channel */

/*
********************************************************************************
*                               ADC  FUNCTIONS
********************************************************************************
*/
void ADC_Init_Fun(void);
uint16_t ADC_Read_Channel_Fun(uint8_t channel);
void AD(void);

#endif /* __ADC_H */


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/
