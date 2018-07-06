#ifdef DW02_PROJECT
#include 	"stack_common.h"  
#include 	"stack_msgs.h"
#include 	"app_ltlcom.h"       /* Task message communiction */
#include 	"syscomp_config.h"
#include 	"task_config.h"
#include 	"drv_comm.h"
//#include    "adc.h"
#include    "drvsignals.h"
#include    "eint.h"
#include     "l1audio.h"
#include     "intrctrl.h"

#include "kal_general_types.h"
#include "stack_config.h"
#include "task_main_func.h"
#include "kal_public_api.h"
#include "stack_ltlcom.h"
#include "hisr_config.h"

#include    "dcl.h"

#include "mmi_features.h"
#include "TimerEvents.h"
#include "device.h"
#include "app_datetime.h"
#include "rj_dw02.h"

#ifdef __BIRD_GPS_APP__
#include "bird_gps_framework.h"   
#include "bird_gps_app.h"   
#endif

#include "bird_app.h"
#include "bird_task.h"
#include    "dcl_adc.h"

#include "nmeap.h"    // add by lqy
#include "bird_gpscell.h" // add by lqy

//原20改为10(ms) 去抖动的
kal_bool  b_acc_switch_status = KAL_FALSE;    //   KAL_FALSE   means ACC OFF,  KAL_TRUE means ACC ON
kal_bool KEY_switch_status=KAL_FALSE;    //

kal_bool b_reset = KAL_FALSE;
kal_bool b_outside_volt_disconnect = KAL_FALSE;


//add by lqy for BD
kal_int32 EINT_SW_Debounce_Modify(kal_uint8 eintno, kal_uint8 debounce_time);



extern void TRACE_UART(kal_uint8 * fmt,...);
extern void TRACE_P_3D(kal_uint8 * fmt,...);
extern void TRACE_P_GPS(kal_uint8 * fmt,...);

extern const kal_uint8 ALS_EINT_NO;


extern void RJ_GPS_Send_Uart_msg(char in);


extern const unsigned char ADC_OUTSIDE_VOLTAGE;


void bird_send_message(kal_uint16 message)
{
	ilm_struct       *aux_ilm = NULL;
	DRV_BuildPrimitive(aux_ilm, MOD_AUX, MOD_MMI, message, NULL); 
	msg_send_ext_queue(aux_ilm); 					
}
kal_bool get_BMT_Current_Voltage(DCL_ADC_CHANNEL_TYPE_ENUM ch, kal_uint32 *voltage, double *adc_value)
{
	kal_uint8 adc_channel;
   	DCL_HANDLE adc_handle;
   	ADC_CTRL_GET_PHYSICAL_CHANNEL_T adc_ch;

	if(ch > DCL_VCHARGER_ADC_CHANNEL)
		return KAL_FALSE;  
   
   	adc_handle = DclSADC_Open(DCL_ADC, FLAGS_NONE);
   	if(adc_handle == DCL_HANDLE_INVALID)
   	{
    	ASSERT(0);   
   	}
   	adc_ch.u2AdcName = ch;
   	DclSADC_Control(adc_handle, ADC_CMD_GET_CHANNEL, (DCL_CTRL_DATA_T *)&adc_ch);     

	adc_channel = adc_ch.u1AdcPhyCh;
	*voltage = adc_measureVoltage(adc_channel, adc_value);
    DclSADC_Close(adc_handle);
	return KAL_TRUE;

	

}
/******************************读外部电源电压 begin******************************************/
/*
* FUNCTION
*	   bird_get_adc_channel_voltage
*
* DESCRIPTION                                                           
*   	This function is used to obtain the Battery voltage of specific channel
*
* CALLS  
*
* PARAMETERS
*	   ch: specific channel
*	   voltage: pointer for read the voltage
*	
* RETURNS
*	   KAL_FALSE: invalid channel
*	   KAL_TRUE:  finish the measurement
*
* GLOBALS AFFECTED
*     None
*/  

double bird_get_adc_channel_voltage(void)
{
	double voltage;
	DCL_HANDLE adc_handle;
   	//ADC_CTRL_GET_PHYSICAL_CHANNEL_T adc_get_channel;
	ADC_CTRL_GET_DATA_T adc_data;
	ADC_CTRL_TRANSFORM_INTO_VOLT_T adcTransV;
	
   
   	adc_handle = DclSADC_Open(DCL_ADC, FLAGS_NONE);
   	if(adc_handle == DCL_HANDLE_INVALID)
   	{
		ASSERT(0);   
   	}
	
   	//adc_get_channel.u2AdcName = ch;
   	//DclSADC_Control(adc_handle, ADC_CMD_GET_CHANNEL, (DCL_CTRL_DATA_T *)&adc_get_channel);     

	adc_data.u1Channel = ADC_OUTSIDE_VOLTAGE;    //adc_get_channel.u1AdcPhyCh;
	DclHADC_Control(adc_handle, ADC_CMD_GET_DATA,(DCL_CTRL_DATA_T *)&adc_data);

       //TRACE_P_GPS("u4ADCData=%d",adc_data.u4ADCData);
	adcTransV.u1AdcPhyCh = ADC_OUTSIDE_VOLTAGE;      //adc_get_channel.u1AdcPhyCh;
	adcTransV.d8AdcValue = adc_data.u4ADCData;
	DclSADC_Control(adc_handle, ADC_CMD_TRANSFORM_INTO_VOLT, (DCL_CTRL_DATA_T *)&adcTransV);
       
         if(adcTransV.u4Volt<0)
         {
		adcTransV.u4Volt= 0;
		TRACE_P_GPS("abs");
		kal_prompt_trace( MOD_SOC, "bird_get_adc_channel_voltage adcTransV.u4Volt =%d",adcTransV.u4Volt);	
         }
        TRACE_P_GPS("adcvolt=%d",adcTransV.u4Volt);   //ABS    -2763

	 //voltage = (kal_uint32)adcTransV.u4Volt * 2068/68;
       voltage = (kal_uint32)adcTransV.u4Volt /51 * 561;
        TRACE_P_GPS("voltage=%f",voltage);
	kal_prompt_trace( MOD_SOC, "bird_get_adc_channel_voltage voltage=%d",(int)voltage);

	voltage = voltage / 1000000;
       if(adc_data.u4ADCData<5)
	{
		voltage= 0;
		TRACE_P_GPS("abs");
		kal_prompt_trace( MOD_SOC, "bird_get_adc_channel_voltage adc_data.u4ADCData < 5");
	}
	kal_prompt_trace( MOD_USB, "u4ADCData=%d,voltage=%f",adc_data.u4ADCData,voltage);
	TRACE_P_GPS("u4ADCData=%d,voltage=%f",adc_data.u4ADCData,voltage);

       DclSADC_Close(adc_handle);
	
	return voltage;
}


/******************************读外部电源电压 end******************************************/

void RJ_Delayms(kal_uint32 data)
{
  kal_uint16 i;
  while(data--)
  {
    for(i=0;i<5000;i++){}// 1ms每次循环5000 次  
  }
}
/*
kal_bool get_Acc_onoff()
{
	return b_acc_switch_status;
}*/

/**********************************VCAMD supply power for 433  IC ,VIBR supply power for GPS *********************************************************/
void Rj_DclPMU_Enable(PMU_LDO_BUCK_LIST_ENUM VCamera, PMU_VOLTAGE_ENUM Voltage)
{
	DCL_HANDLE Handle = DCL_HANDLE_INVALID;
	PMU_CTRL_LDO_BUCK_SET_EN PmuEn;
	PMU_CTRL_LDO_BUCK_SET_VOLTAGE PmuVol;

	Handle = DclPMU_Open(DCL_PMU, FLAGS_NONE);

	PmuVol.voltage = Voltage;
	PmuVol.mod = VCamera;
	DclPMU_Control(Handle, LDO_BUCK_SET_VOLTAGE, (DCL_CTRL_DATA_T *)&PmuVol);
	
	PmuEn.enable = DCL_TRUE;
	PmuEn.mod = VCamera;
	DclPMU_Control(Handle, LDO_BUCK_SET_EN, (DCL_CTRL_DATA_T *)&PmuEn);

	DclPMU_Close(Handle);
}

void Rj_DclPMU_Disable(PMU_LDO_BUCK_LIST_ENUM VCamera)
{
	DCL_HANDLE Handle = DCL_HANDLE_INVALID;
	PMU_CTRL_LDO_BUCK_SET_EN PmuEn;

	Handle = DclPMU_Open(DCL_PMU, FLAGS_NONE);

	PmuEn.enable 	= DCL_FALSE;
	PmuEn.mod 		= VCamera;
	DclPMU_Control(Handle, LDO_BUCK_SET_EN, (DCL_CTRL_DATA_T *)&PmuEn);

	DclPMU_Close(Handle);
}

void VMC_SUPPLY_POWER_Enable(void)
{  
	DCL_HANDLE handle = DCL_HANDLE_INVALID;
	PMU_CTRL_LDO_BUCK_SET_EN val;
	PMU_CTRL_LDO_BUCK_SET_VOLTAGE val1;	

       // add by lqy
	kal_prompt_trace( MOD_SOC,"[DRV_Reg1 0xA07001C0]=%x",DRV_Reg(0xA07001C0));
	// add by lqy
       
	val1.voltage = PMU_VOLT_03_300000_V;
	//val1.voltage = PMU_VOLT_03_000000_V;
	val1.mod = VMC;
	handle=DclPMU_Open(DCL_PMU, FLAGS_NONE);
	DclPMU_Control(handle, LDO_BUCK_SET_VOLTAGE, (DCL_CTRL_DATA_T *)&val1);

	//pmic_adpt2_vmc_enable(KAL_TRUE); //replaced by DCL
	val.enable = KAL_TRUE;
	val.mod = VMC;
	DclPMU_Control(handle, LDO_BUCK_SET_EN, (DCL_CTRL_DATA_T *)&val);
	DclPMU_Close(handle);
       //DRV_SetData(0xA07001C0, 0x0030, 0x0030); // add by lqy
       kal_prompt_trace( MOD_SOC,"[DRV_Reg2 0xA07001C0]=%x",DRV_Reg(0xA07001C0));
}

void VMC_SUPPLY_POWER_Disable(void)
{
	DCL_HANDLE Handle = DCL_HANDLE_INVALID;
	PMU_CTRL_LDO_BUCK_SET_EN PmuEn;

	Handle = DclPMU_Open(DCL_PMU, FLAGS_NONE);

	PmuEn.enable 	= DCL_FALSE;
	PmuEn.mod 		= VMC;
	DclPMU_Control(Handle, LDO_BUCK_SET_EN, (DCL_CTRL_DATA_T *)&PmuEn);

	DclPMU_Close(Handle);
}
void Vcamd_Supply(void)   //  433  // or for can
{
		VMC_SUPPLY_POWER_Enable();  //lrf 2013-09-13
}

//add by lqy
void Vcama_Supply(void)
{
      	DCL_HANDLE handle = DCL_HANDLE_INVALID;
	PMU_CTRL_LDO_BUCK_SET_EN val;
	PMU_CTRL_LDO_BUCK_SET_VOLTAGE val1;	
       
	val1.voltage = PMU_VOLT_02_800000_V;
	val1.mod = VCAMA;
	handle=DclPMU_Open(DCL_PMU, FLAGS_NONE);
	DclPMU_Control(handle, LDO_BUCK_SET_VOLTAGE, (DCL_CTRL_DATA_T *)&val1);

	val.enable = KAL_TRUE;
	val.mod = VCAMA;
	DclPMU_Control(handle, LDO_BUCK_SET_EN, (DCL_CTRL_DATA_T *)&val);
	DclPMU_Close(handle);
}


void Vcama_ShutOff(void)
{
       DCL_HANDLE Handle = DCL_HANDLE_INVALID;
	PMU_CTRL_LDO_BUCK_SET_EN PmuEn;

	Handle = DclPMU_Open(DCL_PMU, FLAGS_NONE);

	PmuEn.enable 	= DCL_FALSE;
	PmuEn.mod  = VCAMA;
	DclPMU_Control(Handle, LDO_BUCK_SET_EN, (DCL_CTRL_DATA_T *)&PmuEn);

	DclPMU_Close(Handle);
}

// add by lqy


void Vcamd_ShutOff(void)
{
		VMC_SUPPLY_POWER_Disable();   //by lqy
}

/***********Vib -  GPS********************************************/
void Vvibr_Supply(void)
{
    DCL_HANDLE handle;
    PMU_CTRL_LDO_BUCK_SET_EN val;

    PMU_CTRL_LDO_BUCK_SET_VOLTAGE PmuVol;

	        kal_uint16 temp; // by lqy

	       kal_prompt_trace( MOD_SOC,"[DRV_Reg1 0xA07001B0]=%x",DRV_Reg(0xA07001B0));
	       
		handle=DclPMU_Open(DCL_PMU, FLAGS_NONE);

		PmuVol.voltage = PMU_VOLT_03_000000_V; //by lqy PMU_VOLT_03_300000_V;
		PmuVol.mod = VIBR;
		DclPMU_Control(handle, LDO_BUCK_SET_VOLTAGE, (DCL_CTRL_DATA_T *)&PmuVol);
	
              kal_prompt_trace( MOD_SOC,"[DRV_Reg2 0xA07001B0]=%x",DRV_Reg(0xA07001B0));
			  
		val.enable=DCL_TRUE;
		val.mod=VIBR;
		DclPMU_Control(handle, LDO_BUCK_SET_EN, (DCL_CTRL_DATA_T *)&val);
		DclPMU_Close(handle);
		
		DRV_SetData(0xA07001B0, 0x0030, 0x0030); // add by lqy
		kal_prompt_trace( MOD_SOC,"[DRV_Reg3 0xA07001B0]=%x",DRV_Reg(0xA07001B0));

}
void Vvibr_ShutOff(void)
{
	   DCL_HANDLE handle;
          PMU_CTRL_LDO_BUCK_SET_EN val;
		  
	       val.enable=DCL_FALSE;
		val.mod=VIBR;
		handle=DclPMU_Open(DCL_PMU, FLAGS_NONE);
		DclPMU_Control(handle, LDO_BUCK_SET_EN, (DCL_CTRL_DATA_T *)&val);
		DclPMU_Close(handle);
		kal_prompt_trace( MOD_SOC,"Vvibr_ShutOff DCL_FALSE=%d",DCL_FALSE);

}

 #ifndef BIRD_EXTERNEL_POWER_UNSUPPORT      
/*************************************************************************************************/

/*******************yanchunhai 20130413 断电报警begin*****************************/
/***********************************
//void rj_outside_volt_disconnect(void)  外部电源断开，参数bird_id_data->para_setting_id = 0; 
//void rj_outside_volt_connect(void)  外部电源接上，参数  bird_id_data->para_setting_id = 1;  

*************************/
U8 power_off_flag=0;
U8 volt_read_num=0;
applib_time_struct g_adc_time={0};
extern volatile kal_int8 count_judge_gps_app_timer;  /*Added by Huangwenjian for turning on/off Continuously 12V powersupply lead to system restart 20160722*/
void rj_volt_poweroff(void)
{
      srv_shutdown_normal_start(0);
}
void rj_outside_volt_disconnect(void)
{

     ilm_struct       *rj_ilm = NULL;
      BIRD_id_struct    *bird_id_data = NULL; //lrf
      applib_time_struct currtent_time={0};
      b_outside_volt_disconnect=KAL_TRUE;
      kal_prompt_trace(MOD_SOC,"rj_outside_volt_disconnect power_off_flag=%d",power_off_flag);
/*Added by Huangwenjian for turning on/off Continuously 12V powersupply lead to system restart 20160722 begin*/	  
      count_judge_gps_app_timer = 0;
/*Added by Huangwenjian for turning on/off Continuously 12V powersupply lead to system restart 20160722 end*/
      if(power_off_flag==1)
      {
    	  	    //start power off
    	  	    kal_prompt_trace(MOD_SOC,"rj_outside_volt_disconnect start power off");
    	  	    rj_volt_poweroff();
      	}
      power_off_flag=0;
      bird_id_data = (BIRD_id_struct*)construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);
      bird_id_data->para_setting_id = 0;  

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_RJ_OUTSIDE_VOLT_DISCONNECT, bird_id_data);       
	msg_send_ext_queue(rj_ilm);

	TRACE_P_3D("rj_outside_volt_disconnect");    
}

void Volt_Connect_GetAdc(void)
{
      double voltage;

      voltage=bird_get_adc_channel_voltage();
     

	if((voltage>=16.9)&&(voltage<=20.0))
	{
          volt_read_num++;
	}
	else
	{
          StopTimer(BIRD_VOLT_CONNECT);
          volt_read_num=0;
          return;
	}
       kal_prompt_trace(MOD_SOC,"Volt_Connect_GetAdc volt_read_num=%d",volt_read_num);

	if(volt_read_num>=2)
	{
          StopTimer(BIRD_VOLT_CONNECT);
          volt_read_num=0;            
          power_off_flag=1;
	}
	else
	{
          StartTimer(BIRD_VOLT_CONNECT, 1000, Volt_Connect_GetAdc);
	}

}
void rj_outside_volt_connect(void)
{

      ilm_struct       *rj_ilm = NULL;
      BIRD_id_struct    *bird_id_data = NULL; //lrf
      double voltage;
	  
      voltage=bird_get_adc_channel_voltage();
      b_outside_volt_disconnect=KAL_FALSE;

      kal_prompt_trace(MOD_SOC,"rj_outside_volt_connect bird_get_adc_channel_voltage=%d",(S16)bird_get_adc_channel_voltage());

      if((voltage>=16.9)&&(voltage<=20.0))
      {
    	  	StartTimer(BIRD_VOLT_CONNECT, 1000, Volt_Connect_GetAdc);
      }
      bird_id_data = (BIRD_id_struct*)construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);
      bird_id_data->para_setting_id = 1;  

	DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_RJ_OUTSIDE_VOLT_DISCONNECT, bird_id_data);       
	msg_send_ext_queue(rj_ilm);

	TRACE_P_3D("rj_outside_volt_connect");    



}
#else
extern volatile kal_int8 count_judge_gps_app_timer;  /*Added by Huangwenjian for turning on/off Continuously 12V powersupply lead to system restart 20160722*/
void rj_volt_poweroff(void)
{
      srv_shutdown_normal_start(0);
}

void rj_usb_uncharge(void)
{

      ilm_struct       *rj_ilm = NULL;
      BIRD_id_struct    *bird_id_data = NULL; //lrf
      
      kal_prompt_trace(MOD_SOC,"rj_usb_uncharge");
/*Added by Huangwenjian for turning on/off Continuously 12V powersupply lead to system restart 20160722 begin*/	  
      count_judge_gps_app_timer = 0;
/*Added by Huangwenjian for turning on/off Continuously 12V powersupply lead to system restart 20160722 end*/
      bird_id_data = (BIRD_id_struct*)construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);
      bird_id_data->para_setting_id = 0;  

      DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_RJ_OUTSIDE_VOLT_DISCONNECT, bird_id_data);       
      msg_send_ext_queue(rj_ilm);

}


void rj_usb_charging(void)
{

      ilm_struct       *rj_ilm = NULL;
      BIRD_id_struct    *bird_id_data = NULL; //lrf
	  
      kal_prompt_trace(MOD_SOC,"rj_usb_charging");

      bird_id_data = (BIRD_id_struct*)construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);
      bird_id_data->para_setting_id = 1;  

      DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_RJ_OUTSIDE_VOLT_DISCONNECT, bird_id_data);       
      msg_send_ext_queue(rj_ilm);

}

#endif
/************************yanchunhai 20130413 断电报警end*******************************************/



void UART_WAKE_check_timer();

kal_bool get_Acc_onoff2()
{
	return b_acc_switch_status;
}
kal_bool GET_ACC_SWITCH_STATUS(void)
{
        return KEY_switch_status;

}


#ifdef UART_WAKE_SUPPORT
void UART_WAKE_check_timer()
{
      	GPIO_ModeSetup(34, 2);
      	#ifdef BIRD_ECU_SUPPORT
        Bird_SleepEnable();
        #endif
       	kal_prompt_trace(MOD_SOC," UART_WAKE_check_timer *****");						 
}
void UART2_EINT_HISR(void)  //非法通电必须要在设防状态下启动
{
	GPIO_ModeSetup(34, 0);
	if(KEY_switch_status == KAL_FALSE)
		{
		#ifdef BIRD_ECU_SUPPORT
			Bird_SleepDisable();
		#endif
       	kal_prompt_trace(MOD_SOC," UART2_EINT_HISR *****");			
	}
}


void UART2_EINT_INIT(void)
{
	GPIO_ModeSetup(34, 2);	
   	EINT_Registration(17,KAL_FALSE, KAL_FALSE,UART2_EINT_HISR, KAL_TRUE);
   	EINT_Set_Sensitivity(17, EDGE_SENSITIVE);

      TRACE_P_GPS("UART2_EINT_INIT 	UART2_EINT_NO=%d",ALS_EINT_NO);    

}
#endif
/**SIM 卡无效、无网络，重启，  24小时重启一次**/
void rj_reset(void)
{
#if 1
        applib_time_struct current_time;
        applib_dt_get_rtc_time(&current_time);
        kal_prompt_trace( MOD_SOC, "rj_reset %d:%d:%d:%d:%d:%d" ,
        current_time.nYear,  current_time.nMonth, current_time.nDay, current_time.nHour, current_time.nMin, current_time.nSec); 
        l4cuem_power_reset(NORMAL_RESET);
#endif
}

kal_uint16 acc_flag =0;
kal_uint16 count_acc_on_cnt=0;
kal_uint16 count_acc_off_cnt=0;

void Bird_acc()
{
	char gpio54_data;
       static kal_uint8 is_acc_ok=0;
	GPIO_ModeSetup(54, 0);		//
	GPIO_InitIO(0, 54);
	kal_sleep_task(10); 
	gpio54_data = GPIO_ReadIO(54);
	if(gpio54_data==1) // just for debug
	//if(gpio54_data==0)
		{
		count_acc_on_cnt++;
		count_acc_off_cnt = 0;
		if(count_acc_on_cnt>3)
			count_acc_on_cnt = 3;
		}
	else
		{
		count_acc_on_cnt = 0;
		count_acc_off_cnt ++;
		if(count_acc_off_cnt>3)
			count_acc_off_cnt = 3;
		}
	
	if(count_acc_on_cnt==3 &&( (acc_flag == 0) || (acc_flag == 2)))
	{
		b_acc_switch_status =KAL_TRUE;
		KEY_switch_status =KAL_TRUE;
		ACC_SWITCH_ON();	
		acc_flag = 1;
	}
	else if(count_acc_off_cnt==3 &&( (acc_flag == 0) || (acc_flag == 1)))
	{
		b_acc_switch_status =KAL_FALSE;
		KEY_switch_status =KAL_FALSE;			
		ACC_SWITCH_OFF();
		acc_flag = 2;
	}	
	RJ_GPS_StartTimer(BIRD_SOS_STATE_timer, 400, Bird_acc);

}

kal_bool watchdog_state = 0 ; //0: LEVEL_LOW; 1: LEVEL_HIGH
void bird_watchdog_control()
{
	watchdog_state =  !watchdog_state;
	GPIO_ModeSetup(21, 0);		//eint4	
	GPIO_InitIO(1, 21);
	GPIO_WriteIO(watchdog_state,21);	
	//kal_prompt_trace(MOD_SOC,"watchdog_state =%d",watchdog_state);  
	RJ_GPS_StartTimer(BIRD_WDG_timer, 1*1000, bird_watchdog_control); 	
}
#endif

