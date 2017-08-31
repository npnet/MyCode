/**************************************************************************************************/
/*  $Date: March-1-20111
 *   $Revision: 1.01$
 *
 */

/**************************************************************************************************
* Copyright (C) 2011 Bosch Sensortec GmbH
*
* BOSCH algorithm API
*
* Usage:		BOSCH portrait landscape and skake snap algorithm
*
* Author:       yongsheng.gao@bosch-sensortec.com
**************************************************************************************************/
/*  Disclaimer
*
* Common:
* Bosch Sensortec products are developed for the consumer goods industry. They may only be used
* within the parameters of the respective valid product data sheet.  Bosch Sensortec products are
* provided with the express understanding that there is no warranty of fitness for a particular purpose.
* They are not fit for use in life-sustaining, safety or security sensitive systems or any system or device
* that may lead to bodily harm or property damage if the system or device malfunctions. In addition,
* Bosch Sensortec products are not fit for use in products which interact with motor vehicle systems.
* The resale and/or use of products are at the purchaser’s own risk and his own responsibility. The
* examination of fitness for the intended use is the sole responsibility of the Purchaser.
*
* The purchaser shall indemnify Bosch Sensortec from all third party claims, including any claims for
* incidental, or consequential damages, arising from any product use not covered by the parameters of
* the respective valid product data sheet or not approved by Bosch Sensortec and reimburse Bosch
* Sensortec for all costs in connection with such claims.
*
* The purchaser must monitor the market for the purchased products, particularly with regard to
* product safety and inform Bosch Sensortec without delay of all security relevant incidents.
*
* Engineering Samples are marked with an asterisk (*) or (e). Samples may vary from the valid
* technical specifications of the product series. They are therefore not intended or fit for resale to third
* parties or for use in end products. Their sole purpose is internal client testing. The testing of an
* engineering sample may in no way replace the testing of a product series. Bosch Sensortec
* assumes no liability for the use of engineering samples. By accepting the engineering samples, the
* Purchaser agrees to indemnify Bosch Sensortec from all claims arising from the use of engineering
* samples.
*
* Special:
* This software module (hereinafter called "Software") and any information on application-sheets
* (hereinafter called "Information") is provided free of charge for the sole purpose to support your
* application work. The Software and Information is subject to the following terms and conditions:
*
* The Software is specifically designed for the exclusive use for Bosch Sensortec products by
* personnel who have special experience and training. Do not use this Software if you do not have the
* proper experience or training.
*
* This Software package is provided `` as is `` and without any expressed or implied warranties,
* including without limitation, the implied warranties of merchantability and fitness for a particular
* purpose.
*
* Bosch Sensortec and their representatives and agents deny any liability for the functional impairment
* of this Software in terms of fitness, performance and safety. Bosch Sensortec and their
* representatives and agents shall not be liable for any direct or indirect damages or injury, except as
* otherwise stipulated in mandatory applicable law.
*
* The Information provided is believed to be accurate and reliable. Bosch Sensortec assumes no
* responsibility for the consequences of use of such Information nor for any infringement of patents or
* other rights of third parties which may result from its use. No license is granted by implication or
* otherwise under any patent or patent rights of Bosch. Specifications mentioned in the Information are
* subject to change without notice.
*
* It is not allowed to deliver the source code of the Software to any third party without permission of
* Bosch Sensortec.
*/
/*************************************************************************************************/
#ifdef DW02_PROJECT

#ifndef _MOTION_SENSOR_CUSTOM_H
#define _MOTION_SENSOR_CUSTOM_H

#include "kal_release.h"
#include "motion_sensor.h"

/*GPIO for Bosch BMA222 g-sensor serial interface*/
#define ACC_SENSOR_SCK 43 // 1  lqy  //48
#define ACC_SENSOR_SDA 44 // 2  lqy //47

#define MOTION_SENSOR_EINT_NO    23//8 lqy //INT5
#define SW_DEBOUNCE_TIME         10   //6 //10ms unit

// The default IIC address of the devie is 0x08, when SDO is pulled to GND or left open.
// Otherwise, the address is 0x09, when SDO is pulled to Vddio.
// default address
#define BMA222_I2C_SLAVE_WRITE_ADDR   0x30  //0x10
#define BMA222_I2C_SLAVE_READ_ADDR 0x31     //0x11

// alternative address
//#define BMA222_I2C_SLAVE_WRITE_ADDR 0x12
//#define BMA222_I2C_SLAVE_READ_ADDR 0x13

#define MOTION_SENSOR_EINT_PORT  52//61 lqy//MTK6225 GPIO_1 -> INT5    gpio43----eint2
#ifdef MS_EINT_SUPPORT
#ifdef DW06A_PCB
#define HIGH_G_EINT_NO    14//
#define HIGH_G_EINT_PORT  20 // eint14 MTK6261
#endif
#ifdef DW21_PCB
#define HIGH_G_EINT_NO    5//
#define HIGH_G_EINT_PORT  6 // 
#endif
#endif
#define ACT_HIGH                 1

#define motion_3d_delay_time   216     //   ticks       1 tick =4.61ms 216
void acc_sensor_pwr_up(void);
void acc_sensor_pwr_down(void);
void acc_sensor_init(void);
void acc_sensor_init_with_interrupt(void);
void acc_sensor_init_slope_no_motion(void);
char acc_sensor_write_bytes(kal_uint8 dev_addr, kal_uint8 reg_addr, kal_uint8* reg_data, kal_uint8 wr_len);
char acc_sensor_read_bytes(kal_uint8 dev_addr, kal_uint8 reg_addr, kal_uint8* reg_data, kal_uint8 r_len);

void acc_sensor_get_6bytes(kal_uint16 *accl_x,kal_uint16 *accl_y,kal_uint16 *accl_z);


void acc_sensor_int_reset(void);
MotionSensor_custom_data_struct *ms_get_data(void) ;
MotionSensor_customize_function_struct *ms_GetFunc(void);
#endif

#endif

