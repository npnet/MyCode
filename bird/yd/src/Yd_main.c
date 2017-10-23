/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   The main module dealing with YD
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/

#include "Bird_app.h"
#include "Yd_main.h"
#include "Bird_gpscell.h"
#include "Yd_datadeal.h"
#include "Bird_socket.h"
#include "Bird_std.h"
#include "Bird_task.h"
#include  "Rj_DW02.h" // add by lqy
#include "mcp2515.h"  // add by lqy for can control
#include "nmeap.h"    // add by lqy
#include "Yd_base.h"

#ifdef BIRD_ECU_MMI_SUPPORT
#include "Yd_uart_main.h"
#endif

/*
变量定义
*/
//针对YD业务需要保存的数据
YD_TK001_COMMAND_Info yd_tk001_info;
YD_TK002_COMMAND_Info yd_tk002_info;
#ifdef BIRD_BT_SUPPORT    
YD_TK003_COMMAND_Info yd_tk003_info;
#endif
YD_TK005_COMMAND_Info yd_tk005_info;

ALARM_msg_param alarm_msg[TK001_ALARM_ENUM_max];

U8 rj_gps_stage = BIRD_STATUS_NULL;/* 主逻辑运行状态标识 */
U8 g_n_deepsleepcount = 0;
U8 g_n_heart_send_gps = 0;
U8 g_n_gpsrestcount = 0;
U8 g_n_heart_postimes = 0;
U8 g_save_status=0;
U8 g_third_alarm_flag=0;
U8 g_third_alarm_count=0;

applib_time_struct next_alarm_illegal_stream_date = {0};/*上次非法通电报警时间*/

U8 g_factory_test_count=0;

/*本文件使用的函数*/
void Yd_iniVar();
void Yd_main();
void Yd_main_factory();
void Yd_mainini();
void Yd_DinitSocket();
void Yd_DiniSocketDelay();
void Yd_set_all_msg_handler();
void Bird_Tbox_savedata(Send_Info *_send,applib_time_struct dt);
void Bird_Tbox_saveinfo(Send_Info *_send,applib_time_struct dt);
void Yd_savedata();

extern volatile kal_int8 count_judge_gps_app_timer ;
extern volatile kal_uint16 count_system_time;
extern volatile kal_int8 count_judge_gps_send_timer ;
extern U8 g_n_pos_kind;
extern S8 gpsinfo[100];
extern U8 g_acc_close_gps;
extern U8 g_acc_sleep_status;
extern U8 g_n_sleepcount;
extern U8 g_n_sleepposcount;
extern Socket_Send g_socket_send_alarm;

extern kal_uint32 bird_get_bat_channel_voltage(void);
extern double bird_get_adc_channel_voltage();
extern void bird_oilcut_accsleep_handler();
extern void Yd_acc_close_gps();
extern void Yd_oc_heart_handle(void);
extern void Yd_login();
extern void Yd_lc();
extern void Yd_calib();
extern U8* bird_get_can_time();
extern U8* bird_get_can_time_30S(U8 index);
extern void can_data_reset();
extern void can_data_30S_reset();
extern void TB_Soc_get_cantime(applib_time_struct *dt);
extern void Yd_tbox_heart();
extern S8 Bird_comp_filename(kal_wchar *name1,kal_wchar *name2);

#ifndef BIRD_ACC_UNSUPPORT
extern kal_bool get_Acc_onoff2();
#endif

#ifndef BIRD_EXTERNEL_POWER_UNSUPPORT
extern void rj_volt_poweroff(void);
#endif

#ifdef BIRD_ECU_MMI_SUPPORT
extern volatile kal_uint8 ECU_COM_connect_flag;
#endif

extern void acc_sensor_probe();
extern void Lima_Soc_Dinit(void);
extern void DW02_SWITCH_TO_FLIGHT(MMI_BOOL  is_switch);
extern kal_bool get_b_motion();
extern void RJ_reset_GPS_flag(void);

U8 Yd_get_equstatus()
{
	if(rj_gps_stage==BIRD_STATUS_RUN)
		return 1;
	else
		return 0;
}

U8 yd_set_imei_by_server(U8* imei)
{
    S16 error;     
    U8 convert_imei[NVRAM_EF_IMEI_IMEISV_SIZE]={0};
	
        kal_prompt_trace(MOD_SOC, "receive imei: %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
                               imei[0], imei[1], imei[2], imei[3], imei[4], imei[5], imei[6], imei[7],
                               imei[8], imei[9], imei[10], imei[11], imei[12], imei[13], imei[14]);
	
        convert_imei[0] = (imei[0]-0x30)|((imei[1]-0x30)<<4);
        convert_imei[1] = (imei[2]-0x30)|((imei[3]-0x30)<<4);
        convert_imei[2] = (imei[4]-0x30)|((imei[5]-0x30)<<4);
        convert_imei[3] = (imei[6]-0x30)|((imei[7]-0x30)<<4);
        convert_imei[4] = (imei[8]-0x30)|((imei[9]-0x30)<<4);
        convert_imei[5] = (imei[10]-0x30)|((imei[11]-0x30)<<4);
        convert_imei[6] = (imei[12]-0x30)|((imei[13]-0x30)<<4);
        convert_imei[7] = (imei[14]-0x30);
	

        kal_prompt_trace(MOD_SOC, "convert imei: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
                               convert_imei[0], convert_imei[1], convert_imei[2], convert_imei[3], convert_imei[4], convert_imei[5], 
                               convert_imei[6], convert_imei[7], convert_imei[8], convert_imei[9]);

        WriteRecord(NVRAM_EF_IMEI_IMEISV_LID,
                        1,
                        convert_imei,
                        NVRAM_EF_IMEI_IMEISV_SIZE,
                        &error);

        if(NVRAM_WRITE_SUCCESS == error)
        {
            kal_prompt_trace(MOD_SOC,"yd_set_imei_record_from_NV success..");
            return 1;
        }
		
        return 0;
}

int RandomEvent_Random(int _min, int _max)		/*yeyunfeng 20161014*/
{
	int rnd_value = 0;

	rnd_value = rand() % (_max - _min + 1) + _min;
	//rnd_value = (double) rand() / (RAND_MAX + 1) * (_max - _min) + _min;
	return rnd_value;
}

/***************************************************************************
  Function:       Yd_reg
  Description:    The YD service initialization function
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2014-02-12
****************************************************************************/ 
void Yd_reg(){
 	RJ_GPS_log("begin Yd_reg");
		
	yd_tk001_info.nreg = 1; /*测试不用激活*/
	{
		Yd_mainini();
	}
}

/***************************************************************************
  Function:       Yd_mainini
  Description:    The YD service initialization function
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-05-29
****************************************************************************/ 
void Yd_mainini()
{
	U8 nrestresion = 0;

	RJ_GPS_log("begin yd_mainini");
	#ifdef RJ_GPS_APP_AGPS
		RJ_GPS_AGPS_Start();
	#endif		
	Yd_iniVar();//初始化全局变量Bird_user_info… 分配内存，赋值版本号，获取imei，iccid，短信中心号码等

	yd_init_az_alarm_param(5);
	yd_init_move_alarm_param(5);
	yd_init_rollover_alarm_param(5);
	yd_init_vibration_alarm_param(5);
	yd_init_speed_alarm_param(5);
	yd_init_power_off_alarm_param(10);
	yd_init_low_power_alarm_param(10);

	if(!Lima_get_soc_conn_flag())
	{
		Lima_set_soc_init_flag(FALSE); /*将socket状态设置为未初始化*/		 
		Lima_Soc_Dinit();
		Bird_soc_conn();
	}	
	rj_gps_stage = BIRD_STATUS_NULL; 
	Rj_stop_timer(Bird_task_login_Timer); 
	Rj_start_timer(Bird_task_login_Timer, 10*1000, Yd_login,NULL);	

	//Rj_stop_timer(Bird_task_calibtime_Timer); 
	//Rj_start_timer(Bird_task_calibtime_Timer, 10*1000, Yd_calib,NULL);	

	yd_set_gps_open_msg();
	yd_set_gps_close_msg();
	yd_send_gps_open_msg();
	
	nrestresion = Bird_get_reseterr();
	if(nrestresion==1 || nrestresion==5)
	{
		kal_prompt_trace(MOD_SOC," [yd_login] error flag");
	}	   
	acc_sensor_probe();

	#if 0//ndef BIRD_ACC_UNSUPPORT
		LVYUAN2GPS_ALM_INIT();
	#endif

	#ifdef UART_WAKE_SUPPORT
		UART2_EINT_INIT();
	#endif

	#ifdef DW19_LOCK		
		lock_init();
	#endif

}

/***************************************************************************
  Function:       Yd_iniVar
  Description:    The YD service to initialize global variables, such as access to IMEI
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-05-29
****************************************************************************/ 
void Yd_iniVar()
{
	S8 m=0;
	S8 n2 = 0;
	 S8 fjchar[8];
	 S8 dt[1];
        dt[0] = ':';
        RJ_Gps_wrong_info = RJ_GPS_NO_ERROR;

        rj_gps_stage = BIRD_STATUS_NULL;
        memset(&rj_user_info, 0, sizeof(rj_user_info));
        memset(&rj_position, 0, sizeof(rj_position));
        memset(rj_gps_search_mark, 0, sizeof(rj_gps_search_mark));
		
	 bird_ini_lc_tj();
	 bird_ini_gpsposition();
	 bird_ini_heartposition();
	 
        memcpy(rj_user_info.plat, RJ_GPS_PLAT, strlen(RJ_GPS_PLAT));
        memcpy(rj_user_info.ua, RJ_GPS_UA, strlen(RJ_GPS_UA));
        memcpy(rj_user_info.version, RJ_GPS_VERSION, strlen(RJ_GPS_VERSION));
			
        RJ_GPS_Get_IMEI();/* 获取IMEI */
        RJ_GPS_get_sc();  /* 获取短信中心号码*/
	 RJ_GPS_iccid();/*获取ICCID*/	 
        RJ_GPS_Get_IMSI();
	
	 Lima_set_soc_init_flag(FALSE); /*将socket状态设置为未初始化*/		 
	 Lima_Soc_Dinit();
 	 Bird_soc_send_clear(); //清空发送缓冲区
 	 yd_set_gps_open_msg();
	 yd_set_reboot_control_msg();
	 yd_set_save_nv_msg();     
	 yd_set_fly_mode_change_msg(); 
	 
	 memset(gpsinfo,0,sizeof(gpsinfo));   
	 memset(fjchar,0,sizeof(fjchar));
	 fjchar[0] = '0';
	 fjchar[1] = '0';
	 n2 = 0;
        for(m=0;m<12;m++)
        {	
		memcpy(gpsinfo+n2, fjchar, 2);
		n2 = n2 + 2;
		memcpy(gpsinfo+n2, fjchar, 2);
		n2 = n2 + 2;
		if(m<11)
		{
			memcpy(gpsinfo+n2, (S8 *)dt, 1);
			n2 = n2 + 1;		
		}	
       }
	//kal_prompt_trace( MOD_SOC, "RJ_GPS_Callback %s", gpsinfo);	
}


void Yd_zc_res(U8 *flag)
{/*注册*/
	U8 nflag = 0;	
	nflag = atoi(flag);
	kal_prompt_trace(MOD_SOC," Yd_zc_res %d ",nflag);
	//zc res
	//zc sucess
	if(nflag==1){
		Rj_stop_timer(Bird_task_login_Timer); 
             Rj_start_timer(Bird_task_login_Timer, RJ_GPS_APP_5S, Yd_login,NULL);
		Yd_sendregres(1);
		bird_set_zctimes();//注册次数
		yd_tk001_set_record_from_NV();	
	}
	/*
	else
	{
		Yd_sendregres(nflag);
	}
	*/
}
void Yd_getdw_res(U8 *flag,U8 *add)
{
	U8 nflag = 0;	
	nflag = atoi(flag);
	kal_prompt_trace(MOD_SOC," Yd_getdw_res %d,%s ",nflag,add);
	//zc res
	//zc sucess
	if(nflag==1){
		Yd_sendgetdwres((S8 *)add);
		bird_set_dwtimes();
		yd_tk001_set_record_from_NV();	
	}
}

void Yd_alarmlow_rs()
{
	if(bird_get_normal_sleep_state()==1)
	{
		if(rj_gps_stage == BIRD_STATUS_SLEEP)
		{
			rj_gps_stage = BIRD_STATUS_SLEEP;
			Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);
			Rj_start_timer(Bird_task_sleep_dinisocket_Timer, RJ_GPS_APP_1M*2, Yd_DinitSocket,NULL); /*2分钟之后,反初始化socket*/
		}
	}
}

U8* yd_get_model()
{
     kal_prompt_trace(MOD_SOC,"yd_get_model"); 

     return (U8*)YD_TK001_MODEL;
}

U8* yd_get_T37_model()
{
     kal_prompt_trace(MOD_SOC,"yd_get_T37_model"); 

     return (U8*)YD_TK001_T37_BD01;
}

U8 yd_get_third_party_support()
{ 
	kal_prompt_trace(MOD_SOC,"yd_get_third_party_support"); 

#ifdef BIRD_ECU_MMI_SUPPORT
	return 1;
#else
	return 0;
#endif
}

#ifdef BIRD_BT_SUPPORT
extern kal_uint8 mac_data[6];
U8* yd_save_bt_mac()
{
     U8 bt_mac_str[20];
     U8 i=0,j=0;
	 
     memset(bt_mac_str,0,sizeof(bt_mac_str));
	 
     for (j=0;j<6;i++)
     {
     	    if((i+1)%3==0)
     	    {
     	        bt_mac_str[i]=':';
     	    }
     	    else
     	    {

     	    	if(((mac_data[j]>>4)>=0x00)&&((mac_data[j]>>4)<=0x09))
     	    	    bt_mac_str[i]=(mac_data[j]>>4)+0x30;
     	    	else
     	    	    bt_mac_str[i]=(mac_data[j]>>4)+0x37;
     	    	i++;
     	    	if(((mac_data[j]&0x0F)>=0x00)&&((mac_data[j]&0x0F)<=0x09))
     	    	    bt_mac_str[i]=(mac_data[j]&0x0F)+0x30;
     	    	else
     	    	    bt_mac_str[i]=(mac_data[j]&0x0F)+0x37;	
     	    	j++;
     	    }
     }

     bird_set_bt_mac(bt_mac_str);
     yd_tk003_set_record_from_NV();
	 
     kal_prompt_trace(MOD_SOC,"yd_save_bt_mac %x %x %x %x %x %x",mac_data[0],mac_data[1],mac_data[2],mac_data[3],mac_data[4],mac_data[5]); 
     kal_prompt_trace(MOD_SOC,"yd_save_bt_mac %s %d",bt_mac_str,strlen(bt_mac_str)); 

}
#endif

applib_time_struct alarm_dt[30]={0};

void yd_save_alarm()
{
	FS_HANDLE tboxdata_handle = -1;
	FS_HANDLE tboxinfo_handle = -1;

	FS_DOSDirEntry file_info;
	kal_wchar file_name[BIRD_FILE_PATH_LEN];
	kal_wchar  file[BIRD_FILE_PATH_LEN];
	char file_name_str[BIRD_FILE_PATH_LEN];
	U8 time[30]={0};
	U32  time_len;
	U32  buflen;
	U32  len;
       U8 i=0;
       U8 last=0;
	   
	//tbox data
	/*
	memset(file, 0, sizeof(file));
	kal_wsprintf(file,BIRD_TBOXDATA_PATH,(S16)MMI_CARD_DRV, alarm_dt[0].nYear, alarm_dt[0].nMonth, alarm_dt[0].nDay, alarm_dt[0].nHour);

	memset(file_name_str, 0, sizeof(file_name_str));
	mmi_wcs_to_asc(file_name_str, file);

	kal_prompt_trace(MOD_SOC,"yd_save_alarm data %d %d %d %d %d %d",alarm_dt[0].nYear, alarm_dt[0].nMonth, alarm_dt[0].nDay,
		alarm_dt[0].nHour,alarm_dt[0].nMin,alarm_dt[0].nSec); 

	tboxdata_handle = FS_Open(file, FS_CREATE | FS_READ_WRITE);
	kal_prompt_trace(MOD_SOC,"yd_save_alarm data_handle=%d %s",tboxdata_handle,file_name_str); 
	
	if(tboxdata_handle >= FS_NO_ERROR)
	{
	    FS_GetFileSize(tboxdata_handle,&buflen);
	    if(buflen>=0)
	    {
		    FS_Seek(tboxdata_handle, buflen, FS_FILE_BEGIN);
		    for (i=0;i<30;i++)
		    {
		        kal_prompt_trace(MOD_SOC,"yd_save_alarm data write %d %d %d %d %d %d",alarm_dt[i].nYear, alarm_dt[i].nMonth, alarm_dt[i].nDay,
					alarm_dt[i].nHour,alarm_dt[i].nMin,alarm_dt[i].nSec); 

		        if((alarm_dt[i].nYear!=0)&&(alarm_dt[i].nMonth!=0)&&(alarm_dt[i].nDay!=0)&&(alarm_dt[i].nHour==alarm_dt[0].nHour))
		        {
		            FS_Write(tboxdata_handle, g_socket_send_alarm.send_info[i].send_buf,g_socket_send_alarm.send_info[i].buf_len, &len);
		            last++;
		        }
		    }
		    FS_Close(tboxdata_handle);
	    }
	    else{
		    kal_prompt_trace(MOD_SOC,"yd_save_alarm data buflen<0");
		    FS_Close(tboxdata_handle);
		    return;
	    }

	}
       else
       {
           kal_prompt_trace(MOD_SOC,"yd_save_alarm data open fail"); 
           FS_Close(tboxdata_handle);
           return;
       }

       if(last<30)
       {
           if((alarm_dt[last].nYear!=0)&&(alarm_dt[last].nMonth!=0)&&(alarm_dt[last].nDay!=0)&&(alarm_dt[last].nHour!=alarm_dt[0].nHour))
           {
           memset(file, 0, sizeof(file));
           kal_wsprintf(file,BIRD_TBOXDATA_PATH,(S16)MMI_CARD_DRV, alarm_dt[last].nYear, alarm_dt[last].nMonth, alarm_dt[last].nDay, alarm_dt[last].nHour);

           memset(file_name_str, 0, sizeof(file_name_str));
           mmi_wcs_to_asc(file_name_str, file);

           kal_prompt_trace(MOD_SOC,"yd_save_alarm2 data %d %d %d %d %d %d",alarm_dt[last].nYear, alarm_dt[last].nMonth, alarm_dt[last].nDay,
		alarm_dt[last].nHour,alarm_dt[last].nMin,alarm_dt[last].nSec); 

           tboxdata_handle = FS_Open(file, FS_CREATE | FS_READ_WRITE);
           kal_prompt_trace(MOD_SOC,"yd_save_alarm2 data_handle=%d %s",tboxdata_handle,file_name_str); 
	
           if(tboxdata_handle >= FS_NO_ERROR)
           {
	    FS_GetFileSize(tboxdata_handle,&buflen);
	    if(buflen>=0)
	    {
		    FS_Seek(tboxdata_handle, buflen, FS_FILE_BEGIN);
		    for (i=last;i<30;i++)
		    {
		        kal_prompt_trace(MOD_SOC,"yd_save_alarm2 data write %d %d %d %d %d %d",alarm_dt[i].nYear, alarm_dt[i].nMonth, alarm_dt[i].nDay,
					alarm_dt[i].nHour,alarm_dt[i].nMin,alarm_dt[i].nSec); 

		        if((alarm_dt[i].nYear!=0)&&(alarm_dt[i].nMonth!=0)&&(alarm_dt[i].nDay!=0)&&(alarm_dt[i].nHour==alarm_dt[last].nHour))
		        {
		            FS_Write(tboxdata_handle, g_socket_send_alarm.send_info[i].send_buf,g_socket_send_alarm.send_info[i].buf_len, &len);
		        }
		    }
		    FS_Close(tboxdata_handle);
           }
	    else{
		    kal_prompt_trace(MOD_SOC,"yd_save_alarm2 data buflen<0");
		    FS_Close(tboxdata_handle);
		    return;
           }

           }
           else
           {
           kal_prompt_trace(MOD_SOC,"yd_save_alarm2 data open fail"); 
           FS_Close(tboxdata_handle);
           return;
           }  
           }
       }
       */
       last=0;
	   
	//tbox info
	memset(file_name, 0, sizeof(file_name));
	kal_wsprintf(file_name,BIRD_TBOXINFO_PATH,(S16)MMI_CARD_DRV,  alarm_dt[0].nYear, alarm_dt[0].nMonth, alarm_dt[0].nDay, alarm_dt[0].nHour);
	memset(file_name_str, 0,sizeof(file_name_str));
	mmi_wcs_to_asc(file_name_str, file_name);

	tboxinfo_handle = FS_Open(file_name, FS_CREATE | FS_READ_WRITE);
	kal_prompt_trace(MOD_SOC,"yd_save_alarm info handle=%d %s",tboxinfo_handle,file_name_str); 
	
	if(tboxinfo_handle >= FS_NO_ERROR)
	{
		FS_GetFileSize(tboxinfo_handle,&buflen);
		if(buflen>=0)
		{
		    FS_Seek(tboxinfo_handle, buflen, FS_FILE_BEGIN);
		    for (i=0;i<30;i++)
		    {
		        kal_prompt_trace(MOD_SOC,"yd_save_alarm info write %d %d %d %d %d %d",alarm_dt[i].nYear, alarm_dt[i].nMonth, alarm_dt[i].nDay,
					alarm_dt[i].nHour,alarm_dt[i].nMin,alarm_dt[i].nSec); 
	 	        if((alarm_dt[i].nYear!=0)&&(alarm_dt[i].nMonth!=0)&&(alarm_dt[i].nDay!=0)&&(alarm_dt[i].nHour==alarm_dt[0].nHour))
	 	        {
				Bird_infoformat(&alarm_dt[i],time,&time_len);
				FS_Write(tboxinfo_handle, time,time_len, &len);
				FS_Write(tboxinfo_handle, "9",strlen("9"), &len);
                            last++;
				kal_prompt_trace(MOD_SOC,"yd_save_alarm info %d,%d,%s",buflen,time_len,time);
	 	        }				
		    }
		    FS_Close(tboxinfo_handle);
		}
		else
		{
		    kal_prompt_trace(MOD_SOC,"yd_save_alarm info buflen<0");
		    FS_Close(tboxinfo_handle);
		}
       }
       else
       {
           kal_prompt_trace(MOD_SOC,"yd_save_alarm info open fail"); 
           FS_Close(tboxinfo_handle);
           return;
       }

       if(last<30) 
       {
           if((alarm_dt[last].nYear!=0)&&(alarm_dt[last].nMonth!=0)&&(alarm_dt[last].nDay!=0)&&(alarm_dt[last].nHour!=alarm_dt[0].nHour))
           {
           memset(file_name, 0, sizeof(file_name));
           kal_wsprintf(file_name,BIRD_TBOXINFO_PATH,(S16)MMI_CARD_DRV, alarm_dt[last].nYear, alarm_dt[last].nMonth, alarm_dt[last].nDay, alarm_dt[last].nHour);

           memset(file_name_str, 0, sizeof(file_name_str));
           mmi_wcs_to_asc(file_name_str, file);

           kal_prompt_trace(MOD_SOC,"yd_save_alarm2 info %d %d %d %d %d %d",alarm_dt[last].nYear, alarm_dt[last].nMonth, alarm_dt[last].nDay,
		alarm_dt[last].nHour,alarm_dt[last].nMin,alarm_dt[last].nSec); 

           tboxinfo_handle = FS_Open(file_name, FS_CREATE | FS_READ_WRITE);
           kal_prompt_trace(MOD_SOC,"yd_save_alarm2 info_handle=%d %s",tboxinfo_handle,file_name_str); 
	
           if(tboxinfo_handle >= FS_NO_ERROR)
           {
	    FS_GetFileSize(tboxinfo_handle,&buflen);
	    if(buflen>=0)
	    {
		    FS_Seek(tboxinfo_handle, buflen, FS_FILE_BEGIN);
		    for (i=last;i<30;i++)
		    {
		        kal_prompt_trace(MOD_SOC,"yd_save_alarm2 info write %d %d %d %d %d %d",alarm_dt[i].nYear, alarm_dt[i].nMonth, alarm_dt[i].nDay,
					alarm_dt[i].nHour,alarm_dt[i].nMin,alarm_dt[i].nSec); 

		        if((alarm_dt[i].nYear!=0)&&(alarm_dt[i].nMonth!=0)&&(alarm_dt[i].nDay!=0)&&(alarm_dt[i].nHour==alarm_dt[last].nHour))
		        {
				Bird_infoformat(&alarm_dt[i],time,&time_len);
				FS_Write(tboxinfo_handle, time,time_len, &len);
				FS_Write(tboxinfo_handle, "9",strlen("9"), &len);
				kal_prompt_trace(MOD_SOC,"yd_save_alarm2 info %d,%d,%s",buflen,time_len,time);
		        }
		    }
		    FS_Close(tboxinfo_handle);
           }
	    else{
		    kal_prompt_trace(MOD_SOC,"yd_save_alarm2 info buflen<0");
		    FS_Close(tboxinfo_handle);
		    return;
           }

           }
           else
           {
           kal_prompt_trace(MOD_SOC,"yd_save_alarm2 info open fail"); 
           FS_Close(tboxinfo_handle);
           return;
           }  
           }
       }
  
	
}

Send_Info g_cur_send={0};	
applib_time_struct g_cur_dt={0};
void Yd_savedata()
{	
       U8 rtn=0;

	memset(&g_cur_send, 0, sizeof(Send_Info));
	memset(&g_cur_dt, 0, sizeof(applib_time_struct));
	rtn=bird_soc_send_tboxrealinfo(&g_cur_send,&g_cur_dt);
	kal_prompt_trace(MOD_SOC," Yd_savedata %d",rtn);
	if(rtn==1)
	{
	    Bird_Tbox_savedata(&g_cur_send,g_cur_dt);
	    //can_data_reset();
	    //RJ_reset_GPS_flag();
	}

	Rj_stop_timer(Bird_task_savedata_Timer); 
	Rj_start_timer(Bird_task_savedata_Timer, bird_get_savedata_ival(), Yd_savedata,NULL);

}
/***************************************************************************
  Function:    Yd_main   
  Description:    YD business main process function
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-05-29
****************************************************************************/
void Yd_main()
{	
	kal_uint32 ntimer = 30000;
	Send_Info _send;	
	applib_time_struct dt={0};
       U8 rtn=0;
	U16 i=0;
	U8 check_code=0; 

	kal_prompt_trace(MOD_SOC," bg Yd_main %d",g_third_alarm_flag);
	count_judge_gps_app_timer=0;
	StopTimer(BIRD_READ_FILE);

	if(g_third_alarm_flag==0)
	{
	    if(g_cur_dt.nYear!=0)
	    {
	        if(Lima_get_soc_conn_flag())
	        {
	            kal_prompt_trace(MOD_SOC,"Yd_main %d %d %d %d %d %d",g_cur_send.send_buf[24],g_cur_send.send_buf[25],
					g_cur_send.send_buf[26],g_cur_send.send_buf[27],g_cur_send.send_buf[28],g_cur_send.send_buf[29]);
	            if(((g_cur_send.send_buf[24]/16*10+g_cur_send.send_buf[24]%16)!=g_cur_dt.nYear%100)||
					((g_cur_send.send_buf[25]/16*10+g_cur_send.send_buf[24]%16)!=g_cur_dt.nMonth)||
					((g_cur_send.send_buf[26]/16*10+g_cur_send.send_buf[24]%16)!=g_cur_dt.nDay)||
					((g_cur_send.send_buf[27]/16*10+g_cur_send.send_buf[24]%16)!=g_cur_dt.nHour)||
					((g_cur_send.send_buf[28]/16*10+g_cur_send.send_buf[24]%16)!=g_cur_dt.nMin)||
					((g_cur_send.send_buf[29]/16*10+g_cur_send.send_buf[24]%16)!=g_cur_dt.nSec))
	            {
	                g_cur_send.send_buf[24]=(g_cur_dt.nYear%100)/10*16+(g_cur_dt.nYear%100)%10;
	                g_cur_send.send_buf[25]=g_cur_dt.nMonth/10*16+g_cur_dt.nMonth%10;
	                g_cur_send.send_buf[26]=g_cur_dt.nDay/10*16+g_cur_dt.nDay%10;
	                g_cur_send.send_buf[27]=g_cur_dt.nHour/10*16+g_cur_dt.nHour%10;
	                g_cur_send.send_buf[28]=g_cur_dt.nMin/10*16+g_cur_dt.nMin%10;
	                g_cur_send.send_buf[29]=g_cur_dt.nSec/10*16+g_cur_dt.nSec%10;
	                for(i=2;i<g_cur_send.buf_len-1;i++)
				    check_code ^=g_cur_send.send_buf[i];
	                g_cur_send.send_buf[g_cur_send.buf_len-1]=check_code;

	            }
	            Bird_soc_sendbufAdd2(&g_cur_send);
	        }
	        Bird_Tbox_saveinfo(&g_cur_send,g_cur_dt);
	        can_data_reset();
	        RJ_reset_GPS_flag();
	    }
	    ntimer = 1000*bird_get_nmal_main_ival();
	    g_third_alarm_count=0;
	    //非3级报警时开始处理离线    
	    StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);
		
	}
	else
	{    
           //save 30s data
           if(g_third_alarm_count==0)
           {
               memset(alarm_dt,0,sizeof(alarm_dt));
               Bird_soc_send_alarm_clear();
               for (i=0;i<30;i++)
               {
                   memset(&_send, 0, sizeof(Send_Info));
                   rtn=bird_soc_send_resendinfo(&_send,i,&alarm_dt[i]);
                   if(rtn==0)
                   {
                       kal_prompt_trace(MOD_SOC,"Yd_main alarm break %d",i); 
                       break;
                   }       
               }      
           }
	    else
	    {
               if(g_cur_dt.nYear!=0)
               {
                   if(Lima_get_soc_conn_flag())
	                Bird_soc_sendbufAdd2(&g_cur_send);
	            Bird_Tbox_saveinfo(&g_cur_send,g_cur_dt);
               }
	    }
	    g_third_alarm_count++;
	    kal_prompt_trace(MOD_SOC," Yd_main alarm %d",g_third_alarm_count);
	    if(g_third_alarm_count>30)
	    {
	        ntimer = 1000*bird_get_nmal_main_ival();
	        g_third_alarm_flag=0;
	        g_third_alarm_count=0;
	        if(Lima_get_soc_conn_flag())
	            Bird_soc_alarm_to_send();
	        yd_save_alarm();
	        can_data_30S_reset();
	        StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);
	    }
	    else
	    {
	        ntimer = bird_get_alarm_main_ival();
	    }
	}
	Rj_stop_timer(Bird_task_main_Timer); 
	Rj_start_timer(Bird_task_main_Timer, ntimer, Yd_main,NULL);
}

/***************************************************************************
  Function:      Yd_DinitSocket 
  Description:    YD Dini socket  function
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-05-31
****************************************************************************/ 
void Yd_DinitSocket()
{
	kal_prompt_trace(MOD_SOC,"[Yd_DinitSocket]");   
	Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);
	Rj_stop_timer(BIRD_TASK_SOCKET_SEND); //停止发送定时器
	Lima_set_soc_init_flag(FALSE); /*将socket状态设置为未初始化*/
	Lima_Soc_Dinit();
	
}

void Yd_DiniSocketDelay()
{
      Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);
	Rj_start_timer(Bird_task_sleep_dinisocket_Timer, RJ_GPS_APP_1M*4, Yd_DinitSocket,NULL); 
}

void Yd_DiniSocketDelay2()
{
      Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);
	Rj_start_timer(Bird_task_sleep_dinisocket_Timer, 10*1000, Yd_DinitSocket,NULL); 
}

void yd_start_con_socket()
{
		kal_prompt_trace(MOD_SOC,"yd_start_con_socket");   
		Bird_soc_conn();
		Rj_stop_timer(Bird_task_heart_Timer); 
		Rj_start_timer(Bird_task_heart_Timer, 1000, Yd_tbox_heart,NULL);
}
void Yd_disconnect_socket()
{
		Lima_set_soc_init_flag(FALSE);
		Lima_Soc_Dinit();
		Bird_clear_soc_conn();
		Rj_stop_timer(BIRD_TASK_HEART_WAIT_TIMER);
		Rj_stop_timer(Bird_task_heart_Timer); 
		Rj_stop_timer(BIRD_TASK_SOCKET_RECON);
		Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
		StopTimer(BIRD_DISCON_SOCK_TIMER);
		StartTimer(BIRD_DISCON_SOCK_TIMER, RJ_GPS_APP_1M*bird_get_lixian_time(), yd_start_con_socket); 
}
void  yd_upload_call_handler(U8* caller, U16 len)
{
	kal_prompt_trace(MOD_SOC,"yd_upload_call_handler %s",caller);  
	if(bird_get_CALLDISP()==1){
		kal_prompt_trace(MOD_SOC,"yd_upload_call_handler %s",bird_get_utility_number());  
		if(strcmp((const char *)bird_get_utility_number(), (const char *)caller) == 0){
			if(bird_get_CALLLOCK()==1){
				if(yd_tk001_get_defense()==0){
					yd_tk001_post_alarm_key_off(); //SF
				}
				else{
					yd_tk001_post_alarm_key_on(); //cF
				}
			}
			else{
				yd_tk001_wakeup_function();
			}
		}
	}
	else{
		if(bird_get_CALLLOCK()==1){
			if(yd_tk001_get_defense()==0){
				yd_tk001_post_alarm_key_off(); //SF
			}
			else{
				yd_tk001_post_alarm_key_on(); //cF
			}
		}
		else{
			yd_tk001_wakeup_function();
		}
	}
}


void Bird_Tbox_Init(void)
{
	FS_HANDLE tboxdata_handle = -1;
	FS_HANDLE tboxinfo_handle = -1;

	U16 tboxdata_dir[BIRD_FILE_PATH_LEN];
	U16 tboxinfo_dir[BIRD_FILE_PATH_LEN];

	memset(tboxdata_dir, 0, sizeof(tboxdata_dir));
	kal_wsprintf((kal_wchar*) tboxdata_dir,BIRD_TBOXDATA_DIR,(S16)MMI_CARD_DRV);
	tboxdata_handle = FS_CreateDir(tboxdata_dir);
	kal_prompt_trace(MOD_SOC,"Bird_Tbox_Init tboxdata_handle = %d", tboxdata_handle);
	if ((tboxdata_handle < 0) && (tboxdata_handle != FS_FILE_EXISTS)){
		return;
	}

	memset(tboxinfo_dir, 0, sizeof(tboxinfo_dir));
	kal_wsprintf((kal_wchar*) tboxinfo_dir,BIRD_TBOXINFO_DIR,(S16)MMI_CARD_DRV);
	tboxinfo_handle = FS_CreateDir(tboxinfo_dir);
	kal_prompt_trace(MOD_SOC,"Bird_Tbox_Init tboxinfo_handle = %d", tboxinfo_handle);  
	if ((tboxinfo_handle < 0) && (tboxinfo_handle != FS_FILE_EXISTS)){
		return;
	}

}

void Bird_Tbox_savedata(Send_Info *_send,applib_time_struct dt)
{
	FS_HANDLE tboxdata_handle = -1;

	kal_wchar file_name[BIRD_FILE_PATH_LEN];
	kal_wchar  file[BIRD_FILE_PATH_LEN];
	char file_name_str[BIRD_FILE_PATH_LEN];
	U8 time[30]={0};
	U32  time_len;
	U32  buflen;
	U32  len;

	//tbox data
	memset(file, 0, sizeof(file));
	kal_wsprintf(file,BIRD_TBOXDATA_PATH,(S16)MMI_CARD_DRV, dt.nYear, dt.nMonth, dt.nDay, dt.nHour);

	memset(file_name_str, 0, sizeof(file_name_str));
	mmi_wcs_to_asc(file_name_str, file);

	kal_prompt_trace(MOD_VT,"Bird_Tbox_savedata %d %d %d %d %d %d",dt.nYear, dt.nMonth, dt.nDay, dt.nHour,dt.nMin,dt.nSec); 

	tboxdata_handle = FS_Open(file, FS_CREATE | FS_READ_WRITE);
	kal_prompt_trace(MOD_SOC,"Bird_Tbox_savedata handle=%d %s",tboxdata_handle,file_name_str); 
	
	if(tboxdata_handle >= FS_NO_ERROR)
	{
	    //save data content
	    //FS_Close(tboxdata_handle);
	    //Bird_soc_save_file(_send,dt);
	    FS_GetFileSize(tboxdata_handle,&buflen);
	    if(buflen>=0)
	    {
	 	FS_Seek(tboxdata_handle, buflen, FS_FILE_BEGIN);
		kal_prompt_trace(MOD_SOC,"Bird_Tbox_savedata %d %d %d",buflen,_send->buf_len,_send->send_buf[2]);	
		{
			//_send->send_buf[2]=0x03;
			FS_Write(tboxdata_handle, _send->send_buf,_send->buf_len, &len);
			FS_Close(tboxdata_handle);
		}
	    }
	    else{
		kal_prompt_trace(MOD_SOC,"Bird_Tbox_savedata buflen<0");
		FS_Close(tboxdata_handle);
		return;
	    }

	}
       else
       {
           kal_prompt_trace(MOD_SOC,"Bird_Tbox_savedata open fail"); 
           FS_Close(tboxdata_handle);
           return;
       }
	   
}

void Bird_Tbox_saveinfo(Send_Info *_send,applib_time_struct dt)
{
	FS_HANDLE tboxinfo_handle = -1;

	kal_wchar file_name[BIRD_FILE_PATH_LEN];
	kal_wchar  file[BIRD_FILE_PATH_LEN];
	char file_name_str[BIRD_FILE_PATH_LEN];
	U8 time[30]={0};
	U32  time_len;
	U32  buflen;
	U32  len;
	   
	//tbox info
	memset(file, 0, sizeof(file));
	kal_wsprintf(file,BIRD_TBOXINFO_PATH,(S16)MMI_CARD_DRV, dt.nYear, dt.nMonth, dt.nDay, dt.nHour);
	memset(file_name_str, 0,sizeof(file_name_str));
	mmi_wcs_to_asc(file_name_str, file);

	tboxinfo_handle = FS_Open(file, FS_CREATE | FS_READ_WRITE);
	kal_prompt_trace(MOD_SOC,"Bird_Tbox_saveinfo handle=%d %s",tboxinfo_handle,file_name_str); 
	
	if(tboxinfo_handle >= FS_NO_ERROR)
	{
	    //save info content
	    //FS_Close(tboxinfo_handle);
	    //Bird_soc_save_info(dt);
	    FS_GetFileSize(tboxinfo_handle,&buflen);
	    if(buflen>=0)
	    {
	        {
			Bird_infoformat(&dt,time,&time_len);
			FS_Seek(tboxinfo_handle, buflen, FS_FILE_BEGIN);
			FS_Write(tboxinfo_handle, time,time_len, &len);
			FS_Seek(tboxinfo_handle, buflen+time_len, FS_FILE_BEGIN);
			FS_Write(tboxinfo_handle, "9",strlen("9"), &len);

			kal_prompt_trace(MOD_SOC,"Bird_Tbox_saveinfo %d %d %s",buflen,time_len,time);
			FS_Close(tboxinfo_handle);
	        }
	    }
	    else{
		    kal_prompt_trace(MOD_SOC,"Bird_Tbox_saveinfo buflen<0");
		    FS_Close(tboxinfo_handle);
	    }

       }
       else
       {
           kal_prompt_trace(MOD_SOC,"Bird_Tbox_saveinfo open fail"); 
           FS_Close(tboxinfo_handle);
           return;
       }
	
}
#define BIRD_DATE_FORMAT       "rec_2017-05-12"
kal_wchar g_del_name[30]=0;
extern U8 Bird_comp_time(U8 *name1,U8 *name2);
void Bird_Tbox_delete()
{
	FS_HANDLE tboxdata_handle = -1;

	FS_DOSDirEntry file_info;
	char file_date[10][18]={0};
	char tmp_date[18]={0};
	kal_wchar file_hour[24][6]={L"-00",L"-01",L"-02",L"-03",L"-04",L"-05",L"-06",L"-07",L"-08",L"-09",L"-10",L"-11",
		L"-12",L"-13",L"-14",L"-15",L"-16",L"-17",L"-18",L"-19",L"-20",L"-21",L"-22",L"-23"};
	char char_name[BIRD_FILE_PATH_LEN];
	kal_wchar first_name[BIRD_FILE_PATH_LEN];
	kal_wchar  file[BIRD_FILE_PATH_LEN];
	char file_name_str[BIRD_FILE_PATH_LEN];
	kal_wchar tboxdata_file[BIRD_FILE_PATH_LEN];
	kal_wchar tboxdata_filepath[BIRD_FILE_PATH_LEN];
	U32 file_path_len = (BIRD_FILE_PATH_LEN + 1) * 2;
	U8 i=0;
       U8 count=0;
       U8 result =1; 
	   
       {
           memset(tboxdata_file, 0, sizeof(tboxdata_file));
           kal_wsprintf(tboxdata_file,BIRD_TBOXDATA_FILE,(S16)MMI_CARD_DRV);	
		   
           memset(g_del_name, 0,sizeof(g_del_name));
           tboxdata_handle = FS_FindFirst(tboxdata_file, 0, 0, &file_info, g_del_name, file_path_len);
		   
           memset(char_name, 0, sizeof(char_name));
           mmi_wcs_to_asc(char_name,g_del_name);

           memset(file_date, 0, sizeof(file_date));
           memcpy(file_date[0],char_name,14);
           kal_prompt_trace(MOD_SOC,"Bird_Tbox_delete1 %d %s",tboxdata_handle,file_date[0]);
	    count=1;
		
           if (tboxdata_handle >= 0)   
           {
		   memset(first_name, 0, sizeof(first_name));
		   while (FS_FindNext(tboxdata_handle, &file_info, first_name, file_path_len) == FS_NO_ERROR)
		   {
		       memset(char_name, 0, sizeof(char_name));
		       mmi_wcs_to_asc(char_name,first_name);
		       memset(tmp_date, 0, sizeof(tmp_date));
		       memcpy(tmp_date,char_name,14);	   
		       result=1; 
			   
		       for(i=0;i<=count-1;i++)
		       {
		           if(Bird_comp_time(file_date[i], tmp_date)==0)
		           {
		               result=0;
		               break;
		           }
		       }
		       kal_prompt_trace(MOD_SOC,"Bird_Tbox_delete2 %d %s",result,tmp_date);
		       if((result==1)&&(tmp_date[0]!=0))
		       {
		           memcpy(file_date[count],tmp_date,14);
		           kal_prompt_trace(MOD_SOC,"Bird_Tbox_delete3 %d %s",count,file_date[count]);
		           count++;
		       }
			   	
		       if(Bird_comp_filename(first_name,g_del_name)==2)
		       {
		           memset(g_del_name, 0, sizeof(g_del_name));
		           wcscpy(g_del_name,first_name);
		           memset(char_name, 0, sizeof(char_name));
		           mmi_wcs_to_asc(char_name,g_del_name);
		           kal_prompt_trace(MOD_SOC,"Bird_Tbox_delete4 %s",char_name);
		       }

		   };
		   
		   FS_FindClose(tboxdata_handle);
           }
           else
           {
		   kal_prompt_trace(MOD_SOC,"Bird_Tbox_delete nofile");
		   FS_FindClose(tboxdata_handle);	   	
           }
       }
	   
       kal_prompt_trace(MOD_SOC,"Bird_Tbox_delete count %d",count);
	   
       if(count>8)
       {
           for(i=0;i<24;i++)
           {
		   memset(file, 0, sizeof(file));
		   kal_wsprintf(file,BIRD_TBOXDATA_DIR,(S16)MMI_CARD_DRV);
		   mmi_wcscat(file, L"\\");
		   mmi_wcsncat(file, g_del_name,14);
		   mmi_wcscat(file, file_hour[i]);
		   mmi_wcscat(file, L".txt");
		   
		   memset(char_name, 0, sizeof(char_name));
		   mmi_wcs_to_asc(char_name,file);
		   kal_prompt_trace(MOD_SOC,"Bird_Tbox_delete333 %s",char_name);

		   FS_Delete(file);
           }
       }       
       StopTimer(BIRD_DELETE_FILE_TIMER);
       StartTimer(BIRD_DELETE_FILE_TIMER, 60*60*1000, Bird_Tbox_delete);
}
void yd_can_alarm()
{	
	kal_prompt_trace(MOD_SOC," yd_can_alarm");

	g_third_alarm_flag=1;
	g_third_alarm_count=0;
	Rj_stop_timer(Bird_task_main_Timer); 
	Rj_start_timer(Bird_task_main_Timer, 200, Yd_main,NULL);

}

void yd_set_can_alarm_msg()
{
	SetProtocolEventHandler(yd_can_alarm,MSG_ID_CAN_RX_DATA_ALARM);
}
/***************************************************************************
  Function:      Yd_set_all_msg_handler 
  Description:    Function handler is correspondingly arranged message YD service
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-05-29
****************************************************************************/ 
void Yd_set_all_msg_handler()
{
	yd_set_gps_open_msg();
	yd_set_gps_close_msg();
	yd_set_fly_mode_change_msg();
	yd_set_reboot_control_msg();
	yd_set_write_agps_msg();
	//yd_set_run_event_msg();
	yd_set_save_nv_msg();
       yd_set_can_alarm_msg();
	   	
	//yd_set_az_alarm_msg();
	#ifdef BIRD_ECU_MMI_SUPPORT
		Yd_set_uart_msg_handler();
		yd_set_static_event_msg();
	#else
		//yd_set_aux_key_on_msg();
		//yd_set_aux_key_off_msg();
		//yd_set_acc_key_closed_msg();
		
		//yd_set_sos_alarm_msg();
	#endif

	#ifdef BIRD_SWZ
		//yd_set_collision_alarm_msg();
	#endif


	//yd_set_move_alarm_msg();
	//yd_set_speed_alarm_msg();
	//yd_set_out_vol_alarm_msg();
	//yd_set_low_power_alarm_msg();
	
}

