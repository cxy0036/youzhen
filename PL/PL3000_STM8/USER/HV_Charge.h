/**
  ******************************************************************************
  * @file    HV_Charge.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define Charge level,PID struct.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HV_CHARGE_H
#define __HV_CHARGE_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>

//#define FLASH_TRIG_PORT                GPIOD
//#define FLASH_TRIG_PIN                 GPIO_PIN_0


/*
********************************************************************************
*                               THRESHOLD
********************************************************************************
*/
#define PID_START_THR                  616  //0x0268//0x025C   /* 2.956v ----> HV: 537.3V*/

#define FLASH_STANDAR_OVER_THR         692  //0x00C0//0x0151   /* 0.90v ----> HV: 170V*/
//#define FLASH_MULTI_OVER_THR           680  //0x0268//0x02CE   /* 3.512v ----> HV: 650.4V*/         

//#define FULL_CHARGE_LEVEL1             0x0268   /* 3.011v ----> HV: 547.5V*/
//#define FULL_CHARGE_LEVEL2             0x028F   /* 3.802v ----> HV: 704.1V*/

//#define FULL_CHARGE_LEVEL1             0x02B6   /* 3.893v ----> HV: 720.9V*/
//#define FULL_CHARGE_LEVEL1             0x02C3
//#define FULL_CHARGE_LEVEL1             720  //0x02D0 599
//#define FULL_CHARGE_LEVEL2             744  //0x02E8 620
#define FULL_CHARGE_LEVEL1             715 //750
#define FULL_CHARGE_LEVEL2             715
#define FULL_CHARGE_LEVEL3             740//768  //0x0300 650
#define FULL_CHARGE_LEVEL4             770//792  //0x0318   /* 3.871v ----> HV: 737.8V*/
#define FULL_CHARGE_LEVEL5             795//816  //0x0330   /* 4.075v ----> HV: 754.6V*/ 
#define FULL_CHARGE_LEVEL6             825//840  //0x0348   /* 4.166v ----> HV: 771.5V*/ 
#define FULL_CHARGE_LEVEL7             850//864  //0x0360   /* 4.257v ----> HV: 788.3V*/ 
#define FULL_CHARGE_LEVEL8             878  //0x037A   /* 4.348v ----> HV: 805.2V*/  

/*
********************************************************************************
*                               TIMEOUT VALUE
********************************************************************************
*/
#define FLASH_TIME_OUT                 50000
#define CHARGE_TIME_OUT                45000
#define OPEN_IGBT_TIME_THR             1000                 

/*
********************************************************************************
*                                PID STRUCT
********************************************************************************
*/
typedef struct PID_struct
{
  int16_t SetValue;
  int8_t Kp;
  int8_t Ki;
  int8_t Kd;
  int16_t LastError;
 // int16_t PrevError;
  int16_t SumError;
}
PID;

/*
********************************************************************************
*                                FUNCTIONS
********************************************************************************
*/
void Charge_Control_Fun(uint8_t hv_level);
void Flash_Control(uint8_t plus_times);
int16_t PID_Caculate(PID *pid,int16_t NewValue);
void Timer1_Init(void);
void Timer_PID_Control(uint16_t count_data);
void PID_Control(PID *pid,int16_t newvalue);
void HV_PID_Init(void);
void Full_Charge_Judgment(uint16_t read_hv);
void Multi_Charge_Control(uint16_t charge_time, uint16_t delay_time);

#endif /* __HV_CHARGE_H */



/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/
