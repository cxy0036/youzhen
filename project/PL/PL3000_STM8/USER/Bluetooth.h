#ifndef  __BLUETOOTH_H
#define  __BLUETOOTH_H

/* Includes ------------------------------------------------------------------*/
#include <stm8s.h>

/* AT Command ------------------------------------------------------------------*/
#define BLUE_TEST_CMD                   "AT"  /* Test cmd  */

/* AT Command ------------------------------------------------------------------*/
/* use: "AT+XXX\r\n" */
#define BLUE_RESET_CMD                  "RESET"   //model reset, 500ms reset later
#define BLUE_VERSION_CMD                "VERSION"  //get softwear version
#define BLUE_LADDR_CMD                  "LADDR"  //get model addr
#define BLUE_NAME_CMD                   "NAME"  //get model name: AT+NAME\r\n,retrun model 
//change model name: AT+NAME123\r\n, change the model name is 123
#define BLUE_PIN_CMD                    "PIN"  //set and get PIN

#define BLUE_BAUD_CMD                   "BAUD"  //set and get uart baud rate,1-->1200,2-->2400....9-->230400
#define BLUE_STOP_CMD                   "STOP" //set and get uart stop bit
#define BLUE_PARI_CMD                   "PARI" //set and get uart Parity bit

#define BLUE_DEFAULT_CMD                "DEFAULT"  //model reset to default model, 500ms reset later
#define BLUE_PWRM_CMD                   "PWRM"  //set and get model into low power model,0--lowpower,1--normal
#define BLUE_SLEEP_CMD                  "SLEEP"  //set model into sleep model.
#define BLUE_ROLE_CMD                   "ROLE" //set and get master or slave model,0--slave,1--master
#define BLUE_INQ_CMD                    "INQ" //search device
#define BLUE_SHOW_CMD                   "SHOW" //show search device
#define BLUE_CONN_CMD                   "CONN" //connect device
#define BLUE_POWE_CMD                   "POWE" //set model power 0-->-23db,1-->-6db,...3-->4db
#define BLUE_HELP_CMD                   "HELP" //help

#define BLUE_DATA_HEAD                  0xA5
#define BLUE_DATA_END                   0x5A

#define READ_DATA_FLASH_NUMBER          0x01 

void Bluetooth_Processe(void);
uint8_t Bluetooth_Command_Check(void);
void Bluetooth_Init(void);

#endif
