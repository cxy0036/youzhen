#ifndef __FLASH_CONTROL_H
#define __FLASH_CONTROL_H

#include "SN8F5702.h"

#define Trig_On							          {P07 = 1;}
#define Trig_Off							        {P07 = 0;}

#define Full_Charge_LED_On	          {P01 = 1;}
#define Full_Charge_LED_Off						{P01 = 0;}
#define Full_Charge_LED_TOG						{P01 ^= P01;}

#define Flash_Times_Level1            199999
#define Flash_Times_Level2            299999
#define Flash_Times_Level3            399999
#define Flash_Times_Level4            499999
#define Flash_Times_Level5            599999
#define Flash_Times_Level6            699999
#define Flash_Times_Level7            799999
#define Flash_Times_Level8            899999
#define Flash_Times_Level9            999999



//#define Flash_Times_Level_10					10
#define Flash_Times_Level_1					  1
#define Flash_Times_Level_2					  2
#define Flash_Times_Level_3					  3
#define Flash_Times_Level_4					  4
#define Flash_Times_Level_5					  5
#define Flash_Times_Level_6					  6
#define Flash_Times_Level_7					  7
#define Flash_Times_Level_8					  8
#define Flash_Times_Level_9					  8

void Flash_IO_Init(void);
void Flash_Control(uint8_t flash_times);
void Multi_Flash_Times(uint8_t plus,uint8_t level_rang,uint8_t touch_status);
//void Flash_Times_Count(void);
#endif
