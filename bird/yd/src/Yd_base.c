/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   YD basic business function module
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/


#include "Bird_app.h"
#include "Yd_base.h"
#include "Yd_main.h"
#include "bma222.h"
#include "l4c_common_enum.h"

U8 alarm_illegal_stream_times = 0;/*非法通电报警次数*/
U8 yd_key_status = 0;/*遥控器按键状态*/
U8 yd_learn_status = 0;/*钥匙学习标志*/
unsigned char get_sim_ye_str[200];
S8 get_sim_ye_num[16];
S8 get_sim_ye_msg[400];
U8 g_rev[17];
//错误原因
U8 g_n_bird_err_resion = 0;
U8 g_n_bird_last_err_resion = 0;
//错误次数
U8 g_n_bird_err_times = 0;

#ifdef __MMI_BT_SUPPORT__
extern void bird_write_blue_name(U8 *bluename);
#endif

/***************************************************************************
  Function:      Bird_set_reseterr 
  Description:    set resetree
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-09-22
****************************************************************************/    
void Bird_set_reseterr(U8 n_err_type)
{
	g_n_bird_err_resion = n_err_type;
	yd_tk001_info.nresresion = n_err_type;
}

/***************************************************************************
  Function:      Bird_get_reseterr 
  Description:    get resetree
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-09-22
****************************************************************************/    
U8 Bird_get_reseterr()
{
	return yd_tk001_info.nresresion;
}

void Bird_set_restimes(U8 n_err_times)
{
	g_n_bird_err_times = n_err_times;
}

U8 Bird_get_restimes()
{
	return g_n_bird_err_times;
}

U8 Bird_get_lastreseterr()
{
	return g_n_bird_last_err_resion; 
}
void Bird_set_lastreseterr()
{
	g_n_bird_last_err_resion = yd_tk001_info.nresresion;
}

U8 Yd_get_restarttimes()
{
	return yd_tk001_info.nrestarttimes;
}

void Yd_set_restarttimes(U8 nrestarttimes)
{
	yd_tk001_info.nrestarttimes=nrestarttimes;
}

/*-------------------------------------------------------*/

U16 bird_get_continue_search_gprs_times( )   
{
   return  yd_tk001_info.continue_search.search_times ;
}

U16 bird_get_continue_search_gprs_interval( )   
{
	if(yd_tk001_info.continue_search.search_interval==0)
	{
		return 15;
	}
	else{
		return  yd_tk001_info.continue_search.search_interval ;
	}
}

U32 bird_get_main_interval_value()
{
    return yd_tk001_info.main_interval ;
}

U8 * bird_get_authorize_number( )
{

	  /*
      U8 i = 0;
	for(i=0;i<11;i++)
	{
  		yd_tk001_info.authorize_number[i]=1+i;
	}
	*/
    kal_prompt_trace(MOD_SOC,"bird_set_authorize_number %s  \n",yd_tk001_info.authorize_number);       
    if(strlen(yd_tk001_info.authorize_number)>10)
        return yd_tk001_info.authorize_number;
    else
        		
        return NULL;
}

void bird_set_continue_search_gprs(U16 argv1 ,U16 argv2 )
{	
	if(argv1!=0)
	{
		yd_tk001_info.continue_search.search_interval = argv1;    
   		 yd_tk001_info.continue_search.search_times = argv2; 
	}
	kal_prompt_trace(MOD_SOC,"bird_set_continue_search_gprs a1,a2 %d,%d",yd_tk001_info.continue_search.search_interval,yd_tk001_info.continue_search.search_times); 

}

void bird_set_authorize_number( U8 *src )
{
    kal_prompt_trace(MOD_SOC,"bird_set_authorize_number %s  \n",src);         
    memset(yd_tk001_info.authorize_number, 0x00, sizeof(yd_tk001_info.authorize_number));    
    strcpy(yd_tk001_info.authorize_number,( const char *)src);
    kal_prompt_trace(MOD_SOC,"bird_set_authorize_number %s  \n",yd_tk001_info.authorize_number);    
}

void bird_set_utility_number( U8 *src )
{
     kal_prompt_trace(MOD_SOC,"bird_set_utility_number %s  \n",src);  
     memset(yd_tk001_info.utility_number, 0x00, sizeof(yd_tk001_info.utility_number));    
     strcpy(yd_tk001_info.utility_number, (const void *)src);
}

void  bird_set_sim_iccid_to_nvram( U8 *src )
{
     kal_prompt_trace(MOD_SOC,"bird_set_sim_iccid_to_nvram %s  \n",src);  
     memset(yd_tk001_info.sim_iccid, 0x00, sizeof(yd_tk001_info.sim_iccid));    
     strcpy(yd_tk001_info.sim_iccid, (const void *)src);
}

U8 * bird_get_admin_psw( void )
{
    if(yd_tk001_info.admin_pw[0] == 0)
        return NULL;
    else
        return yd_tk001_info.admin_pw;
}

void bird_set_utility_pw( U8 *src  )
{
     kal_prompt_trace(MOD_SOC,"bird_set_utility_pw %s  \n",src);      
     memset(yd_tk001_info.sys_password, 0x00, sizeof(yd_tk001_info.sys_password));   
     strcpy(yd_tk001_info.sys_password, (const void *)src);
}

U8 bird_get_trace()
{
	return yd_tk001_info.ntrace;
}

void bird_set_server_address( U8 *src  )
{
     kal_prompt_trace(MOD_SOC,"bird_set_server_address1 %s  \n",src); 
     memset(yd_tk002_info.server_address, 0x00, sizeof(yd_tk002_info.server_address));
     strcpy(yd_tk002_info.server_address, (const char *)src);
     kal_prompt_trace(MOD_SOC,"bird_set_server_address2 %s  \n",yd_tk002_info.server_address); 

}

#ifdef BIRD_BT_SUPPORT    
void bird_set_bt_mac(U8* src)
{
     memset(yd_tk003_info.bt_mac, 0x00, sizeof(yd_tk003_info.bt_mac));
     strcpy(yd_tk003_info.bt_mac, (const char *)src);
	 
     kal_prompt_trace(MOD_SOC,"bird_set_bt_mac2 %s %d",yd_tk003_info.bt_mac,strlen(yd_tk003_info.bt_mac)); 

}
#endif

U8 *  bird_get_sim_iccid(void)
{
    if(strlen(rj_user_info.u_iccid)>15)
        return rj_user_info.u_iccid;
    else
        return NULL;
}

U8 * bird_get_utility_number( )
{
    if(strlen(yd_tk001_info.utility_number)>10)
        return yd_tk001_info.utility_number;
    else
        return NULL;
}

void Bird_set_resttime()
{
	bird_set_test_time_afert(yd_tk001_info.resttime);
	kal_prompt_trace(MOD_SOC,"Bird_set_resttime %s ",yd_tk001_info.resttime);
}

void bird_set_main_interval_value(U32 interval)
{
     kal_prompt_trace(MOD_SOC,"bird_set_main_interval_value %d  \n",interval);  
     if(interval <2)
        interval = 2;
    yd_tk001_info.main_interval = interval;
}

void bird_set_trace_value(U32 interval)
{
     kal_prompt_trace(MOD_SOC,"bird_set_main_interval_value %d  \n",interval);  
     if(interval >=1)
        interval = 1;
     else{
	 	interval = 0;
     	}
    yd_tk001_info.ntrace= interval;
}

void bird_set_alarm_delay_value(U32 time)
{
    yd_tk001_info.alarm_delay = time;
}

void bird_set_authorize_number_ext( U8 *src , U8 len )
{
    kal_prompt_trace(MOD_SOC,"bird_set_authorize_number_ext %s  \n",src);   
     memset(yd_tk001_info.authorize_number, 0x00, sizeof(yd_tk001_info.authorize_number));    
     memcpy(yd_tk001_info.authorize_number, (const void *)src, len);
}

void bird_set_utility_pw_ext( U8 *src , U8 len )
{
     kal_prompt_trace(MOD_SOC,"bird_set_utility_pw %s  \n",src);      
     memset(yd_tk001_info.sys_password, 0x00, sizeof(yd_tk001_info.sys_password));       
     memcpy(yd_tk001_info.sys_password, (const void *)src, len);
}

#if defined(BIRD_SWZ)||defined(__BIRD_RUN_EINT__)	
extern int bma222_set_high_g_interrupt_threshold(float threshold);
U8 bird_set_hitl_value(int  hitlvalue)       
{		
	int data;

       kal_prompt_trace(MOD_SOC,"bird_set_hitl_value hitlvalue =  %d",hitlvalue);       
       yd_tk001_info.hitlvalue= hitlvalue;
	
	data = bma222_set_high_g_interrupt_threshold(hitlvalue);
	if (data == -1)
		data = bma222_set_high_g_interrupt_threshold(hitlvalue);
	TRACE_P_3D("bird_set_hitl_value hitlvalue = %d,  return_data = %d ",(int)hitlvalue,data);
	if (data == -1)
		return 0;
	else
		return 1;
}

U8 bird_get_hitlvalue()
{     
    kal_prompt_trace(MOD_SOC,"bird_get_hitlvalue  = %d",yd_tk001_info.hitlvalue);  
    return yd_tk001_info.hitlvalue;
}
U8 bird_get_hitldelaytime()
{     
    kal_prompt_trace(MOD_SOC,"bird_get_hitldelaytime  = %d",yd_tk001_info.hitldelaytime);  
    return yd_tk001_info.hitldelaytime;
}
U8 bird_get_hitl_latch_time()
{     
    kal_prompt_trace(MOD_SOC,"bird_get_hitl_latch_time  = %d",yd_tk001_info.hitllatchtime);  
    return yd_tk001_info.hitllatchtime;
}
#endif

void bird_set_corner_value(U32 value)
{
   kal_prompt_trace(MOD_SOC,"bird_set_corner_value %d  \n",value);       
   if(value>180){
   	value = 179;
   }
    yd_tk001_info.Corner= value;
}

void bird_set_admin_number( U8 *src  ,U8 length)
{
     memset(yd_tk001_info.admin_number, 0x00, sizeof(yd_tk001_info.admin_number));
     memcpy(yd_tk001_info.admin_number, (const char *)src , length);
}

void bird_get_sim_ye( U8 *src  ,U8 length,U8 *rev)
{
     S8 *begin_offset =NULL;	
     S32 len=0;
     S32 len2 = strlen(",");

     memset(g_rev, 0x00, sizeof(g_rev));
     memset(get_sim_ye_str, 0x00, sizeof(get_sim_ye_str));
	 
     memcpy(get_sim_ye_str, (const char *)src , length);
     memcpy(g_rev, (U8 *)rev , 17);	 
	 
     begin_offset=strstr(app_strtolower(get_sim_ye_str),",");  
     len = begin_offset - get_sim_ye_str+len2;
     memcpy(get_sim_ye_msg,get_sim_ye_str,len-len2);	
     memcpy(get_sim_ye_num,(get_sim_ye_str+len),length-len+len2);	
     kal_prompt_trace(MOD_SOC, "bird_get_sim_ye %s", src);  	 
}


void bird_set_illegal_stream_alarm_gprs(U8 flag)
{
   kal_prompt_trace(MOD_SOC,"bird_set_illegal_stream_alarm_gprs %d  \n",flag);      
    if(yd_tk001_info.illegal_stream_alarm != flag)
    {
        yd_tk001_info.illegal_stream_alarm = flag;
        alarm_illegal_stream_times = 0;
    }

}

U32 bird_get_data_to_U32(U8 * addr)
{
        return 256*256*256*(*addr)+256*256*(*(addr+1))+256*(*(addr+2))+*(addr+3);
}


U8 * bird_get_server_address(  )
{
     kal_prompt_trace(MOD_SOC,"bird_get_server_address"); 
     return yd_tk002_info.server_address;
}
#ifdef BIRD_BT_SUPPORT    
U8* bird_get_bt_mac()
{
     kal_prompt_trace(MOD_SOC,"bird_get_bt_mac %s %d",yd_tk003_info.bt_mac,strlen(yd_tk003_info.bt_mac)); 
    if(strlen(yd_tk003_info.bt_mac)==17)
        return yd_tk003_info.bt_mac;
    else
        return NULL;
}
#endif

U8 yd_tk001_get_key()
{
	return yd_key_status;
}

void yd_tk001_set_key(U8 key_status)
{
	yd_key_status = key_status;
}

U8 yd_tk001_get_learn()
{
	return yd_learn_status;
}
void yd_tk001_set_learn(U8 key_learn)
{
	yd_learn_status = key_learn;
}

U8 bird_get_CALLLOCK()
{
	return yd_tk001_info.CALLLOCK;
}

U8 bird_get_CALLDISP()
{
	return yd_tk001_info.CALLDISP;
}

U8 bird_get_SMS()
{
	return yd_tk001_info.SMS;
}

U8 bird_get_VIBCALL()
{
	return yd_tk001_info.VIBCALL;
}

U8* bird_get_CID(  )
{
     kal_prompt_trace(MOD_SOC,"bird_get_CID"); 
     return yd_tk001_info.CID;
}

U8* bird_get_ipadd(){
	return yd_tk001_info.ipadd;
}
void bird_set_CALLLOCK(U8 v)
{
	yd_tk001_info.CALLLOCK=v;
}

void bird_set_CALLDISP(U8 v)
{
	yd_tk001_info.CALLDISP=v;
}

void bird_set_SMS(U8 v)
{
	yd_tk001_info.SMS=v;
}

void bird_set_VIBCALL(U8 v)
{
	yd_tk001_info.VIBCALL=v;
}

void bird_set_CID( U8 *src )
{
    kal_prompt_trace(MOD_SOC,"bird_set_CID %s",src);         
    memset(yd_tk001_info.CID, 0x00, sizeof(yd_tk001_info.CID));   
    if(strlen(( const char *)src)>8)
    {
    	  return;
    }
    strcpy(yd_tk001_info.CID,( const char *)src);

}

void bird_set_ipadd( U8 *src )
{
    kal_prompt_trace(MOD_SOC,"bird_set_ipadd %s",src);         
    memset(yd_tk001_info.ipadd, 0x00, sizeof(yd_tk001_info.ipadd));   
    strcpy(yd_tk001_info.ipadd,( const char *)src);
    kal_prompt_trace(MOD_SOC,"bird_set_ipadd %s",yd_tk001_info.ipadd);  
}


U32 bird_get_alarm_delay_value()
{
    return yd_tk001_info.alarm_delay;
}


U8 * bird_get_utility_pw( )
{
    if(yd_tk001_info.sys_password[0] !=0)
        return yd_tk001_info.sys_password;
    else
        return NULL;
}

U8 bird_get_zctimes(){
	return yd_tk001_info.nzctimes;
}

U8 bird_get_dwtimes(){
	return yd_tk001_info.ndwtimes;
}

void bird_set_zctimes(){
	yd_tk001_info.nzctimes = yd_tk001_info.nzctimes+1;
}

void bird_set_dwtimes(){
	yd_tk001_info.ndwtimes = yd_tk001_info.ndwtimes+1;
}

U32 bird_get_corner_value()
{
   kal_prompt_trace(MOD_SOC,"bird_get_corner_value  \n");       
    return yd_tk001_info.Corner;
}

U8 * bird_get_admin_number( void )
{
    if(yd_tk001_info.admin_number[0]== 0)
        return NULL;
    else
        return yd_tk001_info.admin_number;
}
U8 bird_get_illegal_stream_alarm_gprs()
{
    kal_prompt_trace(MOD_SOC,"bird_get_illegal_stream_alarm_gprs flag: %d",yd_tk001_info.illegal_stream_alarm);
    return  yd_tk001_info.illegal_stream_alarm ;
}
void bird_set_motor_state(U8 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_motor_state: %d",flag);
	yd_tk001_info.motor_state=flag;
}
U8 bird_get_motor_state(){
	kal_prompt_trace(MOD_SOC,"bird_get_motor_state: %d",yd_tk001_info.motor_state);
	return yd_tk001_info.motor_state;
}
void bird_set_lock_state(U8 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_lock_state: %d",flag);
	yd_tk001_info.lock_state=flag;
}
U8 bird_get_lock_state(){
	kal_prompt_trace(MOD_SOC,"bird_get_lock_state: %d",yd_tk001_info.lock_state);
	return yd_tk001_info.lock_state;
}
void bird_set_equmode(U8 flag)
{
	yd_tk001_info.equmode=flag;
}
U8 bird_get_equmode()
{/*用以判断报警是否存在报警间隔机制*/
	kal_prompt_trace(MOD_SOC,"bird_get_equmode =  %d  \n",yd_tk001_info.equmode);       
	return yd_tk001_info.equmode;/*设备的模式，默认 0为正常模式，1 为演示模式，2 为调试模式*/
}

void bird_set_limit_speed_value(U8 value)
{
     kal_prompt_trace(MOD_SOC,"bird_set_limit_speed_value =  %d  \n",value);       
     yd_tk001_info.limit_speed= value;
}

U8 bird_get_limit_speed_value()
{     
    kal_prompt_trace(MOD_SOC,"bird_get_limit_speed_value  = %d  \n",yd_tk001_info.limit_speed);  
    return yd_tk001_info.limit_speed;
}

void bird_set_agps_address(U8 *src)
{
     kal_prompt_trace(MOD_SOC,"bird_set_agps_address1 %s  \n",src); 
     memset(yd_tk001_info.agps_address, 0x00, sizeof(yd_tk001_info.agps_address));
     strcpy(yd_tk001_info.agps_address, (const char *)src);
     kal_prompt_trace(MOD_SOC,"bird_set_agps_address2 %s  \n",yd_tk001_info.agps_address); 

}
void bird_set_abd_address(U8 *src)
{
     kal_prompt_trace(MOD_SOC,"bird_set_abd_address1 %s  \n",src); 
     memset(yd_tk001_info.abd_address, 0x00, sizeof(yd_tk001_info.abd_address));
     strcpy(yd_tk001_info.abd_address, (const char *)src);
     kal_prompt_trace(MOD_SOC,"bird_set_abd_address2 %s  \n",yd_tk001_info.abd_address); 

}
U8* bird_get_agps_address(void)
{
     return yd_tk001_info.agps_address;

}
U8* bird_get_abd_address(void)
{
     return yd_tk001_info.abd_address;

}

void bird_set_uart_bat_interval(U8 flag)
{
	yd_tk001_info.uart_bat_interval=flag;
}
U8 bird_get_uart_bat_interval(){
	return yd_tk001_info.uart_bat_interval;
}

void bird_set_car_state(U8 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_car_state: %d",flag);
	yd_tk001_info.car_status=flag;
}
U8 bird_get_car_state(){
	kal_prompt_trace(MOD_SOC,"bird_get_car_state: %d",yd_tk001_info.car_status);
	return yd_tk001_info.car_status;
}


void bird_set_savedata_ival(U16 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_savedata_ival: %d",flag);
	yd_tk005_info.savedata_ival=flag;
}
U16 bird_get_savedata_ival(){
	return yd_tk005_info.savedata_ival;
}
void bird_set_nmal_main_ival(U16 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_nmal_main_ival: %d",flag);
	yd_tk005_info.nmal_main_ival=flag;
}
U16 bird_get_nmal_main_ival(){
	return yd_tk005_info.nmal_main_ival;
}
void bird_set_alarm_main_ival(U8 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_alarm_main_ival: %d",flag);
	yd_tk005_info.alarm_main_ival=flag;
}
U16 bird_get_alarm_main_ival(){
	return yd_tk005_info.alarm_main_ival;
}
void bird_set_heart_ival(U8 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_heart_ival: %d",flag);
	yd_tk005_info.heart_ival=flag;
}
U8 bird_get_heart_ival(){
	return yd_tk005_info.heart_ival;
}
void bird_set_ter_res_time(U8 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_ter_res_time: %d",flag);
	yd_tk005_info.ter_res_time=flag;
}
U8 bird_get_ter_res_time(){
	return yd_tk005_info.ter_res_time;
}
void bird_set_ser_res_time(U8 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_ser_res_time: %d",flag);
	yd_tk005_info.ser_res_time=flag;
}
U8 bird_get_ser_res_time(){
	return yd_tk005_info.ser_res_time;
}
void bird_set_conn_interval(U8 flag)
{
	kal_prompt_trace(MOD_SOC,"bird_set_conn_interval: %d",flag);
	yd_tk005_info.conn_interval=flag;
}
U8 bird_get_conn_interval(){
	return yd_tk005_info.conn_interval;
}

void RJ_GPS_Get_ID(void)
{
	memset(rj_user_info.u_id,0x00,sizeof(rj_user_info.u_id));
	memcpy((void *)(rj_user_info.u_id), (const void *)YD_TK001_SW_FLAG, strlen((const char *)YD_TK001_SW_FLAG));
	memcpy((void *)(rj_user_info.u_id+strlen((const char *)YD_TK001_SW_FLAG)), (const void *)(rj_user_info.u_imei),15);//modify by wtl 160616
	//get id
	kal_prompt_trace(MOD_SOC,"id . %s \n",rj_user_info.u_id);   
}

/*-------------------------------------------------------
                      获取IMEI
-------------------------------------------------------*/
#ifdef BIRD_BT_SUPPORT
		extern void send_device_num_to_BT(void);
#endif	
void RJ_GPS_get_imei_rsp(void * msg_ptr)
{
        mmi_nw_get_imei_rsp_struct  * imei_rsp = NULL; //lrf

        mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_NW_GET_IMEI_RSP,RJ_GPS_get_imei_rsp);

        /* convert the msg_ptr to imei_rep type */
        imei_rsp = (mmi_nw_get_imei_rsp_struct *)msg_ptr;

        /* check the response is correct or not */
        if (KAL_TRUE == imei_rsp->result)
        {
                memset(rj_user_info.u_imei, 0, sizeof(rj_user_info.u_imei));
                /* get the IMEI number successful */
                memcpy(rj_user_info.u_imei, (S8*) imei_rsp->imei, sizeof(imei_rsp->imei) - 1);
		  RJ_GPS_Get_ID();	
#ifdef BIRD_BT_SUPPORT
		send_device_num_to_BT();
		kal_prompt_trace(MOD_SOC,"RJ_GPS_get_imei_rsp  send_device_num_to_BT");   
#endif	  
#ifdef __MMI_BT_SUPPORT__
		  bird_write_blue_name(rj_user_info.u_id);
#endif
        }
        else
        {
                /* if it is aborted when get the imei, appliction just do nothing */
        }
}

U8 *RJ_GPS_get_imei_array(void)
{
    if(rj_user_info.u_imei[0] == 0)
        return NULL;
    else
        return rj_user_info.u_imei;
}

void RJ_GPS_send_imei_message(module_type MOD_DEST)
{
        ilm_struct  * ilm_ptr = NULL;

        /* set imei response handler */
        mmi_frm_set_protocol_event_handler(MSG_ID_MMI_NW_GET_IMEI_RSP, RJ_GPS_get_imei_rsp, MMI_TRUE);

        /* allocate the ilm struct memory */
        ilm_ptr = allocate_ilm(MOD_MMI);

        /* fill the member of the ilm struct */
        ilm_ptr->msg_id = MSG_ID_MMI_NW_GET_IMEI_REQ;
        ilm_ptr->local_para_ptr = (local_para_struct *) NULL;
        ilm_ptr->peer_buff_ptr = (peer_buff_struct *) NULL;

        /* send the message */
        SEND_ILM(MOD_MMI, MOD_DEST, MMI_L4C_SAP, ilm_ptr);
}

void RJ_GPS_Get_IMEI(void)
{
        mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_NW_GET_IMEI_RSP,RJ_GPS_get_imei_rsp);
        RJ_GPS_send_imei_message(MOD_L4C); 
}


void RJ_GPS_get_imsi_rsp(void *inMsg)
{
      U8	imsi_str[17];
      mmi_smu_get_imsi_rsp_struct *local_data_p = (mmi_smu_get_imsi_rsp_struct*) inMsg;
	if (local_data_p->result == 1)
	{
		memset(imsi_str,0x00,sizeof(imsi_str));
		memcpy(imsi_str, local_data_p->imsi + 1, sizeof(imsi_str) - 1);
		memcpy(rj_user_info.u_imsi,imsi_str,sizeof(imsi_str) - 1);     //将SIM存放在数组中  
              kal_prompt_trace(MOD_SOC,"imsi. %s \n",imsi_str);   
			  
        	
	}
}



void RJ_GPS_send_imsi_message(module_type MOD_DEST)
{
    ilm_struct *ilm_ptr = NULL;

    mmi_frm_set_protocol_event_handler(MSG_ID_MMI_SMU_GET_IMSI_RSP, RJ_GPS_get_imsi_rsp, MMI_TRUE);

    ilm_ptr = allocate_ilm(MOD_MMI);
    ilm_ptr->msg_id         = MSG_ID_MMI_SMU_GET_IMSI_REQ;
    ilm_ptr->local_para_ptr = (local_para_struct *) NULL;
    ilm_ptr->peer_buff_ptr  = (peer_buff_struct *) NULL;
    
    SEND_ILM(MOD_MMI, MOD_DEST, MMI_L4C_SAP, ilm_ptr);

}

void RJ_GPS_Get_IMSI(void)
{
      mmi_frm_clear_protocol_event_handler(MSG_ID_MMI_SMU_GET_IMSI_RSP,RJ_GPS_get_imsi_rsp);
      RJ_GPS_send_imsi_message(MOD_L4C);
}


/*-------------------------------------------------------*/

/*-------------------------------------------------------
                      获取短信中心
-------------------------------------------------------*/
void RJ_GPS_get_sc_rsp (srv_sms_callback_struct* callback_data)
{
        S8 *pdata = (S8*)(callback_data->action_data);

        if(callback_data->result == MMI_TRUE)
        {
                memset(rj_user_info.sc, 0, sizeof(rj_user_info.sc));
                mmi_ucs2_to_asc((S8*)rj_user_info.sc, pdata);
        }
}

void RJ_GPS_get_sc(void)
{
        srv_sms_sim_enum sim_able;

        sim_able=SRV_SMS_SIM_1;

        srv_sms_get_setting_para(SRV_SMS_ACTIVE_SC_ADDR, 
                                                    NULL,
                                                    sim_able, 
                                                    RJ_GPS_get_sc_rsp, 
                                                    NULL);
}


U8 * bird_get_sim_iccid_from_nvram( )
{
    if(strlen(yd_tk001_info.sim_iccid)>15)
        return yd_tk001_info.sim_iccid;
    else
        return NULL;
}
/*-------------------------------------------------------*/

/*-------------------------------------------------------
                      获取ICCID
-------------------------------------------------------*/
void RJ_GPS_get_iccid_rsp(srv_sim_cb_struct *info)
{
        mmi_smu_read_sim_rsp_struct *iccid_data =(mmi_smu_read_sim_rsp_struct*) info;
        U8 iccid[RJ_GPS_LEN]; 
        U16 read_length = 0;
        U16 result = 0;
        U8 i=0;

        read_length = iccid_data->length;
        result = iccid_data->result.flag;

        memset(iccid,0,sizeof(iccid)); 

        kal_prompt_trace(MOD_SOC,"RJ_GPS_get_iccid_rsp,%d %d",result,iccid_data->length);
        for (i = 0; i < iccid_data->length; i++) 
        { 
            kal_prompt_trace(MOD_SOC,"RJ_GPS_get_iccid_rsp,data[%d]=%d",i, *(iccid_data->data + i));
        }
        if(iccid_data->result.flag == L4C_SUCCESS)
        {
 
            	{ 
                    for (i = 0; i <  iccid_data->length; i++)
                    {            
                        if((*(iccid_data->data + i) & 0x0F)>9)
                        iccid[2*i] = (*(iccid_data->data + i) & 0x0F) + 0x37;
                        else
                        iccid[2*i] = (*(iccid_data->data + i) & 0x0F) + 0x30;
						
                        if(((*(iccid_data->data + i) & 0xF0) >> 4)>9)
                        iccid[2*i+1] = ((*(iccid_data->data + i) & 0xF0) >> 4) + 0x37;
                        else			
                        iccid[2*i+1] = ((*(iccid_data->data + i) & 0xF0) >> 4) + 0x30;
                    }
                     iccid[20] = 0;
            	}
 
             
                memset(rj_user_info.u_iccid,0,sizeof(rj_user_info.u_iccid));
                memcpy(rj_user_info.u_iccid,iccid, sizeof(rj_user_info.u_iccid));

               kal_prompt_trace(MOD_SOC,"iccid = %s",iccid);        

                if(strcmp((const char *)bird_get_sim_iccid_from_nvram(), (const char *)rj_user_info.u_iccid) != 0)
                {
                	g_n_ydischangesim = 2;
                }
		 else{
		 	g_n_ydischangesim=1;
	 	}

        }
}

void RJ_GPS_iccid(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE Message;
    mmi_smu_read_sim_req_struct *dataPtr;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   mmi_frm_set_protocol_event_handler(MSG_ID_MMI_SMU_READ_SIM_RSP, (PsIntFuncPtr)RJ_GPS_get_iccid_rsp, MMI_TRUE);
    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_L4C;
    Message.oslMsgId = MSG_ID_MMI_SMU_READ_SIM_REQ;
    dataPtr = (mmi_smu_read_sim_req_struct*) OslConstructDataPtr(sizeof(mmi_smu_read_sim_req_struct));
    dataPtr->file_idx = (U8) FILE_ICCID_IDX;
    dataPtr->para = 0;
    dataPtr->length = 0x0a;
    Message.oslDataPtr = (oslParaType*) dataPtr;
    Message.oslPeerBuffPtr = NULL;
    OslMsgSendExtQueue(&Message);
}

/*-------------------------------------------------------*/


void bird_deal_send_msg_handler(U16 bird_msg_id , U8 bird_para_id , U8 is_init)
{
        ilm_struct *bird_ilm = NULL;   
        BIRD_id_struct    *bird_id_data = NULL; //lrf

        bird_id_data = (BIRD_id_struct*)construct_local_para(sizeof(BIRD_id_struct), TD_CTRL);	

        bird_id_data ->Is_init_post = is_init;

        switch(bird_msg_id)
        {
            case MSG_ID_YD_TK001_MSG_NORMAL_STATIC:
            case MSG_ID_YD_TK001_MSG_NORMAL_RUNNING:
            case MSG_ID_YD_TK001_MSG_SET_DEFENSE:
            case MSG_ID_YD_TK001_MSG_CLEAN_DEFENSE:
            case MSG_ID_MMI_GPS_IND:
            case MSG_ID_MMI_YD_TK001_MSG_PARA_SAVE_NV:
            case MSG_ID_MMI_YD_TK001_MSG_CONTROL_SAVE_NV:
            case MSG_ID_MMI_YD_TK001_MSG_COMMAND_POSITION:
            case MSG_ID_YD_TK001_MSG_REBOOT:
            case MSG_ID_MMI_YD_TK001_MSG_POST_PARA:
            case MSG_ID_MMI_YD_TK001_MSG_ALARM_MOVED:
            case MSG_ID_AUX_KEY_OPEN:
            case MSG_ID_MMI_YD_TK001_MSG_ALARM_SPEED:
            case MSG_ID_RJ_OUTSIDE_VOLT_DISCONNECT:
	     case MSG_ID_YD_TK001_MSG_SAVENV:
	     case MSG_ID_YD_TK001_MSG_SAVENV_RESET:
	     case MSG_ID_YD_TK001_MSG_CLOSEGPS:
  	     case MSG_ID_YD_TK001_MSG_OPENGPS:
            case MSG_ID_YD_TK001_MSG_WRITE_AGPS:
            case MSG_ID_YD_TK001_MSG_WRITE_PEPH:		
            case MSG_ID_YD_TK001_MSG_START_WRITE_PEPH:				
	     case MSG_ID_YD_TK001_MSG_FLY:
            case MSG_ID_RJ_LOW_VOLT:
	     case MSG_ID_YD_TK001_MSG_SOS_IS_ON:
	     case MSG_ID_YD_TK001_MSG_SOS_IS_OFF:
	     case MSG_ID_YD_TK001_MSG_AZ_CLOSE:
	     case MSG_ID_YD_TK001_MSG_AZ_OPEN:
		 case MSG_ID_YD_TK001_MSG_NORMAL_COLLISION:
	     case MSG_ID_ACC_KEY_CLOSED:
	     case MSG_ID_AUX_ECU_DATA_ALARM:
	     case MSG_ID_YD_TK001_MSG_LOCK_SUCCESS:
	     case MSG_ID_YD_TK001_MSG_UNLOCK_SUCCESS:
	     case MSG_ID_YD_TK001_MSG_UNLOCK_FAIL:
	     case MSG_ID_YD_TK001_MSG_MOTOR_FAILURE:
	     case MSG_ID_YD_TK001_MSG_LOCK_STATE:
	     case MSG_ID_YD_TK001_MSG_UNLOCK_STATE:
	     case 	MSG_ID_YD_TK001_MSG_C9_CMD1_INQUIRE:
	     case MSG_ID_YD_TK001_MSG_C9_CMD2_INQUIRE:
	     case MSG_ID_YD_TK001_MSG_C9_CMD3_INQUIRE:
	     case MSG_ID_YD_TK001_MSG_C9_CMD4_INQUIRE:
	     case MSG_ID_YD_TK001_MSG_A0_CMD_ALARM:
	     case MSG_ID_YD_TK001_MSG_CLOSE_ALARM:
	     case MSG_ID_YD_TK001_MSG_RESTART_ALARM:
	     case MSG_ID_YD_TK001_MSG_ECU_UNCHARGING:
	     case MSG_ID_YD_TK001_MSG_ECU_CHARGING_ON:
	     case MSG_ID_YD_TK001_MSG_ECU_ACC_OFF:
	     case MSG_ID_YD_TK001_MSG_ECU_ACC_ON:
	     case MSG_ID_YD_TK001_MSG_SPEED_LIMIT_OFF:
	     case MSG_ID_YD_TK001_MSG_SPEED_LIMIT_ON:
	     case MSG_ID_YD_TK001_MSG_NORMAL_MOTOR_STATUS:
	     case MSG_ID_YD_TK001_MSG_DISCONNECT_MOTOR_STATUS:
	     case MSG_ID_YD_TK001_MSG_VIBRATE_IS_ON:
	     case MSG_ID_YD_TK001_MSG_ECU_STATIC:
	     case MSG_ID_YD_TK001_MSG_ECU_RUNNING:
	     case MSG_ID_AUX_DEFEND_CLOSED:
	     case MSG_ID_AUX_DEFEND_OPEN:
	     case MSG_ID_YD_TK001_MSG_ECU_DATA_ALARM:		 	
	     case 	MSG_ID_AUX_ECU_CONNECT_ALARM:
	     case MSG_ID_AUX_ECU_DISCONNECT_ALARM:
	     case MSG_ID_YD_TK001_MSG_CHARGING_OVER_VOLTAGE:
	     case MSG_ID_BATTERY_CMD_ALARM:
	     case MSG_ID_HARDWARE_CONFIG_CMD_ALARM:
	     case MSG_ID_FAULT_CMD_ALARM:
	     case MSG_ID_ALARM_CMD:
	     case MSG_ID_SELFDEFINE_DATA_CMD:
	     case MSG_ID_YD_TK001_MSG_ECU_START:
	     case MSG_ID_YD_TK001_MSG_ECU_CLOSE:
	     case MSG_ID_YD_TK001_MSG_SOFT_START:
	     case MSG_ID_YD_TK001_MSG_HARD_START:
	     case MSG_ID_YD_TK001_MSG_SPEED_START:
	     case MSG_ID_YD_TK001_MSG_SELFDEFINE_START:
	     case MSG_ID_YD_TK001_MSG_PLAYTONE_ALARM:
	     case MSG_ID_YD_TK001_MSG_BT_ACCREDIT:
	     case MSG_ID_BT_SENSOR_OPEN:
	     case MSG_ID_BT_SENSOR_CLOSE:
	     case MSG_ID_BT_MAC_DATA_ALARM:
	     case MSG_ID_BIRD_UART_MESSAGE_DATA:
	     case MSG_ID_YD_TK001_MSG_REMOTER:
	     case MSG_ID_AUX_MUTE_DEFEND_OPEN:
	     case MSG_ID_REMOTER_LEARN_SUCCESS:
                     bird_id_data ->para_setting_id = bird_para_id;
                     break;

             default:
                return;
            
        }
        DRV_BuildPrimitive(bird_ilm, MOD_MMI, MOD_MMI, bird_msg_id, bird_id_data);       
        msg_send_ext_queue(bird_ilm);     
}
