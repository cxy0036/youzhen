/**
  ******************************************************************************
  * @file    error_chk.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define error check Functions.
   *****************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ERROR_CHK_H
#define __ERROR_CHK_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>

/*
********************************************************************************
*                           FAN ADC READ VALUE THRESHOLD
********************************************************************************
*/
#define MAIN_FAN_OPEN_ERROR_THR                 0x0002          /* LOW 0.03V */
#define MAIN_FAN_SHORT_ERROR_THR                0x0332          /* up 4.0V */

/*
********************************************************************************
*                                  LAMP TYPE
********************************************************************************
*/
#define ONE_LAMP                                1
#define TWO_LAMP                                2
#define THREE_LAMP                              3

/*
********************************************************************************
*                                 FUNCTIOS
********************************************************************************
*/
void ERROR_CHK_Fun(void);
void MainFan_Error_Chk_Fun(void);
//void Probe_Error_Chk_Fun(void);
void Lamp_Type_Check(void);
//void Skin_Color_Check(void);
//void Get_Mode_Type(void);
void Lamp_Type_Check2(void);
#endif /*__ERROR_CHK_H*/


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/