/**************************************************************************************************/
/*  $Date: May-24-2010$
 *   $Revision: 1.1
 *
 */

/**************************************************************************************************
* Copyright (C) 2010 Bosch Sensortec GmbH
*
* BMA220 acceleration sensor's adaptor layer on MTK platform
*
* Usage:		GPIO simulator I2C Interface for BMA220 read and write
*
* Author:       Xiaolong.gao@bosch-sensortec.com
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


#include "kal_release.h"
#include "gpio_sw.h"

#include "motion_sensor.h"

#include "bma220.h"
#include "motion_sensor_custom_bma220.h"
//#include "bosch_sensor_app.h"
//#include "gps_main.h"

//#include "Yd_main.h"



  #define i2c_delay   0
  #define i2c_delay_backup   50


   #define delay_short 100      //20		//No test
   #define delay_long  200     //40		//No test
   
#define EDGE_SENSITIVE           	KAL_TRUE
#define LEVEL_SENSITIVE          	KAL_FALSE
#define InterruptPin_Config()		GPIO_ModeSetup(MOTION_SENSOR_EINT_PORT, 2)

#define SET_I2C_CLK_OUTPUT 	GPIO_InitIO(OUTPUT,ACC_SENSOR_SCK); {int j; for(j=0;j<i2c_delay;j++);}
#define SET_I2C_DATA_OUTPUT	GPIO_InitIO(OUTPUT,ACC_SENSOR_SDA);{int j; for(j=0;j<i2c_delay;j++);}
#define SET_I2C_DATA_INPUT		GPIO_InitIO(INPUT,ACC_SENSOR_SDA);{int j; for(j=0;j<i2c_delay;j++);}
/*Modified by Huangwenjian 20170516 begin*/
//#define SET_I2C_CLK_HIGH		GPIO_WriteIO(1,ACC_SENSOR_SCK); {int j; for(j=0;j<i2c_delay;j++);}
//#define SET_I2C_CLK_LOW		GPIO_WriteIO(0,ACC_SENSOR_SCK); {int j; for(j=0;j<i2c_delay;j++);}

#define SET_I2C_CLK_HIGH			GPIO_InitIO(INPUT,ACC_SENSOR_SCK);\
									{int j; for(j=0;j<i2c_delay_backup;j++);}
#define SET_I2C_CLK_LOW			 GPIO_InitIO(OUTPUT,ACC_SENSOR_SCK);\
									GPIO_WriteIO(0,ACC_SENSOR_SCK);\
									{int j; for(j=0;j<i2c_delay_backup;j++);}

//#define SET_I2C_DATA_HIGH		GPIO_WriteIO(1,ACC_SENSOR_SDA);{int j; for(j=0;j<i2c_delay;j++);}
//#define SET_I2C_DATA_LOW		GPIO_WriteIO(0,ACC_SENSOR_SDA);{int j; for(j=0;j<i2c_delay;j++);}
#define SET_I2C_DATA_HIGH			GPIO_InitIO(INPUT,ACC_SENSOR_SDA);\
									{int j; for(j=0;j<i2c_delay_backup;j++);}
#define SET_I2C_DATA_LOW			 GPIO_InitIO(OUTPUT,ACC_SENSOR_SDA);\
									GPIO_WriteIO(0,ACC_SENSOR_SDA);\
									{int j; for(j=0;j<i2c_delay_backup;j++);}
/*Modified by Huangwenjian 20170516 end*/
#define GET_I2C_DATA_BIT		GPIO_ReadIO(ACC_SENSOR_SDA)

//defination only for MTK platform compiler
#define ACC_0G_X		(2059)
#define ACC_1G_X		(2059+16)
#define ACC_MINUS1G_X	(2059+16)

#define ACC_0G_Y		(2059)
#define ACC_1G_Y		(2059+16)
#define ACC_MINUS1G_Y	(2059+16)

#define ACC_0G_Z		(2059)
#define ACC_1G_Z		(2059+16)
#define ACC_MINUS1G_Z	(2059+16)

#define motion_3d_delay_time   216     //   ticks       1 tick =4.61ms 216

//extern void RJ_GPS_log(kal_uint8 * fmt,...);
extern void TRACE_P_3D(kal_uint8 * fmt,...);
extern void TRACE_P_GPS(kal_uint8 * fmt,...);

extern kal_timerid motion_3d_timerid;




MotionSensor_custom_data_struct *ms_get_data(void) ;
MotionSensor_customize_function_struct *ms_GetFunc(void);
void acc_sensor_get_6bytes_bma220(kal_uint16 *accl_x,kal_uint16 *accl_y,kal_uint16 *accl_z);

MotionSensor_custom_data_struct  ms_custom_data_def_bma220 = 
{          
	/*X axis*/
	ACC_0G_X,   
	ACC_1G_X,
	ACC_MINUS1G_X,     
	/*Y axis*/
	ACC_0G_Y,   
	ACC_1G_Y,
	ACC_MINUS1G_Y,     
	/*Z axis*/
	ACC_0G_Z,   
	ACC_1G_Z,
	ACC_MINUS1G_Z    
};

MotionSensor_customize_function_struct ms_custom_func_bma220=
{
     ms_get_data,
     acc_sensor_get_6bytes_bma220,
     acc_sensor_init_bma220,
     acc_sensor_pwr_up_bma220,
     acc_sensor_pwr_down_bma220     
};	

bma220_t t_bma220;


void bosch_motion_sensor_eint_hisr_bma220(void)
{ 

   
   
}


static void acc_sensor_interrput_configuration_bma220(void)
{
   InterruptPin_Config();
   EINT_SW_Debounce_Modify(MOTION_SENSOR_EINT_NO, SW_DEBOUNCE_TIME);
   EINT_Mask(MOTION_SENSOR_EINT_NO);
   EINT_Registration(MOTION_SENSOR_EINT_NO, KAL_TRUE, ACT_HIGH,
                     bosch_motion_sensor_eint_hisr_bma220, KAL_TRUE);
   EINT_Set_Sensitivity(MOTION_SENSOR_EINT_NO, LEVEL_SENSITIVE);
}

static void acc_sensor_interrput_disable_bma220(void)
{
   bma220_set_mode(BMA220_MODE_NORMAL);
   bma220_set_en_high_xyz(0x00);
   bma220_set_en_low(0x00);
   bma220_set_en_orient(0x00);   //0x0d
   bma220_set_orient_ex(0x00);
   bma220_set_en_tt_xyz(0x00);  //0x0d
   bma220_set_en_slope_xyz(0x00);  //0x0d
   bma220_set_latch_int(0x00);
   bma220_reset_int();
}

void acc_sensor_write_1byte_bma220(kal_int8 add, kal_int8 data)
{
   kal_int32 i, j;

   //Start
   SET_I2C_DATA_OUTPUT;
   SET_I2C_CLK_OUTPUT;
   SET_I2C_DATA_HIGH;
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_short; j++);
   SET_I2C_DATA_LOW;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_LOW;

   //First step--Send write command
   for (i = 7; i >= 0; i--)
   {
      if (BMA220_I2C_SLAVE_WRITE_ADDR &(1 << i))
      {
         SET_I2C_DATA_HIGH;
      }
      else
      {
         SET_I2C_DATA_LOW;
      }
      SET_I2C_CLK_HIGH;
      for (j = 0; j < delay_long; j++);
      SET_I2C_CLK_LOW;
      for (j = 0; j < delay_short; j++);
   }

   //Waiting for ACK from motion sensor
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_long; j++);
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   //May-18-2010 -- Modify by Johnson
   add=(add)<<1;

   //Second step -- Send register address
   for (i = 7; i >= 0; i--)
   {
      if (add &(1 << i))
      {
         SET_I2C_DATA_HIGH;
      }
      else
      {
         SET_I2C_DATA_LOW;
      }
      SET_I2C_CLK_HIGH;
      for (j = 0; j < delay_long; j++);
      SET_I2C_CLK_LOW;
      for (j = 0; j < delay_short; j++);
   }

   //Waiting for ACK from motion sensor
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_long; j++);
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   //Third step -- Send the data
   for (i = 7; i >= 0; i--)
   {
      if (data &(1 << i))
      {
         SET_I2C_DATA_HIGH;
      }
      else
      {
         SET_I2C_DATA_LOW;
      }
      SET_I2C_CLK_HIGH;
      for (j = 0; j < delay_long; j++);
      SET_I2C_CLK_LOW;
      for (j = 0; j < delay_short; j++);
   }

   //Waiting for ACK from motion sensor
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_long; j++);
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   //STOP
   //SET_I2C_DATA_OUTPUT;
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_short; j++);
   SET_I2C_DATA_LOW;
   for (j = 0; j < delay_short; j++);
   SET_I2C_DATA_HIGH;
}

//In fact in this version's driver, only support 1 byte write
//If needed multi bytes write, must modify this function
char acc_sensor_write_bytes_bma220(kal_uint8 dev_addr, kal_uint8 reg_addr,
                            kal_uint8 *reg_data, kal_uint8 wr_len)
{
   int i = 0;
   for (i = 0; i < wr_len; i++)
   {
      acc_sensor_write_1byte_bma220((reg_addr + i), *(reg_data + i));
   }

   return 1;
}

char acc_sensor_read_bytes_bma220(kal_uint8 dev_addr, kal_uint8 reg_addr, kal_uint8
                           *reg_data, kal_uint8 r_len)
{
   kal_int32 i, j, k;

   for (k = 0; k < r_len; k++)
   {
      *(reg_data + k) = 0;
   }
   
   //START
   SET_I2C_DATA_OUTPUT;
   SET_I2C_CLK_OUTPUT; 
   SET_I2C_DATA_HIGH;
   SET_I2C_CLK_HIGH;
   SET_I2C_DATA_LOW;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_LOW;

   //Need to confirm this delay
   for (j = 0; j < delay_long; j++);

   //First step--Send write command
   for (i = 7; i >= 0; i--)
   {
      if (BMA220_I2C_SLAVE_WRITE_ADDR &(1 << i))
      {
         SET_I2C_DATA_HIGH;
      }
      else
      {
         SET_I2C_DATA_LOW;
      }
      for (j = 0; j < delay_short; j++);
      SET_I2C_CLK_HIGH;
      for (j = 0; j < delay_long; j++);
      SET_I2C_CLK_LOW;
      for (j = 0; j < delay_short; j++);
   }

   //Waiting for ACK from motion sensor
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_long; j++);
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   //May-18-2010 -- Modify by Johnson
   reg_addr=(reg_addr)<<1;

   //Second step -- Send register address
   for (i = 7; i >= 0; i--)
   {
      if ((reg_addr) &(1 << i))
      {
         SET_I2C_DATA_HIGH;
      }
      else
      {
         SET_I2C_DATA_LOW;
      }
      SET_I2C_CLK_HIGH;
      for (j = 0; j < delay_long; j++);
      SET_I2C_CLK_LOW;
      for (j = 0; j < delay_short; j++);
   }

   //Waiting for ACK from motion sensor
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_long; j++);
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;
#if 0
   //write stop
   SET_I2C_DATA_LOW;
   SET_I2C_CLK_LOW;
   for(j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for(j = 0; j < delay_short; j++);
   SET_I2C_DATA_HIGH;
   for(j = 0; j < delay_short; j++);
#endif   
   //read start
   SET_I2C_DATA_HIGH;
   SET_I2C_CLK_HIGH;
   for(j = 0; j < delay_short; j++);
   SET_I2C_DATA_LOW;
   for(j = 0; j < delay_short; j++);
   SET_I2C_CLK_LOW;

   //Third step -- Send read command
   for (i = 7; i >= 0; i--)
   {
      if ((BMA220_I2C_SLAVE_READ_ADDR) &(1 << i))
      {
         SET_I2C_DATA_HIGH;
      }
      else
      {
         SET_I2C_DATA_LOW;
      }
      SET_I2C_CLK_HIGH;
      for (j = 0; j < delay_long; j++);
      SET_I2C_CLK_LOW;
      for (j = 0; j < delay_short; j++);
   }

   //Waiting for ACK from motion sensor
   SET_I2C_DATA_LOW;
   SET_I2C_DATA_INPUT;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_long; j++);
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_OUTPUT;

   for (k = 0; k < r_len; k++)
   {
      //----- Read data out now --------
      // Read  data
      SET_I2C_DATA_INPUT;

      for (i = 7; i >= 0; i--)
      {
         SET_I2C_CLK_HIGH;
         for (j = 0; j < delay_long; j++);
         if (GET_I2C_DATA_BIT)
         {
            *(reg_data + k) |= (1 << i);
         }
         SET_I2C_CLK_LOW;
         for (j = 0; j < delay_long; j++);
      }

	//Check if this is the last byte or not
	//if yes, send ACK, data=0
	//if no, send NACK, data=1
      if (k != r_len - 1)
      {
         //ACK
         SET_I2C_DATA_OUTPUT;
         SET_I2C_DATA_LOW;
         for (j = 0; j < delay_short; j++);
         SET_I2C_CLK_HIGH;
         for (j = 0; j < delay_long; j++);
         SET_I2C_CLK_LOW;
      }
      else
      {
         //NACK
         SET_I2C_DATA_OUTPUT;
         SET_I2C_DATA_HIGH;
         for (j = 0; j < delay_short; j++);
         SET_I2C_CLK_HIGH;
         for (j = 0; j < delay_long; j++);
         SET_I2C_CLK_LOW;
      }
   }

 
   //STOP
   SET_I2C_DATA_OUTPUT;
   SET_I2C_CLK_LOW;
   SET_I2C_DATA_LOW;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_short; j++);
   SET_I2C_DATA_HIGH;

   return 1;
} /* I2C_send_byte() */

static void System_Delay_1ms(kal_uint8 delay_count)
{
   kal_uint32 x;
   volatile kal_uint32 delay;

   #define INSTRUCTION_CLK_NUM 4
   #ifdef MCU_13M //   CLK = 1/13us 1ms=CLK*13000=1ms 1 ins= 6CLK
      #define MCU_CLK_HZ 13000
   #elif defined(MCU_26M)
      #define MCU_CLK_HZ 26000
   #elif defined(MCU_39M)
      #define MCU_CLK_HZ 39000
   #elif defined(MCU_52M)
      #define MCU_CLK_HZ 52000
   #elif defined(MCU_104M)
      #define MCU_CLK_HZ 104000
   #elif defined(MCU_208M)
      #define MCU_CLK_HZ 208000
   #endif
   
   for (delay = 0; delay < delay_count; delay++)
   {
        /*for (x = 0; x < (MCU_CLK_HZ / INSTRUCTION_CLK_NUM); x++){}*/
        for (x = 0; x < 91000; x++){}
   }
}

void acc_sensor_pwr_up_bma220(void)
{
   bma220_set_mode(BMA220_MODE_NORMAL);
   System_Delay_1ms(2);
}

void acc_sensor_pwr_down_bma220(void)
{
   bma220_set_mode(BMA220_MODE_SUSPEND);
}

void acc_sensor_init_bma220(void)
{
   static kal_uint8 p_flag = 0;
   
   GPIO_ModeSetup(ACC_SENSOR_SCK, 0x00);

   GPIO_ModeSetup(ACC_SENSOR_SDA, 0x00);

   if (p_flag == 0)
   {
      p_flag=1;
	  
      bma220_init(&t_bma220);
   }
   acc_sensor_pwr_up_bma220();
   
   acc_sensor_interrput_disable_bma220();
   
   bma220_set_range(BMA220_RANGE_2G);
   bma220_set_bandwidth(BMA220_BANDWIDTH_150HZ);
}

void acc_sensor_init_with_interrupt_bma220(void)
{
   extern void bosch_motion_sensor_eint_xyz(void);

    acc_sensor_pwr_up_bma220();   
    acc_sensor_interrput_disable_bma220();
   
    bma220_set_range(BMA220_RANGE_2G);      //BMA220_RANGE_2G
    bma220_set_bandwidth(BMA220_BANDWIDTH_1000HZ);     //BMA220_BANDWIDTH_150HZ

    //gps_start_timer(RJ_3D_TIMER, 1000, bosch_motion_sensor_eint_xyz, 0);
     kal_set_timer(motion_3d_timerid, (kal_timer_func_ptr)bosch_motion_sensor_eint_xyz, NULL, 
   	motion_3d_delay_time,	0);

	//RJ_GPS_log("acc_sensor_init_with_interrupt_bma220");
	//TRACE_P_3D("acc_sensor_init_with_interrupt_bma220");  

}

//This function only for MTK platfrom compiler
void acc_sensor_get_6bytes_bma220(kal_uint16 *accl_x,kal_uint16 *accl_y,kal_uint16 *accl_z)
{

	  bma220acc_t accl;

	   bma220_read_accel_xyz(&accl);

	   *accl_x=accl.x;
	   *accl_y=accl.y;
	   *accl_z=accl.z;
}


MotionSensor_custom_data_struct *ms_get_data_bma220(void) 
{
   return (&ms_custom_data_def_bma220);
} 
MotionSensor_customize_function_struct *ms_GetFunc_bma220(void)
{
   return (&ms_custom_func_bma220);  
} 

void slt_MmiMotionTstInit(void)
{
   acc_sensor_init_bma220();
   acc_sensor_pwr_up_bma220();
   //gSensorIsAutoTest = FALSE;
   //gui_start_timer(100, slt_MmiMotionTst);
}
void slt_MmiMotionTstexit(void)
{
   acc_sensor_pwr_down_bma220();
   //gui_cancel_timer(slt_MmiMotionTst);
}

slt_MmiMotionAutoTstInit(void)
{
   acc_sensor_init_bma220();
   acc_sensor_pwr_up_bma220();
   //gSensorIsAutoTest = TRUE;
   //gui_start_timer(100, slt_MmiMotionTst);
}
////////////////////////////////////////////////////
