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
* The resale and/or use of products are at the purchaser抯 own risk and his own responsibility. The
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

#include "kal_release.h"
#include "gpio_sw.h"

#include "motion_sensor.h"

#include "bma222.h"
#include "bma220.h"

#include "motion_sensor_custom.h"
#include "mmi_features.h"

#include "TimerEvents.h"

#include "rj_dw02.h"

//#include "gps_main.h"

//#include  "bosch_sensor_algo.h"





 #define i2c_delay   0
 #define delay_short 100//20		//No test
 #define delay_long  200//40		//No test

#define EDGE_SENSITIVE           	KAL_TRUE
#define LEVEL_SENSITIVE          	KAL_FALSE
#define InterruptPin_Config()		GPIO_ModeSetup(MOTION_SENSOR_EINT_PORT, 2)      // ranyingchun    2   
#ifdef MS_EINT_SUPPORT
#ifdef DW06A_PCB
#define InterruptPin_high_g_Config()		GPIO_ModeSetup(HIGH_G_EINT_PORT, 2)      // ranyingchun   
#endif
#ifdef DW21_PCB
#define InterruptPin_high_g_Config()		GPIO_ModeSetup(HIGH_G_EINT_PORT, 1)      // ranyingchun   
#endif
#endif

#define SET_I2C_CLK_OUTPUT 		GPIO_InitIO(OUTPUT,ACC_SENSOR_SCK); {int j; for(j=0;j<i2c_delay;j++);}
#define SET_I2C_DATA_OUTPUT		GPIO_InitIO(OUTPUT,ACC_SENSOR_SDA);{int j; for(j=0;j<i2c_delay;j++);}
#define SET_I2C_DATA_INPUT		GPIO_InitIO(INPUT,ACC_SENSOR_SDA);{int j; for(j=0;j<i2c_delay;j++);}
/*Modified by Huangwenjian 20170516 begin*/
#define SET_I2C_CLK_HIGH			GPIO_InitIO(INPUT,ACC_SENSOR_SCK);\
									{int j; for(j=0;j<50;j++);}
#define SET_I2C_CLK_LOW			 GPIO_InitIO(OUTPUT,ACC_SENSOR_SCK);\
									GPIO_WriteIO(0,ACC_SENSOR_SCK);\
									{int j; for(j=0;j<50;j++);}
//#define SET_I2C_CLK_HIGH		GPIO_WriteIO(1,ACC_SENSOR_SCK); {int j; for(j=0;j<i2c_delay;j++);}
//#define SET_I2C_CLK_LOW			GPIO_WriteIO(0,ACC_SENSOR_SCK); {int j; for(j=0;j<i2c_delay;j++);}

//#define SET_I2C_DATA_HIGH		GPIO_WriteIO(1,ACC_SENSOR_SDA);{int j; for(j=0;j<i2c_delay;j++);}
//#define SET_I2C_DATA_LOW		GPIO_WriteIO(0,ACC_SENSOR_SDA);{int j; for(j=0;j<i2c_delay;j++);}
#define SET_I2C_DATA_HIGH			GPIO_InitIO(INPUT,ACC_SENSOR_SDA);\
									{int j; for(j=0;j<50;j++);}
#define SET_I2C_DATA_LOW			 GPIO_InitIO(OUTPUT,ACC_SENSOR_SDA);\
									GPIO_WriteIO(0,ACC_SENSOR_SDA);\
									{int j; for(j=0;j<50;j++);}
/*Modified by Huangwenjian 20170516 end*/
#define GET_I2C_DATA_BIT		GPIO_ReadIO(ACC_SENSOR_SDA)

//defination only for MTK platform compiler
#define ACC_0G_X		(2059)
#define ACC_1G_X		(2059+128)
#define ACC_MINUS1G_X	(2059+128)

#define ACC_0G_Y		(2059)
#define ACC_1G_Y		(2059+128)
#define ACC_MINUS1G_Y	(2059+128)

#define ACC_0G_Z		(2059)
#define ACC_1G_Z		(2059+128)
#define ACC_MINUS1G_Z	(2059+128)

#define XYZ_ABS(a)			((a) >= 0 ? (a) : (-(a)))
#define XYZ_BIGGER(a,b)			(a >= b ? (a) : (b))

extern kal_int32 EINT_SW_Debounce_Modify(kal_uint8 eintno, kal_uint8 debounce_time);
extern void EINT_Mask(kal_uint8 eintno);
extern void EINT_Registration(kal_uint8 eintno, kal_bool Dbounce_En, kal_bool ACT_Polarity, void (reg_hisr)(void), kal_bool auto_umask);
extern kal_uint32 EINT_Set_Sensitivity(kal_uint8 eintno, kal_bool sens);

extern void RJ_GPS_log(kal_uint8 * fmt,...);
extern void TRACE_P_3D(kal_uint8 * fmt,...);
extern void TRACE_P_GPS(kal_uint8 * fmt,...);
//extern void Acc_Switch_on_Progress(void);



extern bma220_t t_bma220;


/********yanchunhai 20120816   *****************************/
/***b_is_no_motion = KAL_TRUE    means GPS  is stable;  b_motion = KAL_TRUE  means GPS  motion*/  
/* 只针对于传感器判断 */
kal_bool  b_is_no_motion = KAL_FALSE;                    /* GPS 静止 */
kal_bool b_motion = KAL_TRUE;                              /* GPS 静止 */

kal_timerid motion_3d_timerid;
kal_uint8 is_init_3d = KAL_FALSE;
kal_uint16 X_DATA[10] = { 0 },Y_DATA[10] = { 0 },Z_DATA[10] = { 0 };

/********yanchunhai 20120816   *****************************/

MotionSensor_custom_data_struct  ms_custom_data_def =
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

kal_bool get_b_motion()
{
	if(b_motion==0)
		{
			return KAL_FALSE;
		}
	else{
			return KAL_TRUE;
		}
	
}
kal_int8 acc_sensor_get_ini()
{
	return is_init_3d;
}
#ifdef BIRD_ECU_SUPPORT
kal_bool motion_flag = KAL_FALSE;
extern kal_bool ecu_motion_flag ;
kal_uint32 ecu_count=0;
kal_bool get_motion_flag()
{
	 if(ecu_count>5)
	 {
	motion_flag=b_motion;
	 }
	 else{
	 motion_flag=b_motion | ecu_motion_flag;}
	 
	if(motion_flag==0)
		{
			return KAL_FALSE;
		}
	else{
			return KAL_TRUE;
		}	 
	 
}
#endif
MotionSensor_customize_function_struct ms_custom_func=
{
     ms_get_data,
     acc_sensor_get_6bytes,
     acc_sensor_init,
     acc_sensor_pwr_up,
     acc_sensor_pwr_down
};

bma222_t t_bma222;





void bosch_motion_sensor_int_enable(kal_bool enable)
{
   if(enable==KAL_TRUE)   
      EINT_UnMask(MOTION_SENSOR_EINT_NO);
   else
      EINT_Mask(MOTION_SENSOR_EINT_NO);
}   

kal_uint16 acc_sensor_biggest_diff(kal_uint16 *accl_data)
{
	kal_uint8  i = 0,plus_count =0,minus_count =0;
	kal_uint8  minus_flag = 1,plus_flag = 1;

	short data[10] ={ 0 } ;
	kal_uint16 plus_data = 0,small_plus_data = 0, small_minus_data = 0,big_minus_data = 0,tmp=0;


   	for(i = 0; i<10; i++)
   	{
   	     data[i] = *(accl_data++);
		 
		//TRACE_P_3D("data = %d" ,data);
		 //kal_prompt_trace(MOD_WPS,"data = %d" ,data[i]);
         	if( data[i] >= 0)
         	{
			plus_count++;
         		//minus_flag = 0;
				
                 	if(data[i]>plus_data)  // 求正数的最大值
                 	{
                           plus_data =   data[i];
                 	}
			if(1==plus_count)
			{
				small_plus_data = data[i];				

			}
			else
			{
				if(data[i]<=small_plus_data)
				{
					small_plus_data = data[i];
				}
				
			}
         	}
		else
		{
			//plus_flag = 0;
			minus_count++;
			
			if( XYZ_ABS(data[i]) > big_minus_data)     //求负数的最大值
	         	{
				big_minus_data = XYZ_ABS(data[i]);
			}

			if(1==minus_count)
			{
				small_minus_data = XYZ_ABS(data[i]);			  //  ranyingchun	

			}
			else
			{
				if(XYZ_ABS(data[i])<=small_minus_data)
				{
					small_minus_data = XYZ_ABS(data[i]);
				}
				
			}
		}			

   	}

	

      if( 10 == minus_count )   //  < = 0
     	{     	   		  
		return big_minus_data-small_minus_data;

     	}
	 else if(10==plus_count)    //>0
	 {		
		return plus_data-small_plus_data;
	 }
	 else
	 {

		return plus_data+big_minus_data;

	 }


}

kal_uint16 acc_sensor_XYZ_diff(void)
{
    kal_uint8 k = 0;
    kal_uint16 X = 0, Y = 0, Z= 0;
	
    X = acc_sensor_biggest_diff(X_DATA);
    Y = acc_sensor_biggest_diff(Y_DATA);
    Z = acc_sensor_biggest_diff(Z_DATA);
    
//	TRACE_P_3D("XYZ_diff x=%d,y=%d,z=%d,big=%d",X,Y,Z,XYZ_BIGGER(XYZ_BIGGER(X,Y),Z));     // ranyingchun
/*
	 TRACE_P_3D("XYZ_diff x=%d,y=%d,z=%d,big=%d",X,Y,Z,XYZ_BIGGER(XYZ_BIGGER(X,Y),Z));
	 if(0xdd == t_bma220.chip_id)
	 {
	 	 kal_prompt_trace(MOD_WPS,"bma220 XYZ_diff x=%d,y=%d,z=%d,big=%d",X,Y,Z,XYZ_BIGGER(XYZ_BIGGER(X,Y),Z));
	 }
	 else
	 {
	 	kal_prompt_trace(MOD_WPS,"bma222 XYZ_diff x=%d,y=%d,z=%d,big=%d",X,Y,Z,XYZ_BIGGER(XYZ_BIGGER(X,Y),Z));
	 }
*/	 
	if(0xdd == t_bma220.chip_id){
		kal_prompt_trace(MOD_WPS,"XYZ_diff x=%d,y=%d,z=%d,big=%d",X,Y,Z,XYZ_BIGGER(XYZ_BIGGER(X+Y,Y+Z),X+Z));
		return XYZ_BIGGER(XYZ_BIGGER(X+Y,Y+Z),X+Z);
		
	}
	else
	{
		kal_prompt_trace(MOD_WPS,"XYZ_diff x=%d,y=%d,z=%d,big=%d",X,Y,Z,XYZ_BIGGER(XYZ_BIGGER(X,Y),Z));
		return XYZ_BIGGER(XYZ_BIGGER(X,Y),Z);
		
	}
   
}


kal_bool  is_bosch_motion_sensor_motion(short xyz)
{
    kal_uint8 n=0;
    kal_uint8 ret = KAL_TRUE;
    extern kal_uint8 bird_get_vibration_sensitivity_value();    // 1-5  默认5
    
 	
	if(0xdd == t_bma220.chip_id)
	{	
     	
		   if(bird_get_vibration_sensitivity_value()>14)   
		    {
		          n=14;                                                //
		         
		    }
		    else
		    {
		          n = bird_get_vibration_sensitivity_value();        // 14 - bird_get_vibration_sensitivity_value();  
		    }	 


	}
      else{
    		if(bird_get_vibration_sensitivity_value()>15)   
    		{

		#ifdef MS_EINT_SUPPORT
			 n=4;   
		#else
		        n=15;                      //
		#endif
         
    		}
    		else
    		{
	       #ifdef MS_EINT_SUPPORT	
			n = (bird_get_vibration_sensitivity_value()+1)/4;        // n =15 - bird_get_vibration_sensitivity_value();  
			if(n == 0)
				n = 1;
		#else
	         		n =bird_get_vibration_sensitivity_value();        // n =15 - bird_get_vibration_sensitivity_value();  
		#endif
    		}

	}
       
	if((xyz>n))
	{
	        ret = KAL_TRUE;
	}
	else
	{
	        ret = KAL_FALSE;
	}         

       //TRACE_P_3D("ret=%d,n=%d, sens=%d,%d", ret,n,bird_get_vibration_sensitivity_value(),xyz);

    return ret;          
}


/*
* FUNCTION                                                            
*	bosch_motion_sensor_eint_hisr
*
* DESCRIPTION                                                           
*   	This function is to handle low-g and high-g interrupt
*
* CALLS  
*
* PARAMETERS
*	None
*	
* RETURNS
*	None
*/

volatile kal_uint8 motion_sensor_run_static_en = 1;
void bosch_motion_sensor_eint_xyz(void)
{
	kal_uint8  n = 1;   //  YDBJ,n
	
	static kal_uint8  i=0,j=0 ;
	short   x=0,y=0,z=0;
	kal_uint16   XYZ=0;

	static  kal_uint8 data_count= 0, k = 0;
	static  kal_uint8 flag_xyz = KAL_FALSE;

   	static short  x_prev=0,y_prev=0,z_prev=0;
	 kal_cancel_timer(motion_3d_timerid);   


 	if(0xdd == t_bma220.chip_id)
	{
		acc_sensor_get_6bytes_bma220(&x,&y,&z);
	}
	else if(0xf8 == t_bma222.chip_id)
	{
             acc_sensor_get_6bytes(&x,&y,&z);

	}  	

	   X_DATA[k] = x;
	   Y_DATA[k] = y;
	   Z_DATA[k] = z;
	   kal_prompt_trace(MOD_SOC,"x=%d,y=%d,z=%d,k=%d",x,y,z,k);


	   k++;

        //TRACE_P_3D("x=%d,y=%d,z=%d,k=%d",x,y,z,k);

	   if(k == 10)
	  {
       	  k = 0;

		    XYZ = acc_sensor_XYZ_diff();
		   memset(&X_DATA, 0, sizeof(X_DATA));
		   memset(&Y_DATA, 0, sizeof(Y_DATA));
		   memset(&Z_DATA, 0, sizeof(Z_DATA));
		   
		  	if(KAL_TRUE==flag_xyz)
			{
				//if(is_bosch_motion_sensor_motion(XYZ_ABS(x-x_prev),XYZ_ABS(y-y_prev),XYZ_ABS(z-z_prev)))  // 运动
				if(is_bosch_motion_sensor_motion(XYZ))  // 运动
		             {
		                  i = 0;
				    j++;
				}
				else  //静止
				{
				     j = 0;
		                   i++;
				}
			}

	       if(i>10)  //5s 静止判断 8s
		{
			j = 0;
			i = 5;
	            b_motion = KAL_FALSE;
				if(!b_is_no_motion)
				{
		    			 rj_3d_sensor_static_alarm();
				}
		}
		else if(j >0)    //(j >=n)  //运动判断
		{
			i = 0;
			j = 1;
		     b_motion = KAL_TRUE;
				// if(b_is_no_motion)
				 {
		     			rj_3d_sensor_running_alarm();
				 }
		}
		      	b_is_no_motion = !b_motion;

        if(KAL_FALSE==flag_xyz)
	{
        
          flag_xyz = KAL_TRUE;

	}              

	  //  TRACE_P_3D("b_motion=%d",b_motion);

	kal_prompt_trace(MOD_SOC,"b_motion=%d",b_motion); 

	//Acc_Switch_on_Progress();


      }

        // TRACE_P_3D("x=%d,y=%d,z=%d",x,y,z);

         // TRACE_P_3D("Cx=%d,Cy=%d,Cz=%d,n=%d,b_motion=%d",XYZ_ABS(x - x_prev),XYZ_ABS(y-y_prev),XYZ_ABS(z-z_prev),n,b_motion);
          // RJ_GPS_log("Cx=%d,Cy=%d,Cz=%d,n=%d,b_motion=%d",XYZ_ABS(x - x_prev),XYZ_ABS(y-y_prev),XYZ_ABS(z-z_prev),n,b_motion);
 	   


   	 
          kal_set_timer(motion_3d_timerid, (kal_timer_func_ptr)bosch_motion_sensor_eint_xyz, NULL, 21,0);    
   
}

void sensor_3d_guard(void)
{
        #if  0
        gps_stop_timer(RJ_3D_TIMER);
        gps_start_timer(RJ_3D_TIMER, 1000, bosch_motion_sensor_eint_xyz, 0);
        #endif

        RJ_GPS_log("sensor_3d_guard");

}

void bosch_motion_sensor_eint_hisr(void)
{
			rj_3d_sensor_collision_alarm();
	//		TRACE_P_3D("bosch_motion_sensor_eint_hisr ------ high_g");
			 kal_prompt_trace(MOD_BIRD,"bosch_motion_sensor_eint_hisr ------ high_g");
}


#ifdef MS_EINT_SUPPORT
 void acc_sensor_interrput_configuration_bma222e(void)     //static
{
   InterruptPin_Config();
   InterruptPin_high_g_Config();
//   EINT_SW_Debounce_Modify(MOTION_SENSOR_EINT_NO, 2);    // SW_DEBOUNCE_TIME
//   EINT_Mask(MOTION_SENSOR_EINT_NO);
//   EINT_Registration(MOTION_SENSOR_EINT_NO, KAL_TRUE, KAL_FALSE,                 /* ACT_HIGH     KAL_FALSE*/
 //                    bosch_motion_sensor_eint_hisr, KAL_TRUE);
 //  EINT_Set_Sensitivity(MOTION_SENSOR_EINT_NO, LEVEL_SENSITIVE);
/*
   EINT_Registration(MOTION_SENSOR_EINT_NO,KAL_FALSE, KAL_FALSE,bosch_motion_sensor_eint_hisr, KAL_TRUE);
   EINT_Set_Sensitivity(MOTION_SENSOR_EINT_NO, EDGE_SENSITIVE);
  */ 
      EINT_Registration(HIGH_G_EINT_NO,KAL_FALSE, KAL_FALSE,bosch_motion_sensor_eint_hisr, KAL_TRUE);
   EINT_Set_Sensitivity(HIGH_G_EINT_NO, EDGE_SENSITIVE);
}
#endif


void acc_sensor_interrput_configuration(void)
{
   InterruptPin_Config();   
/*
   EINT_SW_Debounce_Modify(MOTION_SENSOR_EINT_NO, SW_DEBOUNCE_TIME);
   EINT_Mask(MOTION_SENSOR_EINT_NO);
   EINT_Registration(MOTION_SENSOR_EINT_NO, KAL_TRUE, ACT_HIGH,
                     bosch_motion_sensor_eint_hisr, KAL_TRUE);
   EINT_Set_Sensitivity(MOTION_SENSOR_EINT_NO, LEVEL_SENSITIVE);
*/   
   EINT_Registration(MOTION_SENSOR_EINT_NO,KAL_FALSE, KAL_FALSE,bosch_motion_sensor_eint_hisr, KAL_TRUE);
   EINT_Set_Sensitivity(MOTION_SENSOR_EINT_NO, EDGE_SENSITIVE);
   kal_prompt_trace(MOD_BIRD,"acc_sensor_interrput_configuration");
}

static void acc_sensor_interrput_disable(void)
{
	unsigned char i;
	bma222_set_mode(0x00);

	for (i=0; i<11; i++)
		bma222_set_Int_Enable(i, 0);

	bma222_reset_interrupt();
}

void acc_sensor_write_1byte(kal_int8 add, kal_int8 data)
{
   kal_int32 i, j;

   //Start
   //SET_I2C_DATA_OUTPUT;
   //SET_I2C_CLK_OUTPUT;
   /*Delete by Huangwenjian 20170516*/
   SET_I2C_DATA_HIGH;
   SET_I2C_CLK_HIGH;
   for (j = 0; j < delay_short; j++);
   SET_I2C_DATA_LOW;
   for (j = 0; j < delay_short; j++);
   SET_I2C_CLK_LOW;

   //First step--Send write command
   for (i = 7; i >= 0; i--)
   {
      if (BMA222_I2C_SLAVE_WRITE_ADDR &(1 << i))
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
char acc_sensor_write_bytes(kal_uint8 dev_addr, kal_uint8 reg_addr,
                            kal_uint8 *reg_data, kal_uint8 wr_len)
{
   int i = 0;
   for (i = 0; i < wr_len; i++)
   {
      acc_sensor_write_1byte((reg_addr + i), *(reg_data + i));
   }

   return 1;
}

char acc_sensor_read_bytes(kal_uint8 dev_addr, kal_uint8 reg_addr, kal_uint8
                           *reg_data, kal_uint8 r_len)
{
   kal_int32 i, j, k;

   for (k = 0; k < r_len; k++)
   {
      *(reg_data + k) = 0;
   }

   //START
   //SET_I2C_DATA_OUTPUT;
   //SET_I2C_CLK_OUTPUT;
   /*Delete by Huangwenjian 20170516*/
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
      if (BMA222_I2C_SLAVE_WRITE_ADDR &(1 << i))
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

   //write stop
   SET_I2C_DATA_LOW;
   SET_I2C_CLK_LOW;
   for(j = 0; j < delay_short; j++);
   SET_I2C_CLK_HIGH;
   for(j = 0; j < delay_short; j++);
   SET_I2C_DATA_HIGH;
   for(j = 0; j < delay_short; j++);

   //read start
   //SET_I2C_DATA_HIGH;
   //SET_I2C_CLK_HIGH;
   SET_I2C_DATA_LOW;
   for(j = 0; j < delay_short; j++);
   SET_I2C_CLK_LOW;

   //Third step -- Send read command
   for (i = 7; i >= 0; i--)
   {
      if ((BMA222_I2C_SLAVE_READ_ADDR) &(1 << i))
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

void System_Delay_1ms(kal_uint8 delay_count)
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

void acc_sensor_pwr_up(void)
{
   bma222_set_mode(bma222_MODE_NORMAL);
   System_Delay_1ms(1);
}

void acc_sensor_pwr_down(void)
{
   bma222_set_mode(bma222_MODE_SUSPEND);
}








void acc_sensor_int_reset(void)
{
    acc_sensor_interrput_disable();

   // normal settings
   bma222_set_range(bma222_RANGE_4G);
  // bma222_set_range(bma222_RANGE_2G);
 
   bma222_set_bandwidth(bma222_BW_125HZ); 
  
   // set interrupt latch duration.
   bma222_set_Int_Mode(0x0B);	// latch 1ms

   // map all interrupt to INT1
   bma222_set_int_data_sel(0);	// map new data interrupt to INT1.
   bma222_set_int1_pad_sel(0);	// low G 		--> INT1
   bma222_set_int1_pad_sel(1);	// high G 		--> INT1
   bma222_set_int1_pad_sel(2);	// slope 		--> INT1
   bma222_set_int1_pad_sel(3);	// double tap 	--> INT1
   bma222_set_int1_pad_sel(4);	// single tap 	--> INT1
   bma222_set_int1_pad_sel(5);	// orient 		--> INT1
   bma222_set_int1_pad_sel(6);	// flat 		--> INT1

  bma222_set_int1_pad_sel(7);	// slo_no_mot 		--> INT1

   //bma222_set_int2_pad_sel();

   // set INT1 trigger level, push pull and open drive
   bma222_set_int_set(0, 1);	// INT1 level high
   bma222_set_int_set(1, 0);	// INT1 push pull(0), open drive(1)
   bma222_set_int_set(2, 1);	// INT2 level high
   bma222_set_int_set(3, 0);	// INT2 push pull(0), open drive(1)


}
void acc_sensor_init(void)
{
   static kal_uint8 p_flag = 0;

   GPIO_ModeSetup(ACC_SENSOR_SCK, 0x00);

   GPIO_ModeSetup(ACC_SENSOR_SDA, 0x00);

   if (p_flag == 0)
   {
      p_flag=1;

      bma222_init(&t_bma222);
   }
   acc_sensor_pwr_up();

   acc_sensor_interrput_disable();

   bma222_set_range(bma222_RANGE_4G);
   bma222_set_bandwidth(bma222_BW_125HZ);

}
void acc_sensor_probe(void)
{
	 kal_uint8   data;
	extern void acc_sensor_init_with_interrupt_bma220(void);


    
#if 1
       if(is_init_3d)
             return;
       
   	GPIO_ModeSetup(ACC_SENSOR_SCK, 0x00);
   	GPIO_ModeSetup(ACC_SENSOR_SDA, 0x00);
/*Added by Huangwenjian 20170516 end*/	
	SET_I2C_CLK_OUTPUT;
	SET_I2C_DATA_OUTPUT;  
	SET_I2C_DATA_HIGH;
	SET_I2C_CLK_HIGH;
	SET_I2C_CLK_LOW;
	SET_I2C_CLK_HIGH;	
/*Added by Huangwenjian 20170516 end*/
       motion_3d_timerid =kal_create_timer("3d sensor");
       kal_sleep_task(100);
  
       bma220_init(&t_bma220);
       bma222_init(&t_bma222);  
       
	{  	  	
		extern char acc_sensor_read_bytes_bma220(kal_uint8 dev_addr, kal_uint8 reg_addr, kal_uint8  *reg_data, kal_uint8 r_len);
	    	acc_sensor_read_bytes(0x0a, 0x00, &data, 1); 
	}    

    	TRACE_P_3D("t_bma222.chip_id=0x%x,t_bma220.chip_id=0x%x,data=0x%x", t_bma222.chip_id,t_bma220.chip_id,data);
	TRACE_P_GPS("t_bma222.chip_id=0x%x,t_bma220.chip_id=0x%x,data=0x%x", t_bma222.chip_id,t_bma220.chip_id,data);
	RJ_GPS_log("t_bma222.chip_id=0x%x,t_bma220.chip_id=0x%x",t_bma222.chip_id,t_bma220.chip_id);
	kal_prompt_trace(MOD_SOC, "HWJ t_bma222.chip_id=0x%x,t_bma220.chip_id=0x%x",t_bma222.chip_id,t_bma220.chip_id);

      if(0xdd == t_bma220.chip_id)
	{
            is_init_3d = KAL_TRUE;
            acc_sensor_init_with_interrupt_bma220();

	}
	else if(0xf8 == t_bma222.chip_id)
	{
             is_init_3d = KAL_TRUE;

#ifdef MS_EINT_SUPPORT
		  acc_sensor_init_slope_no_motion();   // 动静查询方式
		  bma222_interrupt_init();	// 碰撞中断  			 
#else
   		acc_sensor_init_with_interrupt();
#endif 		 
	     

	}
    else   //第一次ID 读失败后反复读一次
    {
         bma220_init(&t_bma220);
         bma222_init(&t_bma222);  
         
             if(0xdd == t_bma220.chip_id)
    	     {
                is_init_3d = KAL_TRUE;
                acc_sensor_init_with_interrupt_bma220();
    	    }
    	    else if(0xf8 == t_bma222.chip_id)
    	    {
                 is_init_3d = KAL_TRUE;

#ifdef MS_EINT_SUPPORT
		  acc_sensor_init_slope_no_motion();   // 动静查询方式
		  bma222_interrupt_init();	// 碰撞中断 			 
#else
   		acc_sensor_init_with_interrupt();
#endif

    	    }
            TRACE_P_3D("second t_bma222.chip_id=0x%x,t_bma220.chip_id=0x%x", t_bma222.chip_id,t_bma220.chip_id);
	     RJ_GPS_log("second t_bma222.chip_id=0x%x,t_bma220.chip_id=0x%x",t_bma222.chip_id,t_bma220.chip_id);
    }    
#endif

}


void acc_sensor_init_slope_no_motion(void)
{ 

    acc_sensor_pwr_up();
    acc_sensor_interrput_disable();

    bma222_set_range(2);
    bma222_set_bandwidth(bma222_BW_125HZ); 

 //  gps_start_timer(RJ_3D_TIMER, 1000, bosch_motion_sensor_eint_xyz, 0);
    kal_set_timer(motion_3d_timerid, (kal_timer_func_ptr)bosch_motion_sensor_eint_xyz, NULL, 
   	motion_3d_delay_time,	0);  
}
void acc_sensor_init_with_interrupt(void)
{ 

    acc_sensor_pwr_up();
    acc_sensor_interrput_disable();

    bma222_set_range(0);
    bma222_set_bandwidth(bma222_BW_125HZ); 

 //  gps_start_timer(RJ_3D_TIMER, 1000, bosch_motion_sensor_eint_xyz, 0);
    kal_set_timer(motion_3d_timerid, (kal_timer_func_ptr)bosch_motion_sensor_eint_xyz, NULL, 
   	motion_3d_delay_time,	0);


   
}

//This function only for MTK platfrom compiler
void acc_sensor_get_6bytes(kal_uint16 *accl_x,kal_uint16 *accl_y,kal_uint16 *accl_z)
{
   bma222acc_t accl;
   
   bma222_read_accel_xyz(&accl);

   *accl_x=accl.x;
   *accl_y=accl.y;
   *accl_z=accl.z;
}

MotionSensor_custom_data_struct *ms_get_data(void)
{
   return (&ms_custom_data_def);
}
MotionSensor_customize_function_struct *ms_GetFunc(void)
{
   return (&ms_custom_func);
}

#endif

