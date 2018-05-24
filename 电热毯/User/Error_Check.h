#ifndef __ERROR_CHECK_H
#define __ERROR_CHECK_H

#include "SN8F5702.H"

#define FAN_OPEN_ERROR_THR                100  /* fan open: 0v*/
#define FAN_SHORT_ERROR_THR               4200 /* fan short: 5v */

#define CARTRIDGE_ERROR_THR               100  /* cartridge open: 0v */         
#define CARTRIDGE_TYPE_1_H                2457 /* One Lamp: 2.5v--3.5v */
#define CARTRIDGE_TYPE_1_L                2048 
#define CARTRIDGE_TYPE_3_H                1965 /* One Lamp: 1.5v--2.4v */
#define CARTRIDGE_TYPE_3_L                1228
#define CARTRIDGE_TYPE_2                  3600 /* two lamp: 4.5v */

void Fan_Error_Check(void);
void Cartridge_Error_Check(void);
void Cartridge_Type_Check(void);
void Error_Check(void);


#endif