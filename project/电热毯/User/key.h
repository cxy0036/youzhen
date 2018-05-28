#ifndef __KEY_H
#define __KEY_H

#include "SN8F5702.H"

#define TOUCH_PIN										0x02  /* Pin P10 <<1  ---> 0x02 */
#define FLASH_SW_PIN								0x01  /* Pin P00 */

#define Standard_Mode               0x01
#define Auto_Mode                   0x02
#define Standard_Multi_Mode         0x03
#define Auto_Multi_Mode             0x04

void Key_IO_Init(void);
void Key_Scan(void);
void Key_Process(void);
void Auto_Flash(void);
void Auto_Multi_Flash(void);
void Standard_Flash(void);
void Standard_Multi_Flash(void);

#endif