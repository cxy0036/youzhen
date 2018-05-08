/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "LED.h"
#include "GPIO_Config.h"
#include "delay.h"
#include "KEY_FTC332.h"
#include "UART.h"
#include "stdio.h"
//#include "clock_config.h"
#include "ADC.h"
#include "EEPROM.h"
//#include "error_chk.h"
#include "HV_Charge.h"
#include "Bluetooth.h"
#include "protocol.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern uint8_t EEPROM_ERROR_Flag;
//extern uint32_t Flash_Count;
extern uint8_t Receive_Buffer_Full_Flag;

void main(void)
{
  //uint16_t read_hv;
  Init_Fun();
  //HV_Control_IO_Init_Fun();
  while(1)
  {
    Key_Read();
    Key_Process_Fun(); 
    Communication_Fun();
    Delay_ms(5);
    //IWDG->KR = 0xAA;  
    Function_Processe();   
    Bluetooth_Processe();
    IWDG->KR = 0xAA;    
    IWDG->KR = 0xAA;    
  } 
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
