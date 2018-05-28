/**
  ******************************************************************************
  * @file    UART.h
  * @author  Captain.
  * @version V1.1.0
  * @date    2017-6-22
  * @brief   define UART Functions.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 YUJIN</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>
#include "stm8s_gpio.h"

/*
********************************************************************************
*                               UART  DEFINE
********************************************************************************
*/
#define USART_DEBUG 						UART2
#define USART_DEBUG_BAUTRATE  					38400
#define USART_BLUETOOTH_BAUTRATE  				9600  //bluetooth init bautrate
/*
********************************************************************************
*                               PROT DEFINE
********************************************************************************
*/
#define USART_DEBUG_TX_PORT   					GPIOD
#define USART_DEBUG_TX_PIN    					GPIO_PIN_5

#define USART_DEBUG_RX_PORT   					GPIOD
#define USART_DEBUG_RX_PIN    					GPIO_PIN_6

/*
********************************************************************************
*                               INTERRUPT DEFINE
********************************************************************************
*/
#define USART_DEBUG_IRQn					USART2_IRQn
#define USART_DEBUG_IRQnHandler					USART2_IRQHandler

#define UART_DE_PORT                                            GPIOC
#define UART_DE_PIN                                             GPIO_PIN_6


#define UART_DE_R                                               {UART_DE_PORT->ODR &= ~UART_DE_PIN;}
#define UART_DE_T                                               {UART_DE_PORT->ODR |= UART_DE_PIN;}                                             

/*
********************************************************************************
*                               FUCTIONS
********************************************************************************
*/
void DEBUG_USART_Config(void);
void USARTx_SendByte(uint8_t data);
void USARTx_SendString(char *str);
void USARTx_SendHalfWord(uint16_t data);
void USARTx_SendFrame(uint8_t data[],uint8_t data_length);

#endif /* __UART_H */


/************************ (C) COPYRIGHT YUJIN *****END OF FILE*****************/
