#ifndef __PROTOCOL_H
#define __PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>

#define BUS_HEADER               0xA5
#define BUS_END			 0x5A

/* Master Request -------------------------------------------------------------*/
#define FUN_No1		         0x01  /* get flash times */
#define FUN_No2			 0x02  /* get flash times level */
#define FUN_No3			 0x03  /* get skin color level */
#define FUN_No4			 0x04  /* get Flash Flag */
#define FUN_No5			 0x05  /* get Flash OK Flag */
#define FUN_No6			 0x06  /* get ERROR flag */

/* Master Send Data -----------------------------------------------------------*/
#define FUN_No7			 0x07  /* Full Charge flag */
#define FUN_No8			 0x08  /* Mode Type */
#define FUN_No9			 0x09  /* Flash Ready OK Flag */
#define FUN_No10		 0x0A  /* Flash Level */
#define FUN_No11                 0x0B  /* get Lamp Type */     
#define FUN_No12                 0x0C  /* get Mode Type */
#define FUN_No13                 0x0D  /* Lock Flag */
#define FUN_No14                 0x0E  /* skin type */
#define FUN_No15                 0x0F  /* All */
#define FUN_No16                 0x10  

//#define CRC_8			 0x31  /* CRC_8 */
#define TIME_OUT                 50000

void Send_Request(uint8_t fun_num, uint8_t *Data,uint8_t length);
uint8_t Crc_Caculate(uint8_t *d,uint8_t length);
//uint8_t Receive_Wait(uint16_t wait_times);
uint8_t  Receive_Wait_Lost_First(uint16_t wait_times);
//uint8_t Receive_Data_Check(uint8_t *d,uint8_t length);
void Communication_Fun(void);

#endif
