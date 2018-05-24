#ifndef __SKIN_COLOR_CHECK_H
#define __SKIN_COLOR_CHECK_H

/* Includes ------------------------------------------------------------------*/
#include "SN8F5702.h"

#define COLOR_TPYE_5_H										1500  /* color type 5:  1.8v */
#define COLOR_TPYE_5_L                    100  /* color type 5: 0.14v */

#define COLOR_TPYE_4_H										2500 /* color type 4: 2.1v */
#define COLOR_TPYE_4_L                    2401  /* color type 4: 1.8v */

#define COLOR_TPYE_3_H										2750  /* color type 3: 2.4v */
#define COLOR_TPYE_3_L                    2551 /* color type 3: 2.1v */

#define COLOR_TPYE_1_H                    3577  /* color type 1: 4v */
#define COLOR_TPYE_1_L                    2751  /* color type 1: 2.4v */
 
#define COLOR_TYPE_STANDARD_              65  /* color standard : 0.080v */
//#define COLOR_TYPE_STANDARD_L             3255

void Skin_Color_Check(void);

#endif

//#define COLOR_TPYE_5G_H										2100  /* color type 5:  800mv */
//#define COLOR_TPYE_5G_L                    200  /* color type 5: 0.14v */

//#define COLOR_TPYE_4G_H										2300 /* color type 4: 1.5v */
//#define COLOR_TPYE_4G_L                    2101  /* color type 4: 1.3v */

//#define COLOR_TPYE_3G_H										2500  /* color type 3: 2.0v */
//#define COLOR_TPYE_3G_L                    2301  /* color type 3: 1.5v */

//#define COLOR_TPYE_1G_H                    3577  /* color type 1: 4v */
//#define COLOR_TPYE_1G_L                    2857  /* color type 1: 3v */



//#define COLOR_TPYE_5B_H										2100  /* color type 5:  800mv */
//#define COLOR_TPYE_5B_L                    200  /* color type 5: 0.14v */

//#define COLOR_TPYE_4B_H										2300 /* color type 4: 1.5v */
//#define COLOR_TPYE_4B_L                    2101  /* color type 4: 1.3v */

//#define COLOR_TPYE_3B_H										2500  /* color type 3: 2.0v */
//#define COLOR_TPYE_3B_L                    2301  /* color type 3: 1.5v */

//#define COLOR_TPYE_1B_H                    3577  /* color type 1: 4v */
//#define COLOR_TPYE_1B_L                    2857  /* color type 1: 3v */
