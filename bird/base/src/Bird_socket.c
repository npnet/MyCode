/**************************************************************************
  Copyright (C), 2012-2013, BIRD. Co., Ltd.
  File name:      
  Author:       Version:        Date: 2013-05-29
  Description:   Socket universal function
  Others:         
  Function List:  
  History:       
    1. Date:
       Author:
       Modification:
****************************************************************************/

#ifndef __MTK_TARGET__
#include <windows.h>    /* for soc_api.h */
#endif


#include "Bird_socket.h"
#include "Bird_app.h"
#include "Bird_task.h"
#include "Bird_gpscell.h"
#include "cbm_api.h"

#ifdef BIRD_ECU_MMI_SUPPORT
#include "Yd_uart_main.h"
#endif
/**************************************************************************************************
    限本文件使用的全局变量 
**************************************************************************************************/
BOOL g_lima_is_soc_init = FALSE;   //socket is created only once
BOOL g_lima_is_getip = FALSE; //lrf add for get ip
BOOL g_lima_is_conn = FALSE;//lrf add for socket is conn
static KAL_ADM_ID g_lima_gp_soc_adm_id = NULL;
static U8         *g_lima_gp_soc_pool = NULL;
static lima_soc_transaction_struct *g_lima_gp_socket       = NULL;
static S32                           g_lima_nwt_acount_id  = -1;   // 断网ID
static mmi_lima_soc_progress_t     g_lima_pfn_progreess  = NULL;//  进度条通知函数 
static S32                           g_lima_timeout_count  = -1;   // 设置接收数据超时的count
static BOOL                          g_lima_gb_timer_ted = FALSE;//超时的timer是否已经开启
static U8                            g_lima_cbm_app_id     = 0;
static U8                            g_lima_rec_res_flag     = 0;
static Socket_Send g_socket_send;
Socket_Send g_socket_send_alarm;
static Socket_Rev g_socket_rev;
static U8 g_n_reconn = 0;//lrf for reconn times
static U8 g_n_rev_reconn = 0;//
U8 g_n_rev_state = 0;
U8 receive_data_array[RECEIVE_DATA_LENGTH] = {0};
U8 receive_data_arraysingle[RECEIVE_DATA_LENGTH] = {0};
extern volatile kal_int8 count_judge_gps_send_timer ;
U8 nrestartnum = 0;

static U8         *g_lima_gp_soc_down_pool = NULL;
static KAL_ADM_ID g_lima_gp_soc_down_adm_id = NULL;

U32 aact_id =0; // add by lqy for gpssetings.c id use
U8 g_pos_return = 0;
S32 g_pos_index = 0;
//low power
S32 g_alarmlow_index=0;
U8 g_alarmlow_return = 0;
U8 g_alarmlow_times = 0;

//power off
S32 g_alarmpoweroff_index=0;
U8 g_alarmpoweroff_return = 0;
U8 g_alarmpoweroff_times = 0;

//move
S32 g_alarmmove_index=0;
U8 g_alarmmove_return = 0;
U8 g_alarmmove_times = 0;

//run
S32 g_alarmrun_index=0;
U8 g_alarmrun_return = 0;
U8 g_alarmrun_times = 0;

//sleep
U8 g_sleeppos_return = 0;
S32 g_sleeppos_index = 0;
U8 g_sleeppos_times = 0;

extern U8 g_gettime_flag;
extern U8 g_third_alarm_flag;

extern void Yd_logintxbox();

BOOL Lima_get_soc_init_flag()
{
     return g_lima_is_soc_init;
}

BOOL Lima_get_soc_conn_flag()
{
     return g_lima_is_conn;
}

void Lima_set_soc_conn_flag(BOOL flag)
{
     g_lima_is_conn = flag;
}

void Lima_set_soc_init_flag(BOOL flag)
{
     g_lima_is_soc_init = flag;
}

void Lima_set_soc_send_flag(U8 sendstatue)
{
	g_socket_send.g_n_send_state = sendstatue;
}

U8 Lima_get_soc_rev_state()
{
     return g_n_rev_state;
}

void Lima_set_soc_rev_state(U8 rev_state)
{
     g_n_rev_state = rev_state;
}

void Bird_clear_soc_conn()
{	
    kal_prompt_trace(MOD_SOC," Bird_clear_soc_conn");
    g_n_reconn = 0;
    g_lima_is_conn = FALSE;
    g_lima_is_getip=FALSE;
}
/**************************************************************************************************
Function name    :    Lima_Soc_Init
Functions        :        初始化申请Socket资源
Parameters        :    0               
    input        :    NONE         
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-05-15        create:
Youzhong            2009-08-25        08A添加g_lima_cbm_app_id区分不同应用发送的请求
GuoJin                      2009-08-25             08A添加cbm_hold_bearer，在soc_colse时保持网络连接不关闭
GuoJin                      2009-09-28             修改移动卡优先联网
**************************************************************************************************/

lima_sim_card_srv Lima_plat_simcard_option(void)
{
#ifndef WIN32
	  if(srv_nw_info_get_service_availability(MMI_SIM1) == SRV_NW_INFO_SA_FULL_SERVICE)
	  {
	       return lima_srv_sim_card1;
	  }	
 #ifdef __MMI_DUAL_SIM_MASTER__
	  else if(srv_nw_info_get_service_availability(MMI_SIM2) == SRV_NW_INFO_SA_FULL_SERVICE)
	  {
	       return lima_srv_sim_card2;
	  }
 #endif     
	  else
	  {
	      return lima_srv_sim_card_none;

	  }
#else
        return lima_srv_sim_card1;
	  
#endif
}


/**************************************************************************************************
Function name    :    Lima_Soc_Set_Conn_Timeout
Functions        :        设置连接超时的时间counter
Parameters        :    1               
    input        :    count :超时count        
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-05-19        create:
**************************************************************************************************/
void Lima_Soc_Set_Conn_Timeout(S32 count)
{
    g_lima_timeout_count = count;
}

//cancel by lqy
/*
U16  Lima_plat_get_yys(void)
{     
	U8  status = 0;
	S8 lima_plmn[7];
       mmi_sim_enum sim;
       
       if(lima_srv_sim_card2==Lima_plat_simcard_option())
       {
          sim=MMI_SIM2;
       }
	else if (lima_srv_sim_card1==Lima_plat_simcard_option())
       {
          sim=MMI_SIM1;
       }   
       else
       {
          sim=MMI_SIM_NONE; 
       }
      memset(lima_plmn, 0, sizeof(lima_plmn));
      status = srv_nw_info_get_nw_plmn(sim, lima_plmn, 7);
	  
      
	if (status == 0)
	{
	    return lima_operator_cmcc;
	}

	if(strncmp(lima_plmn, BIRD_UNICOM_HOME_NUMBER_1,BIRD_HOME_NUMBER_LENGTH) == 0)
	{
		return lima_operator_unicom;//getYYS : liantong
	}

       return lima_operator_cmcc;
    
	 
}*/  

//cancell by lqy

//add add by lqy   
S8 *  bird_get_nw_plmn(void)
{     
	S8 nw_plmn[7];
       mmi_sim_enum sim;
       MMI_BOOL status;
       if(lima_srv_sim_card2==Lima_plat_simcard_option())
       {
          sim=MMI_SIM2;
       }
	else if (lima_srv_sim_card1==Lima_plat_simcard_option())
       {
          sim=MMI_SIM1;
       }   
       else
       {
          sim=MMI_SIM_NONE; 
       }
      memset(nw_plmn, 0, sizeof(nw_plmn));
      status = srv_nw_info_get_nw_plmn(sim, nw_plmn, 7);
      kal_prompt_trace(MOD_SOC,"status=%d",status);	   
      kal_prompt_trace(MOD_SOC,"plmn=%s",nw_plmn);
	  
      return nw_plmn;
      /*
	if (status == 0)
	{
	    return lima_operator_cmcc;
	}

	if(strncmp(lima_plmn, BIRD_UNICOM_HOME_NUMBER_1,BIRD_HOME_NUMBER_LENGTH) == 0)
	{
		return lima_operator_unicom;//getYYS : liantong
	}

       return lima_operator_cmcc;*/
    
	 
}
//add add by lqy

// add by lqy
U32 Get_aact_id(void)
{
   return aact_id;
}
// add by lqy

void Bird_soc_set_sendflag(S8 nflow)
{
	U8 i=0;
	kal_prompt_trace(MOD_SOC,"Bird_soc_set_sendflag flow:%d",nflow);
	for(i=0;i<MAX_BIRD_SENDBUF_LEN;i++)
	{
		//kal_prompt_trace(MOD_SOC,"Bird_soc_set_sendflag statue:%d,flow %d",g_socket_send.send_info[i].send_statue,g_socket_send.send_info[i].send_flow);
		if(g_socket_send.send_info[i].send_statue==1)
		{
			continue;
		}
		
		if(g_socket_send.send_info[i].send_flow==nflow)
		{
			g_socket_send.send_info[i].send_statue=1;
			kal_prompt_trace(MOD_SOC,"Bird_soc_set_sendflag index:%d",i);
			break;
		}
	}
}

void Bird_soc_set_revflag(BOOL flag)
{
       kal_prompt_trace(MOD_SOC, "[Bird_soc_set_revflag] %d", flag);
	if(flag)
	{
		g_socket_send.g_n_rev_state = 1;
	 	g_n_rev_reconn = 0;
	}
	else
	{
		g_socket_send.g_n_rev_state = 0;		
	}
}

void Bird_soc_set_loginflag()
{
	U8 i=0;
	kal_prompt_trace(MOD_SOC,"Bird_soc_set_loginflag ");
	for(i=0;i<MAX_BIRD_SENDBUF_LEN;i++)
	{
		if(g_socket_send.send_info[i].send_statue==1)
		{
			continue;
		}
		
		if(g_socket_send.send_info[i].send_type==BIRD_SOC_SEND_LOGIN)
		{
			g_socket_send.send_info[i].send_statue=1;
			kal_prompt_trace(MOD_SOC,"Bird_soc_set_loginflag index:%d",i);
			break;
		}
	}
}

U8 Bird_soc_get_loginflag()
{
	U8 nflag = 0;
	U8 i=0;
	for(i=0;i<MAX_BIRD_SENDBUF_LEN;i++)
	{		
		if(g_socket_send.send_info[i].send_type==BIRD_SOC_SEND_LOGIN)
		{
			if(g_socket_send.send_info[i].send_statue==0)
			{
				nflag = 1;
				kal_prompt_trace(MOD_SOC,"bird_soc_send_login entry2\n");  
				break;
			}
		}
	}	
	return nflag;
}
/***
***/
void Bird_soc_clear_file()
{
    S32  handle;
    U32  len;
    U32  buflen;
    U16  name[128];
    kal_prompt_trace(MOD_SOC,"Bird_soc_clear_file entry");
    memset(name, 0, sizeof(name));
    kal_wsprintf((kal_wchar*) name,BIRD_POS_FILE_PATH,(S16)MMI_CARD_DRV);
    handle = FS_Open((U16 *)name, FS_CREATE | FS_READ_WRITE);
    if (handle >= FS_NO_ERROR)
    {
    	FS_Seek(handle, 0, FS_FILE_BEGIN);
	FS_Truncate(handle);
    }
    FS_Close(handle);	
}
void Bird_soc_save_file(Send_Info *_send,applib_time_struct dt)
{
    S32  handle;
    U32  len;
    U32  buflen;
    kal_wchar  name[BIRD_FILE_PATH_LEN];
    U16  i=0;
    static S8 buf[200];
    U8 nend=1;
	
    memset(name, 0, sizeof(name));
    kal_wsprintf(name,BIRD_TBOXDATA_PATH,(S16)MMI_CARD_DRV, dt.nYear, dt.nMonth, dt.nDay, dt.nHour);
    handle = FS_Open(name, FS_CREATE | FS_READ_WRITE);
    kal_prompt_trace(MOD_SOC,"Bird_soc_save_file %d",handle);

    if (handle >= FS_NO_ERROR)
    {
    	 FS_GetFileSize(handle,&buflen);
    	 if(buflen>=0)
	 {
	 	FS_Seek(handle, buflen, FS_FILE_BEGIN);
		kal_prompt_trace(MOD_SOC,"Bird_soc_save_file %d %d %d",buflen,_send->buf_len,_send->send_buf[2]);	
		{
			FS_Seek(handle, buflen, FS_FILE_BEGIN);
			//_send->send_buf[2]=0x03;
			FS_Write(handle, _send->send_buf,_send->buf_len, &len);
			FS_Close(handle);
			return;
		}
	 }
    }
    FS_Close(handle);
}

void Bird_soc_save_info(applib_time_struct dt)
{

    S32  handle;
    U32  len;
    U32  buflen;
    kal_wchar  name[BIRD_FILE_PATH_LEN];
    U8 time[50];
    S8 yearchar[4];
    S8 monthchar[4];
    S8 daychar[4];
    S8 hourchar[4];
    S8 minchar[4];
    S8 secchar[4];	
    U16  i=0;

    memset(name, 0, sizeof(name));
    kal_wsprintf( name,BIRD_TBOXINFO_PATH,(S16)MMI_CARD_DRV, dt.nYear, dt.nMonth, dt.nDay, dt.nHour);
    handle = FS_Open(name, FS_CREATE | FS_READ_WRITE);
    kal_prompt_trace(MOD_SOC,"Bird_soc_save_info %d",handle);
    kal_prompt_trace(MOD_SOC,"Bird_soc_save_info %d %d %d %d %d %d",dt.nYear, dt.nMonth, dt.nDay, dt.nHour,dt.nMin,dt.nSec); 

    memset(time, 0, sizeof(time));
    sprintf(yearchar,"%d",dt.nYear);
   if(dt.nMonth<10){
   	 sprintf(monthchar,"%d",0);
	sprintf(monthchar+1,"%d",dt.nMonth);
   }
   else{
   	
   	sprintf(monthchar,"%d",dt.nMonth);
   }
   
   if(dt.nDay<10){
   	daychar[0]='0';
   	sprintf(daychar+1,"%d",dt.nDay);
   }
   else{
   	sprintf(daychar,"%d",dt.nDay);
   }
   
   if(dt.nHour<10){
   	hourchar[0]='0';
   	sprintf(hourchar+1,"%d",dt.nHour);
   }
   else{
   	sprintf(hourchar,"%d",dt.nHour);
   }
   if(dt.nMin<10){
   	minchar[0]='0';
   	sprintf(minchar+1,"%d",dt.nMin);
   }
   else{
   	sprintf(minchar,"%d",dt.nMin);
   }
   
   if(dt.nSec<10){
   	secchar[0]='0';
   	sprintf(secchar+1,"%d",dt.nSec);
   }
   else{
   	sprintf(secchar,"%d",dt.nSec);
   }

    memcpy(time, (S8 *)&yearchar, 4);
    memcpy(time+4, (S8 *)&monthchar, 2);
    memcpy(time+6, (S8 *)&daychar, 2);
    memcpy(time+8, (S8 *)&hourchar, 2); 
    memcpy(time+10, (S8 *)&minchar, 2); 
    memcpy(time+12, (S8 *)&secchar, 2); 

    if (handle >= FS_NO_ERROR)
    {
	FS_GetFileSize(handle,&buflen);
	if(buflen>=0)
	{
		{
			FS_Seek(handle, buflen, FS_FILE_BEGIN);
			FS_Write(handle, time,14, &len);
			FS_Seek(handle, buflen+14, FS_FILE_BEGIN);
			FS_Write(handle, "9",strlen("9"), &len);

			kal_prompt_trace(MOD_SOC,"Bird_soc_save_info %s %d,%d",time,buflen,len);
			{
				FS_Close(handle);
				return;
			}
		}
	}
	else{
		kal_prompt_trace(MOD_SOC,"Bird_soc_save_info buflen<0");
	}
    }
    FS_Close(handle);
}

void Bird_infoformat(applib_time_struct* dt, U8 *time, U32 *length)
{

    S8 yearchar[6];
    S8 monthchar[4];
    S8 daychar[4];
    S8 hourchar[4];
    S8 minchar[4];
    S8 secchar[4];	
	/*
    applib_time_struct *dt;
	
    dt->nYear=(U16)(input[0]+2000);
    dt->nMonth=input[1];
    dt->nDay=input[2];
    dt->nHour=input[3];
    dt->nMin=input[4];
    dt->nSec=input[5];
	*/
    kal_prompt_trace(MOD_SOC,"Bird_infoformat %d %d %d %d %d %d",dt->nYear, dt->nMonth, dt->nDay, dt->nHour,dt->nMin,dt->nSec); 

    sprintf(yearchar,"%d",dt->nYear);
   if(dt->nMonth<10){
   	 sprintf(monthchar,"%d",0);
	sprintf(monthchar+1,"%d",dt->nMonth);
   }
   else{
   	
   	sprintf(monthchar,"%d",dt->nMonth);
   }
   
   if(dt->nDay<10){
   	daychar[0]='0';
   	sprintf(daychar+1,"%d",dt->nDay);
   }
   else{
   	sprintf(daychar,"%d",dt->nDay);
   }
   
   if(dt->nHour<10){
   	hourchar[0]='0';
   	sprintf(hourchar+1,"%d",dt->nHour);
   }
   else{
   	sprintf(hourchar,"%d",dt->nHour);
   }
   if(dt->nMin<10){
   	minchar[0]='0';
   	sprintf(minchar+1,"%d",dt->nMin);
   }
   else{
   	sprintf(minchar,"%d",dt->nMin);
   }
   
   if(dt->nSec<10){
   	secchar[0]='0';
   	sprintf(secchar+1,"%d",dt->nSec);
   }
   else{
   	sprintf(secchar,"%d",dt->nSec);
   }

    memcpy(time, (S8 *)&yearchar, 4);
    memcpy(time+4, (S8 *)&monthchar, 2);
    memcpy(time+6, (S8 *)&daychar, 2);
    memcpy(time+8, (S8 *)&hourchar, 2); 
    memcpy(time+10, (S8 *)&minchar, 2); 
    memcpy(time+12, (S8 *)&secchar, 2); 
    *length=14;

}
void Bird_soc_setflag_posfile(applib_time_struct dt)
{
    FS_HANDLE  handle;
    U32  len;
    U32 lenper;
    U32  buflen;
    kal_wchar  name[128];
    S8 buf[20];
    S8 nresult=0;
    U32 off;
    S8 yearchar[6]={0};
    S8 monthchar[4]={0};
    S8 daychar[4]={0};
    S8 hourchar[4]={0};
    S8 minchar[4]={0};
    S8 secchar[4]={0};	
	
    lenper = strlen(BIRD_TBOXINFO);
    memset(name, 0, sizeof(name));
    kal_wsprintf(name,BIRD_TBOXINFO_PATH,(S16)MMI_CARD_DRV, dt.nYear, dt.nMonth, dt.nDay, dt.nHour);
    handle = FS_Open(name, FS_READ_WRITE);
    kal_prompt_trace(MOD_SOC,"Bird_soc_setflag_posfile %d", handle);
	
    if (handle >= FS_NO_ERROR)
    {
	FS_GetFileSize(handle,&buflen);
	if(buflen> 0)
	{
	       for(off=0;off<buflen;off=off+lenper)
		{
			FS_Seek(handle, off, FS_FILE_BEGIN);
			memset(buf, 0, sizeof(buf));
			FS_Read(handle, (void *)buf, lenper, &len);
			
			memcpy(yearchar, (S8 *)&buf, 4);
			memcpy(monthchar, (S8 *)&buf+4, 2);
			memcpy(daychar, (S8 *)&buf+6, 2);
			memcpy(hourchar, (S8 *)&buf+8, 2);
			memcpy(minchar, (S8 *)&buf+10, 2);
			memcpy(secchar, (S8 *)&buf+12, 2);
			
			kal_prompt_trace(MOD_SOC,"Bird_soc_setflag_posfile %d,%d,%s",buflen,off,buf);
			if((atoi(yearchar)==dt.nYear)&&(atoi(monthchar)==dt.nMonth)&&(atoi(daychar)==dt.nDay)&&(atoi(hourchar)==dt.nHour)
				&&(atoi(minchar)==dt.nMin)&&(atoi(secchar)==dt.nSec))
			{
			       FS_Seek(handle, off+14, FS_FILE_BEGIN);
				FS_Write(handle, "1",strlen("1"), &len);
				kal_prompt_trace(MOD_SOC,"Bird_soc_setflag_posfile write %d",len);
			       //FS_Close(handle);
			       //return;
			}

		}

	}
    }
    FS_Close(handle);
}

S8 Bird_soc_getfrom_file(U16 noff)
{
    S32  handle;
    U32  len;
    U32 lenper;
    U32  buflen;
    U16  name[128];
    static S8 buf[200];
    S8 nresult;
    Send_Info _send;	
    U32 off;
    S8 i;
    kal_prompt_trace(MOD_SOC,"Bird_soc_getfrom_file entry");
    lenper = MAX_BIRD_NUMBER_PERLINE;
    memset(name, 0, sizeof(name));
    kal_wsprintf((kal_wchar*) name,BIRD_POS_FILE_PATH,(S16)MMI_CARD_DRV);
    handle = FS_Open((U16 *)name, FS_CREATE | FS_READ_WRITE);
    if (handle >= FS_NO_ERROR)
    {
    	 FS_GetFileSize(handle,&buflen);
	if(buflen> 0)
	{
		/*for(i=0;i<MAX_BIRD_POSSAVE_COUNT;i++)
		 {		 
		 	FS_Seek(handle, i*lenper, FS_FILE_BEGIN);
			memset(buf, 0, sizeof(buf));
			FS_Read(handle, (void *)buf, lenper, &len);
			kal_prompt_trace(MOD_SOC,"Bird_soc_save_file len=%d,%s",len,buf);
			if(len==0)
			{
				break;
			}
		 }*/
		off =  noff*lenper;
		if(off<=buflen){
			FS_Seek(handle, off, FS_FILE_BEGIN);
			memset(buf, 0, sizeof(buf));
			FS_Read(handle, (void *)buf, lenper, &len);
			kal_prompt_trace(MOD_SOC,"Bird_soc_getfrom_file2 len%d,%d,off=%d",buflen,len,off);
		 	kal_prompt_trace(MOD_SOC,"Bird_soc_getfrom_file2 len%s",buf);
			//未处理9 or 8
			if(buf[0]==57 || buf[0]==56)
			{
				nresult = 9;
				_send.buf_len=0;
				_send.ini_flag=0;
				_send.send_flow=0;
				_send.send_type=BIRD_SOC_SEND_GPSPOS;
				memset(_send.send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
				memcpy(_send.send_buf,buf+strlen("9"),strlen(buf)-strlen("9"));
				_send.buf_len = strlen(_send.send_buf);
				Bird_soc_sendbufAdd2(&_send);
				nresult = 9;
			}
			else{
				nresult = 1;
			}
		}
		else{
			nresult = 0;
		}

	}
	else{
		nresult = 0;
	}
       FS_Close(handle);
       return nresult ;

    }
}

S8 Bird_soc_is_exist_unread(U16 noff)
{
	S32  handle;
	U32  len;
	U32 lenper;
	U32  buflen;
	U16  name[128];
	static S8 buf[200];
	S8 nresult;
	U32 off;
	S8 i;
	kal_prompt_trace(MOD_SOC,"Bird_soc_is_exist_unread entry");
	lenper = MAX_BIRD_NUMBER_PERLINE;
	memset(name, 0, sizeof(name));
	kal_wsprintf((kal_wchar*) name,BIRD_POS_FILE_PATH,(S16)MMI_CARD_DRV);
	handle = FS_Open((U16 *)name, FS_CREATE | FS_READ_WRITE);
	if (handle >= FS_NO_ERROR)
	{
		FS_GetFileSize(handle,&buflen);
		if(buflen> 0)
		{
			off =  noff*lenper;
			if(off<=buflen)
			{
				FS_Seek(handle, off, FS_FILE_BEGIN);
				memset(buf, 0, sizeof(buf));
				FS_Read(handle, (void *)buf, lenper, &len);
				kal_prompt_trace(MOD_SOC,"Bird_soc_is_exist_unread len%d,%d,off=%d",buflen,len,off);
				kal_prompt_trace(MOD_SOC,"Bird_soc_is_exist_unread len%s",buf);
				//未处理9 or 8
				if(buf[0]==57 || buf[0]==56)
				{
					nresult = 9;
				}
				else
				{
					nresult = 1;
				}
			}
			else
			{/*操作出错*/
				nresult = 0;
			}
		}
		else
		{/*打开的文件无数据，*/
			nresult = 0;
		}
		FS_Close(handle);
		return nresult ;
	}
}

void Bird_pos_return()
{
	U8 send_index=0;
	Send_Info _send;	
	if(g_pos_return==1)
	{
		return;
	}
	send_index = (g_pos_index % MAX_BIRD_SENDBUF_LEN);
	if(g_socket_send.send_info[send_index].buf_len<=0)
	{
		return;
	}
	_send = g_socket_send.send_info[send_index];
	//Bird_Tbox_save(&_send);
	
}
S32 Bird_get_bufunsend(){
	return g_socket_send.g_n_send_indexAdd - g_socket_send.g_n_send_index;
}
void Bird_sleeppos_return()
{
	U8 send_index=0;
	S32 nindex = 0;
	Send_Info _send;	
	if(!Lima_get_soc_conn_flag())
	{
		Rj_stop_timer(BIRD_TASK_SLEEPPOS_WAIT_TIMER);
		//Rj_start_timer(BIRD_TASK_SLEEPPOS_WAIT_TIMER, 60*1000, Bird_sleeppos_return,NULL);
		return;
	}
	nindex = g_socket_send.g_n_send_indexAdd - g_sleeppos_index;
	if(nindex>=MAX_BIRD_SENDBUF_LEN){
		return;
	}
	g_sleeppos_times = g_sleeppos_times +1;
	if(g_sleeppos_times>=3)
	{
		g_sleeppos_times = 0;
		Rj_stop_timer(BIRD_TASK_SLEEPPOS_WAIT_TIMER);
		if(bird_get_normal_sleep_state())
    		{
			Yd_DiniSocketDelay();
		}		
	}
	else{
		if(g_sleeppos_return==1)
		{
			Rj_stop_timer(BIRD_TASK_SLEEPPOS_WAIT_TIMER);
			if(bird_get_normal_sleep_state())
    			{
				Yd_DiniSocketDelay();
			}
			return;
		}
		send_index = (g_sleeppos_index % MAX_BIRD_SENDBUF_LEN);
		if(g_socket_send.send_info[send_index].buf_len<=0)
		{
			return;
		}
		_send = g_socket_send.send_info[send_index];	
		_send.send_type=BIRD_SOC_SEND_OTHER;
		Bird_soc_sendbufAdd2(&_send);	
		Rj_stop_timer(BIRD_TASK_SLEEPPOS_WAIT_TIMER);
		Rj_start_timer(BIRD_TASK_SLEEPPOS_WAIT_TIMER, 60*1000, Bird_sleeppos_return,NULL);
	}
}

void Bird_alarm_poweroff_return(){
	U8 send_index=0;
	S32 nindex = 0;
	Send_Info _send;	
	if(!Lima_get_soc_conn_flag())
	{
		Rj_stop_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER);
		//Rj_start_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER, 60*1000, Bird_alarm_poweroff_return,NULL);
		return;
	}
	nindex = g_socket_send.g_n_send_indexAdd - g_alarmpoweroff_index;
	if(nindex>=MAX_BIRD_SENDBUF_LEN){
		return;
	}	
	g_alarmpoweroff_times = g_alarmpoweroff_times +1;
	if(g_alarmpoweroff_times>=3)
	{
		g_alarmpoweroff_times = 0;
		Rj_stop_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER);
	}
	else{
		if(g_alarmpoweroff_return==1)
		{
			Rj_stop_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER);
			return;
		}
		send_index = (g_alarmpoweroff_index % MAX_BIRD_SENDBUF_LEN);
		if(g_socket_send.send_info[send_index].buf_len<=0)
		{
			return;
		}
		_send = g_socket_send.send_info[send_index];	
		_send.send_type=BIRD_SOC_SEND_OTHER;
		Bird_soc_sendbufAdd2(&_send);	
		Rj_stop_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER);
		Rj_start_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER, 60*1000, Bird_alarm_poweroff_return,NULL);
	}
}
void Bird_alarm_move_return(){
	U8 send_index=0;
	S32 nindex = 0;
	Send_Info _send;	
	if(!Lima_get_soc_conn_flag())
	{
		Rj_stop_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER);
		//Rj_start_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER, 60*1000, Bird_alarm_move_return,NULL);
		return;
	}
	nindex = g_socket_send.g_n_send_indexAdd - g_alarmmove_index;
	if(nindex>=MAX_BIRD_SENDBUF_LEN){
		return;
	}	
	g_alarmmove_times = g_alarmmove_times +1;
	if(g_alarmmove_times>=3)
	{
		g_alarmmove_times = 0;
		Rj_stop_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER);
	}
	else{
		if(g_alarmmove_return==1)
		{
			Rj_stop_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER);
			return;
		}
		send_index = (g_alarmmove_index % MAX_BIRD_SENDBUF_LEN);
		if(g_socket_send.send_info[send_index].buf_len<=0)
		{
			return;
		}
		_send = g_socket_send.send_info[send_index];	
		_send.send_type=BIRD_SOC_SEND_OTHER;
		Bird_soc_sendbufAdd2(&_send);	
		Rj_stop_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER);
		Rj_start_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER, 60*1000, Bird_alarm_move_return,NULL);
	}
}
void Bird_alarm_run_return(){
	U8 send_index=0;
	S32 nindex = 0;
	Send_Info _send;	
	if(!Lima_get_soc_conn_flag())
	{
		Rj_stop_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER);
		//Rj_start_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER, 60*1000, Bird_alarm_run_return,NULL);	
		return;
	}
	nindex = g_socket_send.g_n_send_indexAdd - g_alarmrun_index;
	if(nindex>=MAX_BIRD_SENDBUF_LEN){
		return;
	}	
	g_alarmrun_times = g_alarmrun_times +1;
	kal_prompt_trace(MOD_SOC,"[Bird_alarm_run_return] times,return,index=%d,%d,%d",g_alarmrun_times,g_alarmrun_return,nindex);
	if(g_alarmrun_times>=3)
	{
		g_alarmrun_times = 0;
		Rj_stop_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER);
	}
	else{
		if(g_alarmrun_return==1)
		{
			Rj_stop_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER);
			return;
		}
		send_index = (g_alarmrun_index % MAX_BIRD_SENDBUF_LEN);
		if(g_socket_send.send_info[send_index].buf_len<=0)
		{
			return;
		}
		_send = g_socket_send.send_info[send_index];	
		_send.send_type=BIRD_SOC_SEND_OTHER;
		Bird_soc_sendbufAdd2(&_send);	
		Rj_stop_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER);
		Rj_start_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER, 60*1000, Bird_alarm_run_return,NULL);
	}
}

void Bird_alarm_low_return(){
	U8 send_index=0;
	S32 nindex = 0;
	Send_Info _send;	
	if(!Lima_get_soc_conn_flag())
	{
		Rj_stop_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER);
		//Rj_start_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER, 60*1000, Bird_alarm_low_return,NULL);
		return;
	}
	nindex = g_socket_send.g_n_send_indexAdd - g_alarmlow_index;
	if(nindex>=MAX_BIRD_SENDBUF_LEN){
		return;
	}	
	g_alarmlow_times = g_alarmlow_times +1;
	if(g_alarmlow_times>=3)
	{
		g_alarmlow_times = 0;
		Rj_stop_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER);
	}
	else{
		if(g_alarmlow_return==1)
		{
			Rj_stop_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER);
			return;
		}
		send_index = (g_alarmlow_index % MAX_BIRD_SENDBUF_LEN);
		if(g_socket_send.send_info[send_index].buf_len<=0)
		{
			return;
		}
		_send = g_socket_send.send_info[send_index];	
		_send.send_type=BIRD_SOC_SEND_OTHER;
		Bird_soc_sendbufAdd2(&_send);	
		Rj_stop_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER);
		Rj_start_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER, 60*1000, Bird_alarm_low_return,NULL);
	}

}

void Bird_heart_return()
{
	kal_prompt_trace(MOD_SOC,"Bird_heart_return");

	Lima_set_soc_init_flag(FALSE);
	Lima_Soc_Dinit();
	Bird_clear_soc_conn();
	Bird_soc_conn();
	Yd_logintxbox();
	
}
/***************************************************************************
  Function:      Bird_soc_send 
  Description:    send from buf to socket
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-06-19
****************************************************************************/ 
void Bird_soc_send(){
	S32                   ret = 0;	
	U8 i=0;
	U8 send_index=0;
	//Bird_Log2(2);
	kal_prompt_trace(MOD_SOC,"Bird_soc_send entry");
	//StartTimer(BIRD_SOCKET_SEND, RJ_GPS_APP_1M, Bird_soc_send);
	//Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
       //Rj_start_timer(BIRD_TASK_SOCKET_SEND, RJ_GPS_APP_1M, Bird_soc_send,NULL);

	if(!Lima_get_soc_conn_flag())
	{
		//lrf 20131029add 如果有需要发送的数据启动连接
		if(g_socket_send.g_n_send_index<g_socket_send.g_n_send_indexAdd)
		{
			Bird_soc_conn();
		}
		else
		{
			Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
			Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);			
		}
		return;

	}
	/*	
	if(g_socket_send.g_n_send_state!=BIRD_SOC_NULL)
	{
		 
		 kal_prompt_trace(MOD_SOC,"Bird_soc_send:g_n_rev_state =%d ,g_n_rev_reconn =%d",g_socket_send.g_n_rev_state,g_n_rev_reconn);
		 if(g_socket_send.g_n_rev_state==0) 
		 {
		 	g_n_rev_reconn++;
		 	if(g_n_rev_reconn>80)
		 	{
		 	 	g_socket_send.g_n_rev_state=0;
				g_n_rev_reconn=0; 
				Lima_set_soc_init_flag(FALSE);
				Lima_Soc_Dinit();
				//lrf 20131029add 如果有需要发送的数据启动连接
				Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
				Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);	
				return;
				//Bird_soc_conn();
	 			
		 	}
		      
	 	}
		else if(g_socket_send.g_n_rev_state==1)
		{
			g_n_rev_reconn = 0;
		}
	}
	*/	
	kal_prompt_trace(MOD_SOC,"[Bird_soc_send]g_socket_send.g_n_send_state= %d,index  = %d,indexAdd = %d",g_socket_send.g_n_send_state,g_socket_send.g_n_send_index,g_socket_send.g_n_send_indexAdd);	
       if(g_socket_send.g_n_send_state!=BIRD_SOC_SENDING)
       {
       	/*
	       //重发
       	for(i=0;i<MAX_BIRD_SENDBUF_LEN;i++)
		{
			if(g_socket_send.send_info[i].send_statue==1)
			{
				continue;
			}
			if(g_socket_send.send_info[i].ini_flag==0)
			{
				continue;
			}	
			g_socket_send.send_info[i].send_count++;
			kal_prompt_trace(MOD_SOC,"[Bird_soc_send] send_count = %d,index = %d",g_socket_send.send_info[i].send_count,i);
			if(g_socket_send.send_info[i].send_count>15)
			{
			       kal_prompt_trace(MOD_SOC,"[Bird_soc_send] resent flow= %d,sendtimes=%d",g_socket_send.send_info[i].send_flow,g_socket_send.send_info[i].send_times);
				if(g_socket_send.send_info[i].send_times>=3)
				{
					continue;
				}
				g_socket_send.send_info[i].send_count=0;
				memcpy(g_lima_gp_socket->snd_buffer, g_socket_send.send_info[i].send_buf, g_socket_send.send_info[i].buf_len);   
			 	g_lima_gp_socket->snd_data_len = g_socket_send.send_info[i].buf_len;
				g_socket_send.send_info[i].send_times++;
				g_socket_send.g_n_send_state=BIRD_SOC_SENDING;
				break;
				
			}
		}
		*/
	       if(g_socket_send.g_n_send_state!=BIRD_SOC_SENDING)
	       {
	       	if(g_socket_send.g_n_send_index<g_socket_send.g_n_send_indexAdd)
			{
				send_index = (g_socket_send.g_n_send_index % MAX_BIRD_SENDBUF_LEN);
				if(g_socket_send.send_info[send_index].buf_len<=0)
				{
					g_socket_send.g_n_send_index++;
					//StartTimer(BIRD_SOCKET_SEND, 2000, Bird_soc_send);
					Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
				       Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);
					return;
				}
		       	memcpy(g_lima_gp_socket->snd_buffer, g_socket_send.send_info[send_index].send_buf, g_socket_send.send_info[send_index].buf_len);   
			 	g_lima_gp_socket->snd_data_len = g_socket_send.send_info[send_index].buf_len;
			 	g_socket_send.send_info[send_index].send_times++;
				//lrf 2013-10-27 login
				if(g_socket_send.send_info[send_index].send_type==BIRD_SOC_SEND_LOGIN)     
				{
					g_socket_send.send_info[send_index].send_statue=1;
				}	
				else if(g_socket_send.send_info[send_index].send_type==BIRD_SOC_SEND_GPSPOS)
				{
					g_pos_index = g_socket_send.g_n_send_index;
					g_pos_return = 0;
					Rj_stop_timer(BIRD_TASK_POS_WAIT_TIMER);
				       Rj_start_timer(BIRD_TASK_POS_WAIT_TIMER, 12*1000, Bird_pos_return,NULL);
				}
				else if(g_socket_send.send_info[send_index].send_type==BIRD_SOC_SEND_SLEEPPOS)
				{
					//g_sleeppos_index = g_socket_send.g_n_send_index;
					//g_sleeppos_return = 0;
					//g_sleeppos_times = 0;
					//Rj_stop_timer(BIRD_TASK_SLEEPPOS_WAIT_TIMER);
				       //Rj_start_timer(BIRD_TASK_SLEEPPOS_WAIT_TIMER, 60*1000, Bird_sleeppos_return,NULL);
				}
				else if(g_socket_send.send_info[send_index].send_type==BIRD_SOC_SEND_ALARM_LOW)
				{
					g_alarmlow_index = g_socket_send.g_n_send_index;
					g_alarmlow_return = 0;
					g_alarmlow_times = 0;
					kal_prompt_trace(MOD_SOC,"[Bird_soc_send] alarm_low");
					Rj_stop_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER);
				       Rj_start_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER, 60*1000, Bird_alarm_low_return,NULL);
				}
				else if(g_socket_send.send_info[send_index].send_type==BIRD_SOC_SEND_ALARM_POWEROFF)
				{
					g_alarmpoweroff_index = g_socket_send.g_n_send_index;
					g_alarmpoweroff_return = 0;
					g_alarmpoweroff_times = 0;
					kal_prompt_trace(MOD_SOC,"[Bird_soc_send] alarm_poweroff");
					Rj_stop_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER);
				       Rj_start_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER, 60*1000, Bird_alarm_poweroff_return,NULL);
				}
				else if(g_socket_send.send_info[send_index].send_type==BIRD_SOC_SEND_ALARM_MOVE)
				{
					g_alarmmove_index = g_socket_send.g_n_send_index;
					g_alarmmove_return = 0;
					g_alarmmove_times = 0;
					kal_prompt_trace(MOD_SOC,"[Bird_soc_send] alarm_move");
					Rj_stop_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER);
				       Rj_start_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER, 60*1000, Bird_alarm_move_return,NULL);
				}	
				else if(g_socket_send.send_info[send_index].send_type==BIRD_SOC_SEND_ALARM_ZD)
				{
					g_alarmrun_index = g_socket_send.g_n_send_index;
					g_alarmrun_return = 0;
					g_alarmrun_times = 0;
					kal_prompt_trace(MOD_SOC,"[Bird_soc_send] alarm_zd");
					Rj_stop_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER);
				       Rj_start_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER, 60*1000, Bird_alarm_run_return,NULL);
				}
				else if(g_socket_send.send_info[send_index].send_type==BIRD_SOC_SEND_HEART)
				{
					Rj_stop_timer(BIRD_TASK_HEART_WAIT_TIMER);
				       Rj_start_timer(BIRD_TASK_HEART_WAIT_TIMER, 60*1000, Bird_heart_return,NULL);
				}
				else if(g_socket_send.send_info[send_index].send_type!=BIRD_SOC_SEND_NOREPLY)     
				{
					Bird_soc_set_revflag(FALSE);	
				}

			 	g_socket_send.g_n_send_index++;
				g_socket_send.g_n_send_state=BIRD_SOC_SENDING;
				g_lima_gp_socket->snd_counter = 0;
			}
			else
			{
				//StartTimer(BIRD_SOCKET_SEND, 2000, Bird_soc_send);
				Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
			       Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);
				return;
			}
	       }
		
       }
        g_socket_send.g_n_send_state=BIRD_SOC_SENDING;	
        kal_prompt_trace(MOD_SOC,"[Bird_soc_send]g_lima_gp_socket->snd_data_len = %d, %d,%s",g_lima_gp_socket->snd_data_len,g_lima_gp_socket->snd_counter,(g_lima_gp_socket->snd_buffer + g_lima_gp_socket->snd_counter));	

        ret = soc_send(g_lima_gp_socket->socket_id,
                        (kal_uint8*) (g_lima_gp_socket->snd_buffer + g_lima_gp_socket->snd_counter),
                        (g_lima_gp_socket->snd_data_len - g_lima_gp_socket->snd_counter),
                        0);
           
        kal_prompt_trace(MOD_SOC, "[Bird_soc_send]Http send request result, sent_bytes:%d", ret);
        if (ret > 0)
        {
		g_socket_send.g_n_send_state=BIRD_SOC_SENTED;
		//g_socket_send.g_n_rev_state = 0;
		//StartTimer(BIRD_SOCKET_SEND, 1000, Bird_soc_send);
		Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
		if(g_socket_send.g_n_send_index<g_socket_send.g_n_send_indexAdd)
                  Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);
		else
                  Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);
		mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)Lima_Soc_Socket_Notify);
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,(PsIntFuncPtr)Lima_Soc_Socket_Notify,MMI_TRUE);		  
         }
         else if (ret == SOC_WOULDBLOCK)
         {
                 //StartTimer(BIRD_SOCKET_SEND, 5000, Bird_soc_send);           
		   g_socket_send.g_n_send_state=BIRD_SOC_SENTED;		 
                 mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)Lima_Soc_Socket_Notify);
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,(PsIntFuncPtr)Lima_Soc_Socket_Notify,MMI_TRUE);	
		   Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
	          Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);      				 
         }
         else
         {
                 kal_prompt_trace(MOD_SOC,"[Bird_soc_send]LIMA_SOC_SEND_ERR");
		   g_socket_send.g_n_send_state=BIRD_SOC_SENTED;		 
		   //StartTimer(BIRD_SOCKET_SEND, 2000, Bird_soc_send);
	 	   Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
                 Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);	
		   mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)Lima_Soc_Socket_Notify);
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,(PsIntFuncPtr)Lima_Soc_Socket_Notify,MMI_TRUE);			 
          }
	kal_prompt_trace(MOD_SOC, "[Bird_soc_send]send ret:%d", ret);	 
	

	
}

/***************************************************************************
  Function:      Bird_soc_connbyIp 
  Description:    after get ip,connect to socket
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-06-19
****************************************************************************/ 
void Bird_soc_connbyIp()
{
	kal_int8                   ret = 0;
	U8 nrestresion=0;
	sockaddr_struct *p_soc_add = NULL; //lrf;
       g_lima_gp_socket->state = LIMA_SOC_HTTP_TCP_CON_CREATING;
       p_soc_add = &g_lima_gp_socket->server_ip_addr;
	g_lima_is_getip = true;   
       ret = soc_connect(g_lima_gp_socket->socket_id, p_soc_add); 
       kal_prompt_trace(MOD_SOC,"[Bird_soc_connbyIp]server_ip_addr:%d.%d.%d.%d and port: %d",
                                p_soc_add->addr[0], p_soc_add->addr[1],p_soc_add->addr[2],
                                p_soc_add->addr[3],    p_soc_add->port);
                
       kal_prompt_trace(MOD_SOC,"[Bird_soc_connbyIp]Http connect result is %d", ret);
       if (ret == SOC_SUCCESS)
       {
       		//LED闪烁
		rj_led_status_info.b_SOC_IS_CONNECTED = KAL_TRUE;
		rj_led_status_info.b_SOC_IS_CONNECTING = KAL_FALSE;
		
              g_lima_gp_socket->state = LIMA_SOC_HTTP_TCP_CON_CREATED;  /*已经连接*/
		g_n_reconn = 0;
   		Lima_set_soc_conn_flag(TRUE);
		nrestresion = Bird_get_reseterr();
		if(nrestresion!=0){
			Bird_set_reseterr(BIRD_NORMAL);
			yd_send_save_nv_msg();
		}			
		Bird_save_ip();
		mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)Lima_Soc_Socket_Notify);
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,(PsIntFuncPtr)Lima_Soc_Socket_Notify,MMI_TRUE);	
              //SetProtocolEventHandler(Lima_Soc_Socket_Notify, MSG_ID_APP_SOC_NOTIFY_IND);
	       Bird_soc_send();
       }
       else if (ret == SOC_WOULDBLOCK)
       {//增加超时处理             
                  mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)Lima_Soc_Socket_Notify);
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,(PsIntFuncPtr)Lima_Soc_Socket_Notify,MMI_TRUE);	
       }			
       else
       {//error
                 kal_prompt_trace(MOD_SOC, "[Lima_Soc_Http_Send_Request]LIMA_SOC_CONNECT_ERR");
	          mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)Lima_Soc_Socket_Notify);
		mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,(PsIntFuncPtr)Lima_Soc_Socket_Notify,MMI_TRUE);	
       }
	
}

void Bird_save_ip(){
	U8	saveaddr[36];
	U8   getaddr[36];
	S8 fjchar[10];
	S8 dt[1];
	S8 nlen=0;
						
	memset(saveaddr,0,sizeof(saveaddr));
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",g_lima_gp_socket->server_ip_addr.addr[0]);
	memcpy(saveaddr,fjchar, strlen(fjchar));
       nlen = nlen + strlen(fjchar);
	//.
	memcpy(saveaddr+nlen,".", 1);
	nlen = nlen + 1;
	
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",g_lima_gp_socket->server_ip_addr.addr[1]);
	memcpy(saveaddr+nlen,fjchar, strlen(fjchar));
       nlen = nlen + strlen(fjchar);
	//.
	memcpy(saveaddr+nlen,".", 1);
	nlen = nlen + 1;

	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",g_lima_gp_socket->server_ip_addr.addr[2]);
	memcpy(saveaddr+nlen,fjchar, strlen(fjchar));
       nlen = nlen + strlen(fjchar);
	//.
	memcpy(saveaddr+nlen,".", 1);
	nlen = nlen + 1;

	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",g_lima_gp_socket->server_ip_addr.addr[3]);
	memcpy(saveaddr+nlen,fjchar, strlen(fjchar));
       nlen = nlen + strlen(fjchar);
	
	memcpy(saveaddr+nlen,":",1);
	nlen = nlen + 1;
	
	memset(fjchar,0,sizeof(fjchar));
	sprintf(fjchar,"%d",g_lima_gp_socket->server_ip_addr.port);
	memcpy(saveaddr+nlen,(S8 *)fjchar,strlen((S8 *)fjchar));
	memset(getaddr,0,sizeof(getaddr)); 
	memcpy(getaddr,(U8 *)bird_get_ipadd(),strlen((U8 *)bird_get_ipadd()));
	kal_prompt_trace(MOD_SOC,"[Bird_save_ip] get: %s",getaddr);
	if(strcmp(saveaddr,getaddr)==0){
		kal_prompt_trace(MOD_SOC,"[Bird_save_ip]: same");
	}
	else{
		bird_set_ipadd(saveaddr);
		yd_send_save_nv_msg();
	}
	kal_prompt_trace(MOD_SOC,"[Bird_save_ip]: %s",saveaddr);
}

/***************************************************************************
  Function:      Bird_soc_conn 
  Description:    socket connect and auto connect
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-06-19
****************************************************************************/ 
U8 g_signal_flag=0;
extern U8 Yd_get_equstatus();
extern void yd_send_reboot_control_msg();
extern volatile kal_int8 count_judge_gps_send_timer;

void Bird_soc_conn()
{	
	U16 ret = 0;
	U8 nrestarttimes = 0;
	U8 nrestresion = 0;
	 
	   
	if(Lima_get_soc_conn_flag())
	{
		g_n_reconn = 0;
		Bird_save_ip();
		//if have error write normal
		nrestresion = Bird_get_reseterr();
		kal_prompt_trace(MOD_SOC," [Bird_soc_conn] error flag%d",nrestresion);
		if(nrestresion!=0){
			Bird_set_reseterr(BIRD_NORMAL);
			yd_send_save_nv_msg();
		}		
		return;
	}
	else
	{
		//LED闪烁
		rj_led_status_info.b_SOC_IS_CONNECTING = KAL_TRUE;
		rj_led_status_info.b_SOC_IS_CONNECTED = KAL_FALSE;
		
	       count_judge_gps_send_timer=0;
		{
			g_n_reconn ++;
			kal_prompt_trace(MOD_SOC,"[Bird_soc_conn]: g_n_reconn=%d",g_n_reconn);
	   		if(g_n_reconn>=3)
			{	
			       Bird_set_reseterr(BIRD_CONN_ERR);     
				Bird_set_resttime(); //write rest time
				g_n_reconn = 0;
				kal_prompt_trace(MOD_SOC,"g_n_reconn 30 min");
			       Rj_stop_timer(BIRD_TASK_SOCKET_RECON);
			       Rj_start_timer(BIRD_TASK_SOCKET_RECON, bird_get_conn_interval()*RJ_GPS_APP_1M, Bird_soc_conn,NULL);
			}	
			else
			{
			       Rj_stop_timer(BIRD_TASK_SOCKET_RECON);
			       Rj_start_timer(BIRD_TASK_SOCKET_RECON, RJ_GPS_APP_1M, Bird_soc_conn,NULL);
			}
		}
	}
	Lima_Soc_Dinit();
	if(!Lima_get_soc_init_flag())
	{
		ret = Lima_Soc_Init();
		if (LIMA_SOC_NO_ERROR == ret)
    		{
			ret = Lima_Soc_Create_Socket();
			if (LIMA_SOC_CREATE_ERR != ret)
	       	{	       		
	            		kal_prompt_trace(MOD_SOC,"[g_lima_is_getip]:%d",g_lima_is_getip);
				if(g_n_reconn==2)
				{
					Bird_soc_Set_Header(1);
					Lima_Soc_Http_Get_Parsed_Url((const S8*)g_lima_gp_socket->url, g_lima_gp_socket->url_parsed_info);
					Lima_Soc_Get_Host_By_Name((const S8*)g_lima_gp_socket->url_parsed_info->host_name);	
				}
				else{
					if(!g_lima_is_getip)
					{
						Bird_soc_Set_Header(0);
						Lima_Soc_Http_Get_Parsed_Url((const S8*)g_lima_gp_socket->url, g_lima_gp_socket->url_parsed_info);
						Lima_Soc_Get_Host_By_Name((const S8*)g_lima_gp_socket->url_parsed_info->host_name);	
					}
					else
					{
						Bird_soc_connbyIp();
					}
				}

			}
			else
			{
				Lima_set_soc_init_flag(FALSE);
				Lima_Soc_Dinit();
			}
		}		
	}	
	else
	{
		if(g_n_reconn==2)
		{
			Bird_soc_Set_Header(1);
			Lima_Soc_Http_Get_Parsed_Url((const S8*)g_lima_gp_socket->url, g_lima_gp_socket->url_parsed_info);
			Lima_Soc_Get_Host_By_Name((const S8*)g_lima_gp_socket->url_parsed_info->host_name);	
		}
		else{
			if(!g_lima_is_getip)
			{
				Bird_soc_Set_Header(0);
				Lima_Soc_Http_Get_Parsed_Url((const S8*)g_lima_gp_socket->url, g_lima_gp_socket->url_parsed_info);
				Lima_Soc_Get_Host_By_Name((const S8*)g_lima_gp_socket->url_parsed_info->host_name);	
			}
			else
			{
				Bird_soc_connbyIp();
			}
		}
	}
	
}

/***************************************************************************
  Function:      Bird_soc_sendbufAdd 
  Description:    add a struct to send buf
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lrf2013-06-19
****************************************************************************/ 
void Bird_soc_sendbufAdd2(Send_Info *_sendinfo)
{
       S32 send_index=0;	   
     	kal_prompt_trace(MOD_SOC,"Bird_soc_sendbufAdd2 \n");  
       
	if(_sendinfo->buf_len>MAX_BIRD_SENDBUF_SIZE)
	{
		return;
	}
	send_index = (g_socket_send.g_n_send_indexAdd % MAX_BIRD_SENDBUF_LEN);
	g_socket_send.send_info[send_index].buf_len =_sendinfo->buf_len;
	g_socket_send.send_info[send_index].ini_flag =_sendinfo->ini_flag;
	g_socket_send.send_info[send_index].send_flow=_sendinfo->send_flow;
	g_socket_send.send_info[send_index].send_statue=0;
	g_socket_send.send_info[send_index].send_times=0;
	g_socket_send.send_info[send_index].send_count=0;
	g_socket_send.send_info[send_index].send_type=_sendinfo->send_type;
	memset(g_socket_send.send_info[send_index].send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	memcpy(g_socket_send.send_info[send_index].send_buf, _sendinfo->send_buf, MAX_BIRD_SENDBUF_SIZE);   
	kal_prompt_trace(MOD_SOC,"Bird_soc_sendbufAdd2 buf_len,indexAdd%d,%d",_sendinfo->buf_len,send_index); 
	g_socket_send.g_n_send_indexAdd++;
}

void Bird_soc_alarm_sendbufAdd2(Send_Info *_sendinfo)
{
       S32 send_index=0;	   
     	kal_prompt_trace(MOD_SOC,"Bird_soc_alarm_sendbufAdd2 \n");  
       
	if(_sendinfo->buf_len>MAX_BIRD_SENDBUF_SIZE)
	{
		return;
	}
	send_index = (g_socket_send_alarm.g_n_send_indexAdd % MAX_BIRD_SENDBUF_LEN);
	g_socket_send_alarm.send_info[send_index].buf_len =_sendinfo->buf_len;
	g_socket_send_alarm.send_info[send_index].ini_flag =_sendinfo->ini_flag;
	g_socket_send_alarm.send_info[send_index].send_flow=_sendinfo->send_flow;
	g_socket_send_alarm.send_info[send_index].send_statue=0;
	g_socket_send_alarm.send_info[send_index].send_times=0;
	g_socket_send_alarm.send_info[send_index].send_count=0;
	g_socket_send_alarm.send_info[send_index].send_type=_sendinfo->send_type;
	memset(g_socket_send_alarm.send_info[send_index].send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
	memcpy(g_socket_send_alarm.send_info[send_index].send_buf, _sendinfo->send_buf, MAX_BIRD_SENDBUF_SIZE);   
	kal_prompt_trace(MOD_SOC,"Bird_soc_alarm_sendbufAdd2 buf_len,indexAdd%d,%d",_sendinfo->buf_len,send_index); 
	g_socket_send_alarm.g_n_send_indexAdd++;
}

void Bird_soc_alarm_to_send()
{
     	S8 i,j;
       U8 end_index=0;	   
       S32 send_index=0;	  

     	kal_prompt_trace(MOD_SOC,"Bird_soc_alarm_to_send");  
/*
      for (i = 0; i < g_socket_send_alarm.g_n_send_indexAdd ; i++)
      {
              send_index = (g_socket_send.g_n_send_indexAdd % MAX_BIRD_SENDBUF_LEN);
              g_socket_send.send_info[send_index].buf_len =g_socket_send_alarm.send_info[i].buf_len;
              g_socket_send.send_info[send_index].ini_flag =g_socket_send_alarm.send_info[i].ini_flag;
              g_socket_send.send_info[send_index].send_flow=g_socket_send_alarm.send_info[i].send_flow;
              g_socket_send.send_info[send_index].send_statue=g_socket_send_alarm.send_info[i].send_statue;
              g_socket_send.send_info[send_index].send_times=g_socket_send_alarm.send_info[i].send_times;
              g_socket_send.send_info[send_index].send_count=g_socket_send_alarm.send_info[i].send_count;
              g_socket_send.send_info[send_index].send_type=g_socket_send_alarm.send_info[i].send_type;
              memset(g_socket_send.send_info[send_index].send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
              memcpy(g_socket_send.send_info[send_index].send_buf, g_socket_send_alarm.send_info[i].send_buf, MAX_BIRD_SENDBUF_SIZE);   
		g_socket_send.g_n_send_indexAdd++;
      }
*/       
      for (i = 0; i < g_socket_send_alarm.g_n_send_indexAdd/3; i++)
      {
              send_index = (g_socket_send.g_n_send_indexAdd % MAX_BIRD_SENDBUF_LEN);
              g_socket_send.send_info[send_index].buf_len =g_socket_send_alarm.send_info[i*3].buf_len*3;
              g_socket_send.send_info[send_index].ini_flag =g_socket_send_alarm.send_info[i*3].ini_flag;
              g_socket_send.send_info[send_index].send_flow=g_socket_send_alarm.send_info[i*3].send_flow;
              g_socket_send.send_info[send_index].send_statue=g_socket_send_alarm.send_info[i*3].send_statue;
              g_socket_send.send_info[send_index].send_times=g_socket_send_alarm.send_info[i*3].send_times;
              g_socket_send.send_info[send_index].send_count=g_socket_send_alarm.send_info[i*3].send_count;
              g_socket_send.send_info[send_index].send_type=g_socket_send_alarm.send_info[i*3].send_type;
              memset(g_socket_send.send_info[send_index].send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
              for (j = 0; j <3; j++)  
                  memcpy(g_socket_send.send_info[send_index].send_buf+j*g_socket_send_alarm.send_info[i*3].buf_len, g_socket_send_alarm.send_info[i*3+j].send_buf, g_socket_send_alarm.send_info[i*3].buf_len);  
		end_index=end_index+3;	  
		g_socket_send.g_n_send_indexAdd++;
      }

      if(g_socket_send_alarm.g_n_send_indexAdd%3==0)
		return;
	  
      {
              send_index = (g_socket_send.g_n_send_indexAdd % MAX_BIRD_SENDBUF_LEN);
              g_socket_send.send_info[send_index].buf_len =g_socket_send_alarm.send_info[end_index].buf_len*(g_socket_send_alarm.g_n_send_indexAdd%3);
              g_socket_send.send_info[send_index].ini_flag =g_socket_send_alarm.send_info[end_index].ini_flag;
              g_socket_send.send_info[send_index].send_flow=g_socket_send_alarm.send_info[end_index].send_flow;
              g_socket_send.send_info[send_index].send_statue=g_socket_send_alarm.send_info[end_index].send_statue;
              g_socket_send.send_info[send_index].send_times=g_socket_send_alarm.send_info[end_index].send_times;
              g_socket_send.send_info[send_index].send_count=g_socket_send_alarm.send_info[end_index].send_count;
              g_socket_send.send_info[send_index].send_type=g_socket_send_alarm.send_info[end_index].send_type;
              memset(g_socket_send.send_info[send_index].send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
              for (j = 0; j <g_socket_send_alarm.g_n_send_indexAdd%3; j++)  
                  memcpy(g_socket_send.send_info[send_index].send_buf+j*g_socket_send_alarm.send_info[end_index].buf_len, g_socket_send_alarm.send_info[end_index+j].send_buf, g_socket_send_alarm.send_info[end_index].buf_len);   
		g_socket_send.g_n_send_indexAdd++;
      }
}

void Bird_soc_Set_Header(S8 nflag)
{
	lima_soc_req_header_struct header;	
	header.http_method  = LIMA_SOC_GET;
	if(nflag==0)
	{
		if(g_gettime_flag==0)
		header.p_url        = (S8 *)YD_TK001_HTTP2;
		else
		header.p_url        = (S8 *)bird_get_server_address();
	}
    	else{
		if(g_gettime_flag==0)
		header.p_url        = (S8 *)YD_TK001_IP2;
		else
		header.p_url        = (S8 *)bird_get_ipadd();	
    	}
    	header.fullpath     =  NULL;
    	header.post_data    = NULL;
    	header.post_data_len= 0;
    	Lima_Soc_Set_Req_Header(&header);
    	g_lima_is_getip = FALSE;
}

/***************************************************************************
  Function:      Bird_soc_send_clear 
  Description:    clear send buff
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
void Bird_soc_send_clear()
{
	/*lrf 2013-06-07add*/
     	S8 i;
	g_socket_send.g_n_send_index = 0;
	g_socket_send.g_n_send_indexAdd = 0;
	g_socket_send.g_n_send_state=0;
	g_socket_send.g_n_rev_state = 0;
      for (i = 0; i < MAX_BIRD_SENDBUF_LEN ; i++)
      {
		g_socket_send.send_info[i].buf_len = 0;
		memset(g_socket_send.send_info[i].send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
		g_socket_send.send_info[i].ini_flag = 0;
		g_socket_send.send_info[i].send_statue = 0;
		g_socket_send.send_info[i].send_times = 0;
		g_socket_send.send_info[i].send_flow=0;
		g_socket_send.send_info[i].send_count=0;
      }
   /*lrf 2013-06-07add*/
}

void Bird_soc_send_alarm_clear()
{
     	S8 i;
	g_socket_send_alarm.g_n_send_index = 0;
	g_socket_send_alarm.g_n_send_indexAdd = 0;
	g_socket_send_alarm.g_n_send_state=0;
	g_socket_send_alarm.g_n_rev_state = 0;
      for (i = 0; i < MAX_BIRD_SENDBUF_LEN ; i++)
      {
		g_socket_send_alarm.send_info[i].buf_len = 0;
		memset(g_socket_send_alarm.send_info[i].send_buf, 0, MAX_BIRD_SENDBUF_SIZE);
		g_socket_send_alarm.send_info[i].ini_flag = 0;
		g_socket_send_alarm.send_info[i].send_statue = 0;
		g_socket_send_alarm.send_info[i].send_times = 0;
		g_socket_send_alarm.send_info[i].send_flow=0;
		g_socket_send_alarm.send_info[i].send_count=0;
      }
}
/***************************************************************************
  Function:      add_new_acc_prof_id 
  Description:    add a new gprs profile message to the g_config_account_gprs[]
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lqy2014-04-4
****************************************************************************/ 
static kal_uint32 add_new_acc_prof_id(const WCHAR* account_name)
{
    srv_dtcnt_store_prof_data_struct store_prof_data;
    srv_dtcnt_prof_gprs_struct prof_gprs;
    kal_uint32 acc_prof_id;
    srv_dtcnt_result_enum ret;


    memset(&prof_gprs, 0, sizeof(prof_gprs));
    prof_gprs.APN = "bdnet";	// 这个值在应用使用的时候会修改的
    prof_gprs.prof_common_header.sim_info = SRV_DTCNT_SIM_TYPE_1;
    prof_gprs.prof_common_header.AccountName = (const U8*)account_name;

    prof_gprs.prof_common_header.acct_type = SRV_DTCNT_PROF_TYPE_USER_CONF;

    prof_gprs.prof_common_header.px_service = SRV_DTCNT_PROF_PX_SRV_HTTP;
    prof_gprs.prof_common_header.use_proxy = KAL_TRUE;
    prof_gprs.prof_common_header.px_addr[0] = 10;
    prof_gprs.prof_common_header.px_addr[1] = 0;
    prof_gprs.prof_common_header.px_addr[2] = 0;
    prof_gprs.prof_common_header.px_addr[3] = 172;
    prof_gprs.prof_common_header.px_port = 80;

    strcpy(prof_gprs.prof_common_header.px_authid,"");
    strcpy(prof_gprs.prof_common_header.px_authpw,"");
    	

    strcpy(prof_gprs.prof_common_header.HomePage, "http://wap.chinabird.com");

    store_prof_data.prof_data = &prof_gprs;
    store_prof_data.prof_fields = SRV_DTCNT_PROF_FIELD_ALL;
    store_prof_data.prof_type = SRV_DTCNT_BEARER_GPRS;

    ret = srv_dtcnt_store_add_prof(&store_prof_data, &acc_prof_id);
	
    kal_prompt_trace(MOD_SOC,"add acc_prof_id =%d",acc_prof_id);

    if(ret == SRV_DTCNT_RESULT_SUCCESS)
    {
       // mmi_dtcnt_add_disp_list(acc_prof_id,DATA_ACCOUNT_BEARER_GPRS,prof_gprs.prof_common_header.acct_type,1, prof_gprs.prof_common_header.sim_info, prof_gprs.prof_common_header.read_only);
       
    }
    else
    {
        acc_prof_id = -1;
    }

    return acc_prof_id;
}

/***************************************************************************
  Function:      update_acc_prof_id 
  Description:    update gprs profile message to the g_config_account_gprs[]
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lqy2014-04-4
****************************************************************************/ 
kal_uint32 update_acc_prof_id(kal_uint32 acc_prof_id, U8 *apn, U8 *px_addr, U16 px_port,U8 *px_authid, U8 *px_authpw, U8 *primaryaddr, U8 *secondaryaddr ,U8 *homepage)
{
    srv_dtcnt_store_prof_data_struct store_prof_data;
    srv_dtcnt_prof_gprs_struct prof_gprs;
    //kal_uint32 acc_prof_id;
    srv_dtcnt_result_enum ret;


    memset(&prof_gprs, 0, sizeof(prof_gprs));

   /*
    kal_prompt_trace(MOD_SOC,"apn1 %s  \n",apn); 
    memset((void *)prof_gprs.APN, 0x00, sizeof(SRV_DTCNT_PROF_MAX_APN_LEN));
    strcpy(prof_gprs.APN, (const U8 *)apn);
    memcpy((void *)prof_gprs.APN, (const void *)apn, sizeof(SRV_DTCNT_PROF_MAX_APN_LEN));
    kal_prompt_trace(MOD_SOC,"apn2 %s  \n",prof_gprs.APN); 	
    */

   
    kal_prompt_trace(MOD_SOC,"apn1 %s  \n",apn); 
   if((apn[0] !=0) ||(apn !=NULL))
   {
            prof_gprs.APN = apn;
    }			
    kal_prompt_trace(MOD_SOC,"apn2 %s  \n",prof_gprs.APN); 
    prof_gprs.prof_common_header.sim_info = SRV_DTCNT_SIM_TYPE_1;
    prof_gprs.prof_common_header.AccountName = (const U8*)L"BD Internet";

    prof_gprs.prof_common_header.acct_type = SRV_DTCNT_PROF_TYPE_USER_CONF;

    prof_gprs.prof_common_header.px_service = SRV_DTCNT_PROF_PX_SRV_HTTP;
    prof_gprs.prof_common_header.use_proxy = KAL_TRUE;

    kal_prompt_trace(MOD_SOC,"px_addr1 %s  \n",px_addr); 
    if((px_addr[0] !=0) ||(px_addr !=NULL)) 
    {	
        memset(prof_gprs.prof_common_header.px_addr, 0x00, sizeof(SRV_DTCNT_PROF_MAX_PROXY_ADDR_LEN));
        strcpy(prof_gprs.prof_common_header.px_addr, (const char *)px_addr);
    }	
    	
    kal_prompt_trace(MOD_SOC,"px_addr2 %s  \n",prof_gprs.prof_common_header.px_addr); 
    kal_prompt_trace(MOD_SOC,"px_port1 %d  \n",px_port); 
    if((px_port !=0) ||(px_port !=NULL))
    {	
        prof_gprs.prof_common_header.px_port = px_port;
     }
     	
    kal_prompt_trace(MOD_SOC,"px_port2 %d  \n",prof_gprs.prof_common_header.px_port); 
    kal_prompt_trace(MOD_SOC,"px_authid1 %s  \n",px_authid); 
    if((px_authid[0] !=0) ||(px_authid !=NULL)) 
    {		
        memset(prof_gprs.prof_common_header.px_authid, 0x00, sizeof(SRV_DTCNT_PROF_MAX_PX_AUTH_ID_LEN));
        strcpy(prof_gprs.prof_common_header.px_authid, (const char *)px_authid);
    }
    	
    kal_prompt_trace(MOD_SOC,"px_authid2 %s  \n",prof_gprs.prof_common_header.px_authid); 	

    kal_prompt_trace(MOD_SOC,"px_authpw1 %s  \n",px_authpw); 
    if((px_authpw[0] !=0) ||(px_authpw !=NULL)) 
    {		
        memset(prof_gprs.prof_common_header.px_authpw, 0x00, sizeof(SRV_DTCNT_PROF_MAX_PX_AUTH_PW_LEN));
        strcpy(prof_gprs.prof_common_header.px_authpw, (const char *)px_authpw);
    }
    	
    kal_prompt_trace(MOD_SOC,"px_authpw2 %s  \n",prof_gprs.prof_common_header.px_authpw); 
    
    kal_prompt_trace(MOD_SOC,"PrimaryAddr1 %s  \n",primaryaddr); 
    if((primaryaddr[0] !=0) ||(primaryaddr !=NULL)) 
    {		
        memset(prof_gprs.prof_common_header.PrimaryAddr, 0x00, sizeof(SRV_DTCNT_PROF_MAX_IP_ADDR_LEN));
        strcpy(prof_gprs.prof_common_header.PrimaryAddr, (const char *)primaryaddr);
    }
    	
    kal_prompt_trace(MOD_SOC,"PrimaryAddr2 %s  \n",prof_gprs.prof_common_header.PrimaryAddr); 


    kal_prompt_trace(MOD_SOC,"SecondaryAddr1 %s  \n",secondaryaddr); 
    if((secondaryaddr[0] !=0) ||(secondaryaddr !=NULL)) 
    {		
        memset(prof_gprs.prof_common_header.SecondaryAddr, 0x00, sizeof(SRV_DTCNT_PROF_MAX_IP_ADDR_LEN));
        strcpy(prof_gprs.prof_common_header.SecondaryAddr, (const char *)secondaryaddr);
    }
    	
    kal_prompt_trace(MOD_SOC,"SecondaryAddr2 %s  \n",prof_gprs.prof_common_header.SecondaryAddr); 

    kal_prompt_trace(MOD_SOC,"HomePage1 %s  \n",homepage); 
    if((homepage[0] !=0) ||(homepage !=NULL) )
    {		
        memset(prof_gprs.prof_common_header.HomePage, 0x00, sizeof(SRV_DTCNT_PROF_MAX_HOMEPAGE_LEN));
        strcpy(prof_gprs.prof_common_header.HomePage, (const char *)homepage);
    }
    	 	
    kal_prompt_trace(MOD_SOC,"HomePage2 %s  \n",prof_gprs.prof_common_header.HomePage); 


    store_prof_data.prof_data = &prof_gprs;
    store_prof_data.prof_fields = SRV_DTCNT_PROF_FIELD_ALL;
    store_prof_data.prof_type = SRV_DTCNT_BEARER_GPRS;

    ret = srv_dtcnt_store_update_prof(acc_prof_id, &store_prof_data);
	
    kal_prompt_trace(MOD_SOC,"update acc_prof_id =%d",acc_prof_id);

    if(ret == SRV_DTCNT_RESULT_SUCCESS)
    {
       // mmi_dtcnt_add_disp_list(acc_prof_id,DATA_ACCOUNT_BEARER_GPRS,prof_gprs.prof_common_header.acct_type,1, prof_gprs.prof_common_header.sim_info, prof_gprs.prof_common_header.read_only);
       
    }
    else
    {
        acc_prof_id = -1;
    }

    return acc_prof_id;
}

#define SOCKETREQID 985
static U16 Lima_Soc_Init(void)
{
    U16         ret;
  //  S32         index;//cmcc card index
  
 //   S32        acc;
//add by lqy
     mmi_sim_enum index;
     srv_dtcnt_store_prof_qry_struct acct_id_query; 
     srv_dtcnt_result_enum result;
     ilm_struct *bird_ilm = NULL;   //send msg to save nvram 
     int i,j;
     srv_dtcnt_prof_str_info_qry_struct prof_str_info_qry;
     U16 name[SRV_DTCNT_PROF_MAX_ACC_NAME_LEN+1];// = {0};	 
     char charname[SRV_DTCNT_PROF_MAX_ACC_NAME_LEN+1];
// add by lqy
    
    kal_prompt_trace(MOD_USB, "[BIRD_Soc_Init] Enter");
    index = 0;

    if (NULL == g_lima_gp_soc_pool)
    {//申请所需内存     
        media_get_ext_buffer(MOD_MMI, (void **)&g_lima_gp_soc_pool, MAX_BIRD_SOC_MEMORY_SIZE);
        ASSERT(g_lima_gp_soc_pool != NULL);
        g_lima_gp_soc_adm_id = kal_adm_create((void*)g_lima_gp_soc_pool, MAX_BIRD_SOC_MEMORY_SIZE, NULL, KAL_FALSE);
        g_lima_gp_socket = kal_adm_alloc(g_lima_gp_soc_adm_id, sizeof(lima_soc_transaction_struct));
        ASSERT(g_lima_gp_socket != NULL);
        memset(g_lima_gp_socket, 0, sizeof(lima_soc_transaction_struct));
        g_lima_gp_socket->socket_id = -1;
        g_lima_gp_socket->url_parsed_info = kal_adm_alloc(g_lima_gp_soc_adm_id, sizeof(lima_soc_http_url_struct));
        ASSERT(g_lima_gp_socket->url_parsed_info != NULL);
        memset(g_lima_gp_socket->url_parsed_info, 0, sizeof(lima_soc_http_url_struct));
        g_lima_gp_socket->rcvd_buffer       = kal_adm_alloc(g_lima_gp_soc_adm_id, MAX_BIRD_RCV_BUFFER_SIZE);
        ASSERT(g_lima_gp_socket->rcvd_buffer != NULL);
        memset(g_lima_gp_socket->rcvd_buffer, 0, MAX_BIRD_RCV_BUFFER_SIZE);
        g_lima_gp_socket->snd_buffer       = kal_adm_alloc(g_lima_gp_soc_adm_id, MAX_BIRD_SND_BUFFER_SIZE);
        ASSERT(g_lima_gp_socket->snd_buffer != NULL);
        memset(g_lima_gp_socket->snd_buffer, 0, MAX_BIRD_SND_BUFFER_SIZE);
        g_lima_gp_socket->url = kal_adm_alloc(g_lima_gp_soc_adm_id, MAX_BIRD_URL_LEN);
        ASSERT(g_lima_gp_socket->url != NULL);
        memset(g_lima_gp_socket->url,    0, MAX_BIRD_URL_LEN);
        //设置数据账户
#ifdef __MTK_TARGET__
        g_lima_gp_socket->nwt_acount_type = LIMA_SOC_CMNET;//LIMA_SOC_CMWAP;//yake_soc_get_connect_type();
#else//模拟器
        g_lima_gp_socket->nwt_acount_type = LIMA_SOC_CMNET;
#endif        
        if (0 == g_lima_cbm_app_id)
        {
            cbm_register_app_id(&g_lima_cbm_app_id);
            cbm_hold_bearer(g_lima_cbm_app_id);           
        }
        /*if (g_lima_gp_socket->nwt_acount_type == LIMA_SOC_CMWAP)
        {
             if (Lima_plat_get_yys()==lima_operator_unicom)
                srv_dtcnt_get_acc_id_by_apn("uniwap",&acc);
             else
                srv_dtcnt_get_acc_id_by_apn("cmwap",&acc);
        }		
        else
        {
		if (Lima_plat_get_yys()==lima_operator_unicom)
		{
			kal_prompt_trace(MOD_USB, "[BIRD_Soc_Init] uninet");
			srv_dtcnt_get_acc_id_by_apn("uninet",&acc);
			//srv_dtcnt_get_acc_id_by_apn("uniwap",&acc);
		}
		else
		{
			kal_prompt_trace(MOD_USB, "[BIRD_Soc_Init] cmnet");
			srv_dtcnt_get_acc_id_by_apn("cmnet",&acc);
		}
		   
        }*/
        //add by lqy

       bird_get_nw_plmn();   
       kal_prompt_trace(MOD_SOC,"bird_get_nw_plmn()=%s",bird_get_nw_plmn());
       memset(&acct_id_query,0,sizeof(acct_id_query)); 
       //acct_id_query.qry_info.filters = SRV_DTCNT_STORE_QRY_TYPE_SIM|SRV_DTCNT_STORE_QRY_TYPE_ACC_TYPE; 
	acct_id_query.qry_info.filters = SRV_DTCNT_STORE_QRY_TYPE_ACC_TYPE;    
       //acct_id_query.qry_info.bearer_qry_info = SRV_DTCNT_BEARER_GPRS; 
       acct_id_query.qry_info.acc_type_info = SRV_DTCNT_PROF_TYPE_FACTORY_CONF;	   
	
       result = srv_dtcnt_store_qry_ids(&acct_id_query);
	
	if (result == SRV_DTCNT_RESULT_SUCCESS)
	{
	    	 kal_prompt_trace(MOD_SOC,"factory srv_dtcnt_store_qry_ids sucess"); 
	        kal_prompt_trace(MOD_SOC,"factory acct_id_query num_ids=%d",acct_id_query.num_ids);  
		if(acct_id_query.num_ids>0)
		{
		             for (i=0;i<(acct_id_query.num_ids);i++)
	                     {
             
                                  kal_prompt_trace(MOD_SOC,"accound id=%d",acct_id_query.ids[i]);
	                           prof_str_info_qry.dest = (S8*)name;
                                  prof_str_info_qry.dest_len = sizeof(name);
                                  srv_dtcnt_get_account_name(acct_id_query.ids[i],&prof_str_info_qry,SRV_DTCNT_ACCOUNT_PRIMARY);
                                  mmi_ucs2_to_asc((char *)charname,(char *)prof_str_info_qry.dest);
		                    kal_prompt_trace(MOD_SOC,"prof_str_info_qry name=%s",charname); 
				     // get apn			
				     prof_str_info_qry.dest = (S8*)name;
                                  prof_str_info_qry.dest_len = sizeof(name);
                                  srv_dtcnt_get_apn(acct_id_query.ids[i],&prof_str_info_qry,SRV_DTCNT_ACCOUNT_PRIMARY);
                                  mmi_ucs2_to_asc((char *)charname,(char *)prof_str_info_qry.dest);
		                    kal_prompt_trace(MOD_SOC,"prof_str_info_qry apn name=%s",charname);   			
		                    //srv_dtcnt_store_delete_prof(acct_id_query.ids[i], MMI_TRUE, NULL, NULL);
	                        }
		  }
		 else 
		 {
		         acct_id_query.qry_info.acc_type_info = SRV_DTCNT_PROF_TYPE_USER_CONF;	   
	                result = srv_dtcnt_store_qry_ids(&acct_id_query);
	
	                if (result == SRV_DTCNT_RESULT_SUCCESS)
	               {
	    	               kal_prompt_trace(MOD_SOC,"user srv_dtcnt_store_qry_ids sucess"); 
	                      kal_prompt_trace(MOD_SOC,"user acct_id_query num_ids=%d",acct_id_query.num_ids);  
		               if(acct_id_query.num_ids>0)
		               {
		                      for (i=0;i<(acct_id_query.num_ids);i++)
	                             {
						kal_prompt_trace(MOD_SOC,"accound id=%d",acct_id_query.ids[i]);
						prof_str_info_qry.dest = (S8*)name;
						prof_str_info_qry.dest_len = sizeof(name);
						srv_dtcnt_get_account_name(acct_id_query.ids[i],&prof_str_info_qry,SRV_DTCNT_ACCOUNT_PRIMARY);
						mmi_ucs2_to_asc((char *)charname,(char *)prof_str_info_qry.dest);
						kal_prompt_trace(MOD_SOC,"prof_str_info_qry name=%s",charname); 
						// get apn			
						prof_str_info_qry.dest = (S8*)name;
						prof_str_info_qry.dest_len = sizeof(name);
						srv_dtcnt_get_apn(acct_id_query.ids[i],&prof_str_info_qry,SRV_DTCNT_ACCOUNT_PRIMARY);
						mmi_ucs2_to_asc((char *)charname,(char *)prof_str_info_qry.dest);
						kal_prompt_trace(MOD_SOC,"prof_str_info_qry apn name=%s",charname);  
						//srv_dtcnt_store_delete_prof(acct_id_query.ids[i], MMI_TRUE, NULL, NULL);			 
	                              }
		                }
	                      else 
		               {
                                    add_new_acc_prof_id(L"BD Internet");
		                     //reboot 		 
		       	       {
					    kal_prompt_trace(MOD_SOC,"Lima_Soc_Init reboot");		   
                                         RJ_SYS_ReStart();
		                   	}	
	                      }			 
	                 }
                        else 
	                {
	                      kal_prompt_trace(MOD_SOC,"srv_dtcnt_store_qry_ids fail = %d",result);		 	 	
	                 }  						  
		}
      }		 	 			 	
      else 
     {
	   kal_prompt_trace(MOD_SOC,"srv_dtcnt_store_qry_ids fail = %d",result);		 	 	
      }   	
        //add by lqy

if(acct_id_query.num_ids>0) {
	  aact_id =  acct_id_query.ids[0];
#if defined(__MMI_DUAL_SIM_MASTER__) && defined(__MTK_TARGET__)//选择卡一，卡二联网
        //index = Lima_plat_Get_Priv_Simcard();
        kal_prompt_trace(MOD_SOC,"aact_id=%d",aact_id); 
        index = Get_Simcard();
        kal_prompt_trace(MOD_USB, "[Lima_Soc_Init] SIM  :Get Sim Id =%d!!",index);
        if ( MMI_SIM1 == index)
        {
            g_lima_gp_socket->nwt_acount_id = cbm_encode_data_account_id(acct_id_query.ids[0], CBM_SIM_ID_SIM1, g_lima_cbm_app_id, 0);
        }
        else if (MMI_SIM2 == index)
        {
            g_lima_gp_socket->nwt_acount_id = cbm_encode_data_account_id(acct_id_query.ids[0], CBM_SIM_ID_SIM2, g_lima_cbm_app_id, 0);
        }
        else
        {
            kal_prompt_trace(MOD_USB, "[Lima_Soc_Init] SIM  :Get Sim Id Or Net Registration Fail!!");
        }
#else
        g_lima_gp_socket->nwt_acount_id = cbm_encode_data_account_id(acct_id_query.ids[0], CBM_SIM_ID_SIM1, g_lima_cbm_app_id, 0);
#endif
}  
        g_lima_nwt_acount_id  = g_lima_gp_socket->nwt_acount_id; 
        kal_prompt_trace(MOD_USB, "[Lima_Soc_Init] g_lima_gp_socket->nwt_acount_id = %d", g_lima_gp_socket->nwt_acount_id);

        //设置默认连接超时时间
        if (-1 == g_lima_timeout_count)
        {
            Lima_Soc_Set_Conn_Timeout(DEFAULT_CONN_TIME_OUT_COUNTER);
        }
        ret = LIMA_SOC_NO_ERROR;

    }
    else
    {
        ret = LIMA_SOC_BUSY;
    }
    
    kal_prompt_trace(MOD_SOC, "[Lima_Soc_Init]  g_lima_gp_socket->socket_id = %d",g_lima_gp_socket->socket_id);
    return ret;
}





/**************************************************************************************************
Function name    :    Bird_Soc_Dinit
Functions        :        释放资源，取消Socket请求
Parameters        :    0               
    input        :    NONE         
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-05-15        create:
**************************************************************************************************/
void Lima_Soc_Dinit(void)
{
     g_lima_is_getip = FALSE;
     Lima_set_soc_conn_flag(FALSE);    
    Lima_set_soc_init_flag(FALSE);
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Dinit] Enter:bird_gps_nwt_acount_id,socket_id=%d,%d",g_lima_nwt_acount_id,g_lima_gp_socket->socket_id);
    
    if ((NULL != g_lima_gp_soc_adm_id) && (NULL != g_lima_gp_soc_pool))
    {
        if (NULL != g_lima_gp_socket)
        {
            if (g_lima_gp_socket->socket_id >= 0)
            {
                soc_close(g_lima_gp_socket->socket_id);
            }
            //SetProtocolEventHandler(NULL, MSG_ID_APP_SOC_NOTIFY_IND);
            //SetProtocolEventHandler(NULL, MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
            
            if (NULL != g_lima_gp_socket->rcvd_buffer)
            {
                kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->rcvd_buffer);
                g_lima_gp_socket->rcvd_buffer = NULL;
            }

            if (NULL != g_lima_gp_socket->snd_buffer)
            {
                kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->snd_buffer);
                g_lima_gp_socket->snd_buffer = NULL;
            }
            
            if (NULL != g_lima_gp_socket->url)
            {
                kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->url);
                g_lima_gp_socket->url = NULL;
            }

            if (NULL != g_lima_gp_socket->file_path)
            {
                kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->file_path);
                g_lima_gp_socket->file_path = NULL;
            }

            if (NULL != g_lima_gp_socket->url_parsed_info)
            {
                if (NULL != g_lima_gp_socket->url_parsed_info->host_name)
                {
                    kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->url_parsed_info->host_name);
                    g_lima_gp_socket->url_parsed_info->host_name = NULL;
                }

                if (NULL != g_lima_gp_socket->url_parsed_info->url_link)
                {
                    kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->url_parsed_info->url_link);
                    g_lima_gp_socket->url_parsed_info->url_link = NULL;
                }

                kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->url_parsed_info);
                g_lima_gp_socket->url_parsed_info = NULL;
            }

            if (NULL != g_lima_gp_socket->callback)
            {
                g_lima_gp_socket->callback = NULL;
            }
            kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket);
            g_lima_gp_socket = NULL;
        }
        
        if (g_lima_gp_soc_adm_id != NULL)
        {
            kal_adm_delete(g_lima_gp_soc_adm_id);
            g_lima_gp_soc_adm_id = NULL;
        }
        if (g_lima_gp_soc_pool != NULL)
        {
            media_free_ext_buffer(MOD_MMI, (void **)&g_lima_gp_soc_pool);
            g_lima_gp_soc_pool = NULL;
        }
    }

    if (NULL != g_lima_pfn_progreess)
    {
        g_lima_pfn_progreess = NULL;
    }

   //ini rev buffer
    g_socket_rev.g_n_rev_index=0;    
   g_socket_rev.g_n_rev_indexEnd=0;    
    memset(g_socket_rev.rev_buf, 0, RECEIVE_DATA_LENGTH);	
	
    kal_prompt_trace(MOD_SOC, "[bird_Soc_Dinit]  g_lima_gp_socket->socket_id = %d",g_lima_gp_socket->socket_id);
    kal_prompt_trace(MOD_USB, "[bird_Soc_Dinit] Leave ");
}

// add by lqy
mmi_sim_enum  Get_Simcard(void)
{     
	U8  status = 0;
	S8 lima_plmn[7];
       mmi_sim_enum sim;
       
       if(lima_srv_sim_card2==Lima_plat_simcard_option())
       {
          sim=MMI_SIM2;
       }
	else if (lima_srv_sim_card1==Lima_plat_simcard_option())
       {
          sim=MMI_SIM1;
       }   
       else
       {
          sim=MMI_SIM_NONE; 
       }
      

       return sim;
    
	 
}
// add by lqy


/*  cancel by lqy
int Lima_plat_Get_Priv_Simcard(void)
{
    U8      g_lima_sms_sim_idx = 1;
    BOOL bSimCard1Exist = FALSE;
    BOOL bSimCard2Exist = FALSE;
    int  nSimCard1NetType = 0;
    int  nSimCard2NetType = 0;
    CHAR   yake_nw_plmn[6+1];  
    srv_nw_info_service_availability_enum ret1=srv_nw_info_get_service_availability(MMI_SIM1) ;
    srv_nw_info_service_availability_enum ret2=srv_nw_info_get_service_availability(MMI_SIM2) ;
    
    if ( ret1 == SRV_NW_INFO_SA_FULL_SERVICE) 
    {
        bSimCard1Exist = TRUE;
        srv_nw_info_get_nw_plmn(1,yake_nw_plmn,7);
             if (memcmp((char *) yake_nw_plmn,"46001",5) == 0)
            {
                //中国联通
                nSimCard1NetType = 2;
            }
            else
            {
                //中国移动
                nSimCard1NetType = 1;
            }
    }
    if ( ret2 == SRV_NW_INFO_SA_FULL_SERVICE) 
    {
        bSimCard2Exist = TRUE;
        srv_nw_info_get_nw_plmn(2,yake_nw_plmn,7);
        if (memcmp((char *) yake_nw_plmn,"46001",5) == 0)
            {
                //中国联通
                nSimCard1NetType = 2;
            }
            else
            {
                //中国移动
                nSimCard1NetType = 1;
            }
    }
    
    if(!bSimCard1Exist && !bSimCard2Exist)
		g_lima_sms_sim_idx = 0;
    else if(bSimCard1Exist && nSimCard1NetType == 1)
		g_lima_sms_sim_idx = 1;
    else if(bSimCard2Exist && nSimCard2NetType == 1)
		g_lima_sms_sim_idx = 2;
    else if(bSimCard1Exist && nSimCard1NetType == 2)
		g_lima_sms_sim_idx = 1;
    else if(bSimCard2Exist && nSimCard2NetType == 2)
		g_lima_sms_sim_idx = 2;
    else if(bSimCard1Exist)
		g_lima_sms_sim_idx = 1;
    else if(bSimCard2Exist)
		g_lima_sms_sim_idx = 2;

	return g_lima_sms_sim_idx;

}*/  //cancel by lqy


/**************************************************************************************************
Function name    :    Lima_Soc_Time_Out
Functions        :        连接(接收)时间超时的回调函数
Parameters        :    0               
    input        :    NONE
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-06-01        create:
GuoJin                      2009-08-07            增加接收数据超时
**************************************************************************************************/
static void Lima_Soc_Time_Out(void)
{
    S16                  result;

    kal_prompt_trace(MOD_USB, "[Lima_Soc_Time_Out]Entry");
    if ((NULL != g_lima_gp_socket) && (NULL != g_lima_gp_socket->callback))
    {
        if (LIMA_SOC_CONNECT == g_lima_gp_socket->timeout_type)
        {
            result = LIMA_SOC_SEND_TIME_OUT;
        }
        else if (LIMA_SOC_SEND == g_lima_gp_socket->timeout_type)
        {
            result = LIMA_SOC_SEND_TIME_OUT;
        }
        else
        {
            result = LIMA_SOC_RCVD_TIME_OUT;
        }

        Lima_set_soc_init_flag(FALSE);//恢复。重新开始
        g_lima_gp_socket->request_id = LIMA_GPS_QUIET;

    }
}



/**************************************************************************************************
Function name    :    Lima_Soc_Write_Continue_Dl_Para
Functions        :        写入因取消或错误中断接收数据的连接参数
Parameters        :    1               
    input        :    soc 
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-09-22        create:
**************************************************************************************************/
void Lima_Soc_Write_Continue_Dl_Para(lima_soc_transaction_struct *soc)
{
#ifdef    LIMA_SOC_CONTINUE_DL
    U16  cfgFile[MAX_LIMA_FILENAME_LEN];
    S32  hCfg;
    U16  task_num;
    U16  index;
    UINT read;
    BOOL bExist;
    lima_soc_task_item_struct *pItem = NULL;
    
    //kal_prompt_trace(MOD_USB, "[Lima_Soc_Write_Continue_Dl_Para]Entry");

    bExist = FALSE;
    //kal_wsprintf((kal_wchar*) cfgFile,LIMA_SOC_TASKS_CFG_FILE,(S16)MMI_CARD_DRV);
    //对比url和文件名称
    hCfg = FS_Open((U16 *)cfgFile, FS_CREATE | FS_READ_WRITE);
    if (hCfg >= FS_NO_ERROR)
    {    
        index    = 0;
        FS_GetFileSize((FS_HANDLE)hCfg, &read);
        task_num = read/sizeof(lima_soc_task_item_struct);
        
        if (task_num > 0)
        {
            pItem    = kal_adm_alloc(g_lima_gp_soc_adm_id, task_num * sizeof(lima_soc_task_item_struct));
            memset(pItem, 0, task_num  * sizeof(lima_soc_task_item_struct));
            FS_Seek(hCfg, 0, FS_FILE_BEGIN);
            FS_Read(hCfg, pItem, task_num * sizeof(lima_soc_task_item_struct), &read);
            
            while (index < task_num)
            {
                if ((0 == mmi_ucs2cmp((const S8*)pItem[index].path, (const S8*)soc->file_path)) && (0 == strcmp(pItem[index].url, (const char*)soc->url)))
                {//如果已经存在不在重复添加
                    bExist = TRUE;
                    break;
                }
                index++;
            }
        }
        else
        {
            pItem    = kal_adm_alloc(g_lima_gp_soc_adm_id,  sizeof(lima_soc_task_item_struct));
            memset(pItem, 0, sizeof(lima_soc_task_item_struct));
        }
        
        if (FALSE == bExist)
        {
            memset(pItem, 0, sizeof(lima_soc_task_item_struct));
            mmi_ucs2ncpy((S8*)pItem[0].path, (S8*)soc->file_path, MAX_LIMA_FILENAME_LEN);
            strncpy(pItem[0].url, (const char*)soc->url, MAX_LIMA_URL_LEN);
            FS_Seek(hCfg, 0, FS_FILE_END);
            FS_Write(hCfg, pItem, sizeof(lima_soc_task_item_struct), &read);
        }
        
        kal_adm_free(g_lima_gp_soc_adm_id,pItem);
        pItem = NULL;
        FS_Close(hCfg);
    }
#endif
}

void Lima_Soc_Del_Continue_Dl_Para(lima_soc_transaction_struct *soc)
{
#ifdef    LIMA_SOC_CONTINUE_DL
    U16  cfgFile[MAX_LIMA_FILENAME_LEN];
    S32  hCfg;
    U16  task_num;
    U16  index;
    UINT read;
    lima_soc_task_item_struct *pItem = NULL;//lrf

    
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Del_Continue_Dl_Para]Entry");

    kal_wsprintf((kal_wchar*) cfgFile,LIMA_SOC_TASKS_CFG_FILE,(S16)MMI_CARD_DRV);
    //对比url和文件名称
    hCfg = FS_Open((U16 *)cfgFile, FS_CREATE | FS_READ_WRITE);
    if (hCfg >= FS_NO_ERROR)
    {    
        index    = 0;
        FS_GetFileSize((FS_HANDLE)hCfg, &read);
        task_num = read/sizeof(lima_soc_task_item_struct);
        if (0 == task_num)
        {
            FS_Close(hCfg);
            return;
        }
        pItem    = kal_adm_alloc(g_lima_gp_soc_adm_id, task_num * sizeof(lima_soc_task_item_struct));
        memset(pItem, 0, task_num * sizeof(lima_soc_task_item_struct));
        FS_Seek(hCfg, 0, FS_FILE_BEGIN);
        FS_Read(hCfg, pItem, task_num * sizeof(lima_soc_task_item_struct), &read);
        
        while (index < task_num)
        {
            if ((0 == mmi_ucs2cmp((const S8*)pItem[index].path, (const S8*)soc->file_path)) && (0 == strcmp(pItem[index].url, (const char*)soc->url)))
            {
                memcpy(&pItem[index], &pItem[index + 1], (task_num - index - 1) * sizeof(lima_soc_task_item_struct));
                
                FS_Seek(hCfg, 0, FS_FILE_BEGIN);
                FS_Write(hCfg, pItem, (task_num - 1) * sizeof(lima_soc_task_item_struct), &read);
                //清除多余部分
                FS_Seek(hCfg, (task_num - 1) * sizeof(lima_soc_task_item_struct), FS_FILE_BEGIN);
                FS_Truncate(hCfg);
                break;
            }
            
            index++;
        }
        
        kal_adm_free(g_lima_gp_soc_adm_id,pItem);
        pItem = NULL;
        FS_Close(hCfg);
    }
   
#endif    
}

void Lima_Soc_Set_Continue_Dl_Para(lima_soc_transaction_struct *soc)
{
#ifdef    LIMA_SOC_CONTINUE_DL
    U16  cfgFile[MAX_LIMA_FILENAME_LEN];
    S32  hCfg;
    S32  hFile;
    U16  task_num;
    U16  index;
    UINT read;
    lima_soc_task_item_struct *pItem = NULL;//lrf
    
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Set_Continue_Dl_Para]Entry");

    task_num = 0;
    kal_wsprintf((kal_wchar*) cfgFile,LIMA_SOC_TASKS_CFG_FILE,(S16)MMI_CARD_DRV);
    //对比url和文件名称
    hCfg = FS_Open((U16 *)cfgFile, FS_CREATE | FS_READ_WRITE);
    if (hCfg >= FS_NO_ERROR)
    {    
        index    = 0;
        pItem    = kal_adm_alloc(g_lima_gp_soc_adm_id, sizeof(lima_soc_task_item_struct));
        FS_GetFileSize((FS_HANDLE)hCfg, &read);
        task_num = read/sizeof(lima_soc_task_item_struct);
        if (0 == task_num)
        {
            FS_Close(hCfg);
            return;
        }
        while (index < task_num)
        {
            memset(pItem, 0, sizeof(lima_soc_task_item_struct));
            FS_Seek(hCfg, index * sizeof(lima_soc_task_item_struct), FS_FILE_BEGIN);
            FS_Read(hCfg, pItem, sizeof(lima_soc_task_item_struct), &read);

            if ((0 == mmi_ucs2cmp((const S8*)pItem->path, (const S8*)soc->file_path)) && (0 == strcmp(pItem->url, (const char*)soc->url)))
            {
                hFile = FS_Open((U16 *)soc->file_path, FS_CREATE | FS_READ_WRITE);
                if (hFile >= FS_NO_ERROR)
                {
                    FS_GetFileSize((FS_HANDLE)hFile, &read);
                    soc->start_point = read;
                    FS_Close(hFile);
                }
                break;
            }
            
            index++;
        }
        
        kal_adm_free(g_lima_gp_soc_adm_id,pItem);
        pItem = NULL;
        FS_Close(hCfg);
    }
#endif    
}




U8 Lima_Soc_is_creating_status(void)
{
    U8 ret = 0;
    if(g_lima_gp_socket)
    {
        if((g_lima_gp_socket->state == LIMA_SOC_CREATING)
            || (g_lima_gp_socket->state == LIMA_SOC_HTTP_DNS_QUERY)
            || (g_lima_gp_socket->state == LIMA_SOC_HTTP_TCP_CON_CREATING)
            )
        {
            ret = 1;
        }
    }
    kal_prompt_trace(MOD_SOC,"[Lima_Soc_is_creating_status] g_lima_gp_socket->state:%d",g_lima_gp_socket->state);
    return ret;
}

static void Lima_Soc_Set_Req_Header(lima_soc_req_header_struct *header)
{
    U16 len = 0;

    if (NULL != header->p_url)
    {
        len = strlen((char*)header->p_url);
        if (len < MAX_LIMA_URL_LEN)
        {
            strncpy((char*)g_lima_gp_socket->url, header->p_url, len);
        }
        else
        {
            kal_prompt_trace(MOD_SOC,"[Lima_Soc_Send_Post_Req] url length is too long !");
        }
    }
    
    if (NULL != header->fullpath)
    {
        if (NULL != g_lima_gp_socket->file_path)
        {
            kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->file_path);
            g_lima_gp_socket->file_path = NULL;
        }
        len =  mmi_ucs2strlen((char*)header->fullpath);
        if (0 == len)//兼容声色短信
        {
            g_lima_gp_socket->file_path = NULL;
        }
        else
        {
            g_lima_gp_socket->file_path = kal_adm_alloc(g_lima_gp_soc_adm_id, (len + 1) * ENCODING_LENGTH);
            ASSERT(g_lima_gp_socket->file_path != NULL);
            memset(g_lima_gp_socket->file_path,  0, (len + 1) * ENCODING_LENGTH);
            mmi_ucs2ncpy((S8*)g_lima_gp_socket->file_path, (S8*)header->fullpath, len);
        }
    }
    
    g_lima_gp_socket->http_method   = header->http_method;
    g_lima_gp_socket->request_id    = header->request_id;
    g_lima_gp_socket->post_data     = (kal_int8*)header->post_data;
    g_lima_gp_socket->post_data_len = header->post_data_len;
#ifdef    LIMA_SOC_CONTINUE_DL
    g_lima_gp_socket->need_continue_dl = header->need_continue_dl;
    if (1 == g_lima_gp_socket->need_continue_dl)
    {
        Lima_Soc_Set_Continue_Dl_Para(g_lima_gp_socket);
    }
#endif
}

static U16 Lima_Soc_Create_Socket(void)
{
    kal_uint8 val = 1;

    g_lima_gp_socket->state = LIMA_SOC_CREATING;
    g_lima_gp_socket->socket_id= soc_create(PF_INET, SOCK_STREAM, 0, MOD_MMI, g_lima_gp_socket->nwt_acount_id);
    kal_prompt_trace(MOD_SOC,"[Lima_Soc_Create_Socket] Enter g_lima_gp_socket->socket_id = %d",g_lima_gp_socket->socket_id);
    if (g_lima_gp_socket->socket_id >= 0)
    {
        if (soc_setsockopt(g_lima_gp_socket->socket_id, SOC_NBIO, &val, sizeof(val)) < 0)
        {
            kal_prompt_trace(MOD_SOC,"[Lima_Soc_Create_Socket] nonblock mode error !!");
            return LIMA_SOC_CREATE_ERR;
        }
        val = SOC_READ | SOC_WRITE | SOC_CLOSE | SOC_CONNECT;
        if (soc_setsockopt(g_lima_gp_socket->socket_id, SOC_ASYNC, &val, sizeof(val)) < 0)
        {
            kal_prompt_trace(MOD_SOC,"[Lima_Soc_Create_Socket] set socket mode error !!");
            return LIMA_SOC_CREATE_ERR;
        }
        g_lima_gp_socket->state = LIMA_SOC_CREATED;
        return LIMA_SOC_NO_ERROR;
    }
    else
    {
        return LIMA_SOC_CREATE_ERR;
    }
}



static void Lima_Soc_Http_Get_Parsed_Url(const S8* url, lima_soc_http_url_struct *p_http_url)
{
    S8 *hostname = NULL;
    S8 *slash    = NULL;
    S8 *colon    = NULL;

    kal_prompt_trace(MOD_SOC, "[Lima_Soc_Http_Get_Parsed_Url] Entry");
    kal_prompt_trace(MOD_SOC,"Lima_Soc_Http_Get_Parsed_Url url%s  \n",url); 
    hostname = strstr((char*)app_strtolower((kal_char*)url), "http://");

    if (NULL != hostname)
    {
        hostname += 7;  /* strlen(http://) */
    }
    else{
	  hostname = 	(S8*)url;
    }
        slash = strchr(hostname, '/');
	  kal_prompt_trace(MOD_SOC, "[Lima_Soc_Http_Get_Parsed_Url] slash %d",slash);
        if (NULL == slash)
        {
            colon = strchr(hostname, ':');
		kal_prompt_trace(MOD_SOC, "[Lima_Soc_Http_Get_Parsed_Url] colon %d",colon);
            if (colon)
            {
            	   
                p_http_url->host_name_len = (colon - hostname);
                if (NULL != p_http_url->host_name)
                {
                    kal_adm_free(g_lima_gp_soc_adm_id,p_http_url->host_name);
                    p_http_url->host_name = NULL;
                }
                p_http_url->host_name = kal_adm_alloc(g_lima_gp_soc_adm_id,(U16) (p_http_url->host_name_len + 1));
                if (!p_http_url->host_name)
                {
                    return;
                }
                memset(p_http_url->host_name, 0, p_http_url->host_name_len + 1);
                memcpy(p_http_url->host_name, hostname, p_http_url->host_name_len);
                kal_prompt_trace(MOD_SOC, "[Lima_Soc_Http_Get_Parsed_Url] %s",p_http_url->host_name);    
                colon++;
                p_http_url->ser_port_num = atoi(colon);
				kal_prompt_trace(MOD_SOC, "[Lima_Soc_Http_Get_Parsed_Url] %d",p_http_url->ser_port_num); 
            }
            else
            {
                p_http_url->ser_port_num = 80;
                p_http_url->host_name_len = strlen(hostname);
                if (NULL != p_http_url->host_name)
                {
                    kal_adm_free(g_lima_gp_soc_adm_id,p_http_url->host_name);
                    p_http_url->host_name = NULL;
                }                
                p_http_url->host_name = kal_adm_alloc(g_lima_gp_soc_adm_id,(U16) (p_http_url->host_name_len + 1));
                if (!p_http_url->host_name)
                {
                    return;
                }
                memset(p_http_url->host_name, 0, p_http_url->host_name_len + 1);
                memcpy(p_http_url->host_name, hostname, p_http_url->host_name_len);
            }
        }
        else
        {
            colon = strchr(hostname, ':');
            
            if (colon && colon < slash)
            {
                char *port_str;

                p_http_url->host_name_len = (colon - hostname);
                if (NULL != p_http_url->host_name)
                {
                    kal_adm_free(g_lima_gp_soc_adm_id,p_http_url->host_name);
                    p_http_url->host_name = NULL;
                }                
                p_http_url->host_name = kal_adm_alloc(g_lima_gp_soc_adm_id,(U16) (p_http_url->host_name_len + 1));
                if (!p_http_url->host_name)
                {
                    return;
                }
                memset(p_http_url->host_name, 0, p_http_url->host_name_len + 1);
                memcpy(p_http_url->host_name, hostname, p_http_url->host_name_len);

                port_str = kal_adm_alloc(g_lima_gp_soc_adm_id,(U16) (slash - colon));
                if (!port_str)
                {
                    return;
                }
                memset(port_str, 0, slash - colon);
                strncpy(port_str, colon + 1, slash - colon - 1);
                p_http_url->ser_port_num = atoi(port_str);
                kal_adm_free(g_lima_gp_soc_adm_id, port_str);
            }
            else
            {
                p_http_url->ser_port_num = 80;
                p_http_url->host_name_len = slash - hostname;
                if (NULL != p_http_url->host_name)
                {
                    kal_adm_free(g_lima_gp_soc_adm_id,p_http_url->host_name);
                    p_http_url->host_name = NULL;
                }                
                p_http_url->host_name = kal_adm_alloc(g_lima_gp_soc_adm_id,(U16) (p_http_url->host_name_len + 1));
                if (!p_http_url->host_name)
                {
                    return;
                }
                memset(p_http_url->host_name, 0, p_http_url->host_name_len + 1);
                memcpy(p_http_url->host_name, hostname, p_http_url->host_name_len);
            }
            p_http_url->url_link_len = strlen(slash);
            if (NULL != p_http_url->url_link)
            {
                kal_adm_free(g_lima_gp_soc_adm_id,p_http_url->url_link);
                p_http_url->url_link = NULL;
            }
            p_http_url->url_link = kal_adm_alloc(g_lima_gp_soc_adm_id,(U16) (p_http_url->url_link_len + 1));
            if (!p_http_url->url_link)
            {
                return;
            }
            memset(p_http_url->url_link, 0, p_http_url->url_link_len + 1);
            memcpy(p_http_url->url_link, slash, p_http_url->url_link_len);
        }
}



BOOL Lima_Soc_Get_Server_Ip()
{
   kal_uint8 *host_name=g_lima_gp_socket->url_parsed_info->host_name;
   kal_uint8 i=0,j=0,k=0;
   kal_uint8 ip_buff[3]={0};
     kal_prompt_trace(MOD_SOC, "[Lima_Soc_Get_Server_Ip] entry");
    if((g_lima_gp_socket->server_ip_addr.addr[0] == 0)
        ||(g_lima_gp_socket->server_ip_addr.addr[0] == '0')||(g_lima_gp_socket->server_ip_addr.addr[0] == NULL)
        )
   {
        return FALSE; //need dns
   }

   g_lima_gp_socket->server_ip_addr.addr_len=0;
   while (host_name[i])
   {
         kal_prompt_trace(MOD_SOC, "[Lima_Soc_Get_Server_Ip] host_name %d",host_name[i]);
         if (host_name[i]!='.')
	  {
		  ip_buff[j]=host_name[i];
		  j++;
		  if (g_lima_gp_socket->server_ip_addr.addr_len==3)
          {
                g_lima_gp_socket->server_ip_addr.addr[g_lima_gp_socket->server_ip_addr.addr_len]=atoi(host_name+i);
		        g_lima_gp_socket->server_ip_addr.addr_len++;
                break;
         }
	  }
      else
	  {
                g_lima_gp_socket->server_ip_addr.addr[g_lima_gp_socket->server_ip_addr.addr_len]=atoi(ip_buff);
                j=0;
				g_lima_gp_socket->server_ip_addr.addr_len++;
				memset(ip_buff,0,3);
	  }
	  i++;
   }  
   return TRUE;
}


/**************************************************************************************************
Function name    :    Lima_Soc_Get_Host_By_Name
Functions        :        解析主机名
Parameters        :    1               
    input        :    hostname        主机名 
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-05-15        create:只用于CMNET
**************************************************************************************************/
static void Lima_Soc_Get_Host_By_Name(const S8 *hostname)
{
    kal_int8                   ret = 0;
    static S32 sSocketReq = SOCKETREQID;

    kal_prompt_trace(MOD_SOC,"[Lima_Soc_Get_Host_By_Name] Entry hostname = %s", hostname);
    ret = soc_gethostbyname(KAL_FALSE,
                            MOD_MMI,
                            (kal_int32) sSocketReq,//request_id,app_id,可能需要修改
                            (kal_char*) hostname,
                            (kal_uint8*) g_lima_gp_socket->rcvd_buffer,
                            (kal_uint8*) & g_lima_gp_socket->rcvd_counter,
                            0,
                            g_lima_gp_socket->nwt_acount_id);

    kal_prompt_trace(MOD_SOC, "[Lima_Soc_Get_Host_By_Name] ret = %d", ret);
    
    g_lima_gp_socket->server_ip_addr.port = g_lima_gp_socket->url_parsed_info->ser_port_num;
    if (SOC_SUCCESS == ret)
    {
        memcpy(g_lima_gp_socket->server_ip_addr.addr, g_lima_gp_socket->rcvd_buffer, 4);
        g_lima_gp_socket->rcvd_counter = 0;
        g_lima_gp_socket->server_ip_addr.addr_len = 4;
        g_lima_gp_socket->state = LIMA_SOC_HTTP_TCP_CON_CREATING;
        Bird_soc_connbyIp();  //调用连接
    }
    else if (SOC_WOULDBLOCK == ret)
    { /* waits for APP_SOC_GET_HOST_BY_NAME_IND */
        SetProtocolEventHandler(Lima_Soc_Get_Host_By_Name_Ind, MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
    }
    else
    {
        kal_prompt_trace(MOD_SOC,"[Lima_Soc_Get_Host_By_Name]LIMA_SOC_GET_HOST_ERR");
    }
}



/**************************************************************************************************
Function name    :    Lima_Soc_Get_Host_By_Name_Ind
Functions        :        解析主机名，MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND回调函数
Parameters        :    0               
    input        :    NONE 
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-05-15        create:只用于CMNET
**************************************************************************************************/
static MMI_BOOL Lima_Soc_Get_Host_By_Name_Ind(void *inMsg)
{
    app_soc_get_host_by_name_ind_struct *dns_ind = NULL; //lrf

    kal_prompt_trace(MOD_SOC, "[Lima_Soc_Get_Host_By_Name_Ind] entry");
    dns_ind = (app_soc_get_host_by_name_ind_struct*) inMsg;
    if(dns_ind->request_id==(kal_int32)SOCKETREQID)
    {

    memcpy(g_lima_gp_socket->server_ip_addr.addr, dns_ind->addr, dns_ind->addr_len);
    mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND, (PsIntFuncPtr)Lima_Soc_Get_Host_By_Name_Ind);
    kal_prompt_trace(MOD_SOC, "[Lima_Soc_Get_Host_By_Name_Ind]Http send request to %d.%d.%d.%d and port: %d",
                    g_lima_gp_socket->server_ip_addr.addr[0],
                    g_lima_gp_socket->server_ip_addr.addr[1],
                    g_lima_gp_socket->server_ip_addr.addr[2],
                    g_lima_gp_socket->server_ip_addr.addr[3],
                    g_lima_gp_socket->server_ip_addr.port);
    
    g_lima_gp_socket->rcvd_counter = 0;
    g_lima_gp_socket->server_ip_addr.addr_len = 4;
    g_lima_gp_socket->state = LIMA_SOC_HTTP_TCP_CON_CREATING;
    Bird_soc_connbyIp();  //调用连接
              return KAL_TRUE;		   
     }
     else{
		return KAL_FALSE;
     }

}

static MMI_BOOL Lima_Soc_Socket_Notify(void *inMsg)
{
    U8 nrestresion=0;
    app_soc_notify_ind_struct       *soc_notify = (app_soc_notify_ind_struct*) inMsg;
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Socket_Notify]Entry");
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Socket_Notify]soc_notify->event_type = %d", soc_notify->event_type);
    //添加异步处理2009-06-11
    if ((NULL == g_lima_gp_socket)||(soc_notify->socket_id != g_lima_gp_socket->socket_id))
    {//假如是已经处理完的socket,不在处理
        kal_prompt_trace(MOD_USB, "[Lima_Soc_Socket_Notify]notify->socket_id = %d, g_lima_gp_socket->socket_id = %d", soc_notify->socket_id, g_lima_gp_socket->socket_id);
        return FALSE;
    }

    switch (soc_notify->event_type)
    {
        case SOC_CONNECT:
        {
            if (soc_notify->result == KAL_TRUE)
            {
            	//LED闪烁
		rj_led_status_info.b_SOC_IS_CONNECTED = KAL_TRUE;
		rj_led_status_info.b_SOC_IS_CONNECTING = KAL_FALSE;
		
                 Lima_set_soc_init_flag(TRUE);	
  		   Lima_set_soc_conn_flag(TRUE);
		   Lima_set_soc_send_flag(BIRD_SOC_NULL);
		   nrestresion = Bird_get_reseterr();
		   if(nrestresion!=0){
			  Bird_set_reseterr(BIRD_NORMAL);
			  yd_send_save_nv_msg();
		   }
		   Bird_save_ip();		   
		   g_n_reconn = 0;
		   //StartTimer(BIRD_SOCKET_SEND, RJ_GPS_APP_10S, Bird_soc_send);
		   Rj_stop_timer(BIRD_TASK_SOCKET_SEND);
		   Rj_start_timer(BIRD_TASK_SOCKET_SEND, 1000, Bird_soc_send,NULL);
		   ClearProtocolEventHandler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
            }
            else
            {//手机卡余额不足error_cause = -14，detail_cause = 3183
                kal_prompt_trace(MOD_USB, "[Lima_Soc_Socket_Notify] SOC_CONNECT error_cause = %d,detail_cause = %d", soc_notify->error_cause, soc_notify->detail_cause);
                if(SOC_BEARER_FAIL == soc_notify->error_cause)
                {
                   Lima_set_soc_init_flag(MMI_FALSE);
                }
            }
        }
        break;
        case SOC_WRITE:
        {
            Bird_soc_send();
        }
        break;
        case SOC_READ:
        {
	      Bird_soc_set_revflag(TRUE);
	      
	      //Lima_set_soc_rev_state(1); //收到数据标志
             Lima_Soc_Recv_Response();
        }
        break;
        case SOC_CLOSE:
        {//RCVD block后调用MSG_ID_APP_SOC_NOTIFY_IND回调，当作接收完成处理
            
            kal_prompt_trace(MOD_USB, "g_lima_gp_socket->rcvd_data_len:%d",g_lima_gp_socket->rcvd_data_len);
            if (0 == g_lima_gp_socket->rcvd_data_len)
            {

            }
            kal_prompt_trace(MOD_USB, "[Lima_Soc_Socket_Notify] SOC_CLOSE");
            Lima_set_soc_init_flag(FALSE);
  	     Lima_set_soc_conn_flag(FALSE);
            Lima_Soc_Dinit();
        }
        break;
        default:
            kal_prompt_trace(MOD_USB, "[Lima_Soc_Socket_Notify] default");
            //en_soc_output_result(EN_SOC_ERROR, NULL, 0);
            break;
    }
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Socket_Notify] End");
}


/**************************************************************************************************
Function name    :    Lima_Soc_Recv_Response
Functions        :        Socket接收回传数据
Parameters        :    0               
    input        :    NONE 
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-05-15        create:
**************************************************************************************************/
static void Lima_Soc_Recv_Response(void)
{
	S8    *p_rcvd_buf = NULL;  
    S8  g_yake_ReLocationUrl[MAX_LIMA_URL_LEN]={0};
    S32 fd;
    S16 path[32]={0};    
    
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Recv_Response] entry");
    if(g_lima_gp_socket)
    {
        kal_prompt_trace(MOD_USB, "Lima_Soc_Recv_Response g_lima_gp_socket->state= %d",g_lima_gp_socket->state);
    }
    if (g_lima_gp_socket && g_lima_gp_socket->state < LIMA_SOC_RSP_RCVD)
    {
        kal_int32 ret_val = 0;
        g_lima_gp_socket->state = LIMA_SOC_RSP_RCVING;

        //do 
        //{
            //lrf test
            	g_lima_gp_socket->rcvd_data_len = 0 ;
        	g_lima_gp_socket->rcvd_head_len = 0 ;
        	g_lima_gp_socket->rcvd_total_len = 0 ;
        	g_lima_gp_socket->rcvd_counter = 0 ;	  
		
              kal_prompt_trace(MOD_USB, "[Lima_Soc_Recv_Response]Write To Buffer");
              ret_val = soc_recv(g_lima_gp_socket->socket_id, (kal_uint8*)g_lima_gp_socket->rcvd_buffer,
                                MAX_LIMA_RCV_BUFFER_SIZE,
                                0);

            	kal_prompt_trace(MOD_USB, "[Lima_Soc_Recv_Response]ret_val=  %d", ret_val);
            
            	if (ret_val > 0)
            	{
                	//处理第一次接收
                	if (g_lima_gp_socket->rcvd_counter == 0)
                	{
                    		S8 chlwr[1024];//得到消息头中信息
                    		S8 *pCh = NULL;
                    
                    		memset(chlwr, 0, sizeof(chlwr));
                    		strncpy((char*)chlwr, (char*)g_lima_gp_socket->rcvd_buffer, sizeof(chlwr) - 1);
                    		Lima_Soc_Ascii_Lwr(chlwr);
                    
                    		//增加对chunked方式的处理，预先不知道数据体长度2009-06-12(HTTP1.1)
                    		g_lima_gp_socket->is_chunk_encoding = Lima_Soc_Is_Chunk_Encoding(chlwr);
                    		g_lima_gp_socket->rcvd_head_len     = Lima_Soc_Get_Rcvd_Head_Len(chlwr, g_lima_gp_socket->is_chunk_encoding);

                    		//计算头和数据体长度
                    		if (FALSE == g_lima_gp_socket->is_chunk_encoding)
                    		{
                        		g_lima_gp_socket->rcvd_data_len  = Lima_Soc_Get_Rcvd_Data_Len(chlwr);
                        		g_lima_gp_socket->rcvd_total_len = g_lima_gp_socket->rcvd_data_len + g_lima_gp_socket->rcvd_head_len;
                    		}
                    
                   		 //得到HTTP状态码
                    		//g_lima_gp_socket->http_status = Lima_Soc_Get_Http_Rsp_Status(chlwr);
                    		g_lima_gp_socket->rcvd_data_len = g_lima_gp_socket->rcvd_total_len = g_lima_gp_socket->rcvd_counter+= ret_val;
                   		g_lima_gp_socket->http_status = 200;

                    		kal_prompt_trace(MOD_USB, "[Lima_Soc_Recv_Response]First Recv  rcvd_data_len = %d,rcvd_head_len = %d, rcvd_total_len = %d", g_lima_gp_socket->rcvd_data_len,g_lima_gp_socket->rcvd_head_len,g_lima_gp_socket->rcvd_total_len);
    
                	}
               	 g_lima_gp_socket->rcvd_once_len = ret_val;
                     Lima_Soc_Write_Recv_buf(g_lima_gp_socket);
             		g_lima_gp_socket->rcvd_counter += ret_val;               	
                
            	}
            
        //}while(ret_val > 0);
        
        //memset(g_lima_gp_socket->rcvd_buffer, 0, MAX_LIMA_RCV_BUFFER_SIZE);
        if (ret_val < 0)
        {
        	if (ret_val == SOC_WOULDBLOCK)
            	{
               	mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)Lima_Soc_Socket_Notify);
		       mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,(PsIntFuncPtr)Lima_Soc_Socket_Notify,MMI_TRUE);	
            	}
        }
        else if (ret_val == SOC_SUCCESS)
        {
	     	//memset(g_lima_gp_socket->rcvd_buffer, 0, MAX_LIMA_RCV_BUFFER_SIZE);
        }
    }
    else
    {
        kal_prompt_trace(MOD_USB, "[Lima_Soc_Recv_Response]ERROR!!!!!!!! ");
        Lima_set_soc_init_flag(FALSE);
        Lima_Soc_Dinit();                
    }
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Recv_Response]End ");
}

static S8* Lima_Soc_Ascii_Lwr(S8* upcase)
{
#ifdef LIMA_SOC_SLIM
    app_strtolower(upcase);
#else
    S8 *pCh = NULL; //lrf
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Ascii_Lwr]Entry");

    pCh = upcase;
    while (*pCh != '\0')
    {
        if ((65 <= *pCh) && (*pCh <= 90))
        {
            *pCh = *pCh + 32;
        }
        pCh++;
    }
#endif
    return upcase;
}


static U8 Lima_Soc_Is_Chunk_Encoding(S8 *chlwr)
{
    S8 *pCh     = NULL;
    U8 isChunk = FALSE;

    pCh = strstr(chlwr, "transfer-encoding");
    if (NULL != pCh)
    {
        pCh += 1;
        while((*pCh == ' ') || (*pCh == ':'))
        {
            pCh += 1;
        }

        pCh = strstr(pCh, "chunked");
        if (NULL != pCh)
        {
            isChunk = TRUE;
        }
    }

    return isChunk;
}



/**************************************************************************************************
Function name    :    Lima_Soc_Get_Rcvd_Head_Len
Functions        :        得到回传数据消息头长度
Parameters        :    2               
    input        :    chlwr       服务器返回数据(小写字母)
                    is_chunk  Transfer-Encoding是否为chunked
    output        :    NONE
    return        :    len          消息头长度
< Author >            < Date >              < Notes >
GuoJin                2009-05-15        create:
**************************************************************************************************/
static U32 Lima_Soc_Get_Rcvd_Head_Len(S8 *chlwr, U8 is_chunk)
{
    U32 len  = 0;
    S8  *pCh = NULL;
    //"chunked\r\n\r\n"
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Get_Rcvd_Head_Len] entry");
    if (NULL != chlwr)
    {
        pCh = chlwr;
        if (TRUE == is_chunk)
        {
            pCh = strstr(chlwr, "chunked");
        }
        
        pCh = strstr(pCh, "\r\n\r\n");//消息头结束符
        if (NULL != pCh)
        {
            len = pCh - chlwr + strlen("\r\n\r\n");
        }
    }

    return len;
}



/**************************************************************************************************
Function name    :    Lima_Soc_Get_Rcvd_Data_Len
Functions        :        得到回传数据内容长度
Parameters        :    1               
    input        :    chlwr       服务器返回数据(小写字母)
    output        :    NONE
    return        :    len          数据内容长度
< Author >            < Date >              < Notes >
GuoJin                2009-05-15        create:
GuoJin                2009-05-22             note  :"Content-length:"大小写及后跟空格问题
**************************************************************************************************/
static U32 Lima_Soc_Get_Rcvd_Data_Len(S8 *chlwr)
{
    U32 len    = 0;
    S8  *pCh   = NULL;  

    kal_prompt_trace(MOD_USB, "[Lima_Soc_Get_Rcvd_Data_Len]Entry");
    pCh = strstr(chlwr, "content-length");
    if (NULL != pCh)
    {
        pCh = pCh + strlen("content-length") + 1;
        while((*pCh == ' ') || (*pCh == ':'))
        {
            pCh += 1;
        }
        
        len = atoi(pCh);
    }

    return len;
}



/**************************************************************************************************
Function name    :    Lima_Soc_Get_Http_Rsp_Status
Functions        :        得到HTTP 状态码
Parameters        :    1               
    input        :    chlwr  服务器返回数据
    output        :    NONE
    return        :    status          HTTP状态码
< Author >            < Date >              < Notes >
GuoJin                2009-05-22        create:
**************************************************************************************************/
static U16 Lima_Soc_Get_Http_Rsp_Status(S8 *chlwr)
{
    U16 status    = 0;
    S8  *pCh      = NULL;
    kal_prompt_trace(MOD_USB, "[Lima_Soc_Get_Http_Rsp_Status] entry");
    pCh = chlwr + strlen("HTTP/1.1") + 1;//或"HTTP/1.0 "取得状态码 
    while(*pCh == ' ')
    {
        pCh += 1;
    }
    status = atoi(pCh);
    return status;
}




static void Lima_Soc_Reyake_Request(void)
{
    U16 ret = 0;
    
    if (g_lima_gp_socket->socket_id >= 0)
    {
        soc_close(g_lima_gp_socket->socket_id);
    }
	kal_prompt_trace(MOD_SOC, "[Lima_Soc_Init]  g_lima_gp_socket->socket_id = %d",g_lima_gp_socket->socket_id);
       SetProtocolEventHandler(NULL, MSG_ID_APP_SOC_NOTIFY_IND);
       SetProtocolEventHandler(NULL, MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);
    
    //if (TRUE == g_lima_gb_timer_started)
    //{
     //   StopTimer(BIRD_GPRS_TIMEOUT_TIMER);
     //   g_lima_gb_timer_started = FALSE;
    //}
    
    ret = Lima_Soc_Create_Socket();
    if (LIMA_SOC_CREATE_ERR != ret)
    {
        //CMWAP方式主机域名由代理服务器解析
        if (LIMA_SOC_CMWAP == g_lima_gp_socket->nwt_acount_type)
        {
            g_lima_gp_socket->state = LIMA_SOC_HTTP_TCP_CON_CREATING;
        }
        else
        {
            if (Lima_Soc_Get_Server_Ip()==FALSE)   //dns connect
            {
                    g_lima_gp_socket->state = LIMA_SOC_HTTP_DNS_QUERY;
                    Lima_Soc_Get_Host_By_Name((const S8*)g_lima_gp_socket->url_parsed_info->host_name);
            }
            else
            {
                    g_lima_gp_socket->server_ip_addr.port=g_lima_gp_socket->url_parsed_info->ser_port_num;                    
                     g_lima_gp_socket->state = LIMA_SOC_HTTP_TCP_CON_CREATING;
            }
        }
    }
    else
    {
        kal_prompt_trace(MOD_USB, "[Lima_Soc_Send_Get_Req] LIMA_SOC_CREATE_ERR");
    }
}


static BOOL Lima_Soc_Handle_Location_Url()
{
	 char CRLF[3] = {0x0d, 0x0a, 0x00};//   "/r/n"
     S8 *begin_offset =NULL;
	 S8 *end_offset =NULL;
       S32 len=strlen("location:"),len1=0;
     
	 if (NULL != g_lima_gp_socket->url)
     {
                    kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->url);
                    g_lima_gp_socket->url = NULL;
     }
     g_lima_gp_socket->url = kal_adm_alloc(g_lima_gp_soc_adm_id,MAX_LIMA_URL_LEN);
     if (!g_lima_gp_socket->url)
		return FALSE;
	 memset(g_lima_gp_socket->url,0,MAX_LIMA_URL_LEN);
	 memcpy(g_lima_gp_socket->url,g_lima_gp_socket->rcvd_buffer,g_lima_gp_socket->rcvd_total_len);

	 if (NULL != g_lima_gp_socket->url_parsed_info->url_link)
     {
                    kal_adm_free(g_lima_gp_soc_adm_id,g_lima_gp_socket->url_parsed_info->url_link);
                    g_lima_gp_socket->url_parsed_info->url_link = NULL;
     }
     g_lima_gp_socket->url_parsed_info->url_link = kal_adm_alloc(g_lima_gp_soc_adm_id,(U16) (g_lima_gp_socket->rcvd_total_len + 1));
     if (!g_lima_gp_socket->url_parsed_info->url_link)
		return FALSE;
	 memset(g_lima_gp_socket->url_parsed_info->url_link,0,g_lima_gp_socket->rcvd_total_len + 1);

	 begin_offset=strstr(app_strtolower(g_lima_gp_socket->rcvd_buffer),"location:");  //找到location位置    	 

     if (begin_offset!=NULL)
     {
		         end_offset=strstr(app_strtolower(begin_offset),CRLF);  //找到location位置
				 if (end_offset==NULL)
					 end_offset= g_lima_gp_socket->rcvd_buffer+g_lima_gp_socket->rcvd_total_len-1;
	             
				 len1 = begin_offset-g_lima_gp_socket->rcvd_buffer+len;	
				 
#if 0//def WIN32
		 		 g_lima_gp_socket->url_parsed_info->url_link_len=strlen(temp);
				 memcpy(g_lima_gp_socket->url_parsed_info->url_link,temp,g_lima_gp_socket->url_parsed_info->url_link_len);				 
#else
		 		 g_lima_gp_socket->url_parsed_info->url_link_len=end_offset-begin_offset-len;
		         memcpy(g_lima_gp_socket->url_parsed_info->url_link,(g_lima_gp_socket->url+len1),g_lima_gp_socket->url_parsed_info->url_link_len);				 
#endif
                 if (g_lima_gp_socket->snd_buffer != NULL)
                     memset(g_lima_gp_socket->snd_buffer,0,MAX_LIMA_SND_BUFFER_SIZE );
                  if (g_lima_gp_socket->url!= NULL)
                     memset(g_lima_gp_socket->url,0,MAX_LIMA_URL_LEN );
		   return TRUE;
      }	 
      else
                 return FALSE;
}



/*****************************************************************************
 * FUNCTION
 *  Lima_Dynamic_Check_Download_Res
 * DESCRIPTION
 *  判断是否是图铃下载的函数
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
BOOL Lima_Dynamic_Check_Download_Res(S8 *url)
{
     /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
     BOOL res_need_download = FALSE;
     S8 tempURL[LIMA_DYNAMIC_URL_LEN];
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    memset(tempURL,0,LIMA_DYNAMIC_URL_LEN);
    memcpy(tempURL,url,strlen(url));   
    
    //图片格式
    if(NULL != strstr(app_strtolower(tempURL), ".jpg"))
    {
        res_need_download = TRUE;
    }
	else if(NULL != strstr(app_strtolower(tempURL), ".jpeg"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".gif"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".bmp"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".png"))
    {
        res_need_download = TRUE;
    }
    //视频格式
    else if(NULL != strstr(app_strtolower(tempURL), ".3gp"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".avi"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".mp4"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".wmv"))
    {
        res_need_download = TRUE;
    }
    //音频格式
    else if(NULL != strstr(app_strtolower(tempURL), ".mid"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".amr"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".mp3"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".wav"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".awb"))
    {
        res_need_download = TRUE;
    }
    //其它格式
    else if(NULL != strstr(app_strtolower(tempURL), ".jar"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".lrc"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".sisx"))
    {
        res_need_download = TRUE;
    }
	else if(NULL != strstr(app_strtolower(tempURL), ".ssi"))
    {
        res_need_download = TRUE;
    }
    else if(NULL != strstr(app_strtolower(tempURL), ".vxp"))
    {
        res_need_download = TRUE;
    }
    else
    {
        res_need_download = FALSE;
    }
	return res_need_download;
}


static U32 Lima_Soc_Decode_Chunk_Data(S8 *buffer, U32 data_len)
{
    S8    *pBegin = NULL; //lrf
    S8    *pNext= NULL; //lrf
    U32   size;
    U32   len  = 0;/*接收到的真实数据总长度*/

    pBegin  = buffer;
#ifdef LIMA_SOC_SLIM    
    size    = atoi(pBegin);
#else
    size    = Lima_Soc_Hex_Atoi(pBegin);
#endif

    while (size != 0)//读到0结束
    {
        pNext    = strstr(pBegin, "\r\n") + strlen("\r\n");//一节的结束
        memcpy(buffer + len, pNext, size);
        len     += size;
        pBegin  = pNext + size + strlen("\r\n") ;
#ifdef LIMA_SOC_SLIM    
    size    = atoi(pBegin);
#else        
     size    = Lima_Soc_Hex_Atoi(pBegin);
#endif
    }
    //将之后的数据归零
    memset(pBegin, 0, data_len - len);

    return len;
}



void Bird_soc_rev_command_deal(U8 *p_receive_data)
{
	U16 command_data;
	U8 nflow=0;
	U16 recommand_data;
       U8 *p = p_receive_data;
	U8 i = 0;
        U8 str1[40] = {0};
        U8 str2[40] = {0};
        U8 str3[120] = {0};     
        U8 str4[500] = {0};    
        U8 str5[40] = {0};
        U8 str6[40] = {0};
        U8 str7[40] = {0};     
        U8 str8[40] = {0};  
	  S8 * psplit=NULL;
        S8 p1[50];
	  S8 * p2=NULL;
	  
	  kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal  %s", p_receive_data);
	  p2 = p_receive_data;
    	  psplit = strchr(p2, ',');
	  while(psplit!=NULL)
	  {
	  	memset(p1,0,sizeof(p1));
	  	memcpy(p1, p2,strlen(p2) - strlen(psplit));
		p2 = psplit+1;
		psplit = strchr(p2, ',');
		if(i==0){
			memset(str1,0,sizeof(str1));
	  		memcpy(str1, p1,strlen(p1));
		}
		else if(i==1){
			memset(str2,0,sizeof(str2));
	  		memcpy(str2, p1,strlen(p1));
		}
		else if(i==2){
			memset(str3,0,sizeof(str3));
	  		memcpy(str3, p1,strlen(p1));
		}
		else if(i==3){
			memset(str4,0,sizeof(str4));
	  		memcpy(str4, p1,strlen(p1));
		}
		else if(i==4){
			memset(str5,0,sizeof(str5));
	  		memcpy(str5, p1,strlen(p1));
		}
		else if(i==5){
			memset(str6,0,sizeof(str6));
	  		memcpy(str6, p1,strlen(p1));
		}
		else if(i==6){
			memset(str7,0,sizeof(str7));
	  		memcpy(str7, p1,strlen(p1));
		}
		else if(i==7){
			memset(str8,0,sizeof(str8));
	  		memcpy(str8, p1,strlen(p1));
		}
		i++;
	  }
	  if(p2!=NULL){
		if(i==0){
			memset(str1,0,sizeof(str1));
	  		memcpy(str1, p2,strlen(p2));
		}
		else if(i==1){
			memset(str2,0,sizeof(str2));
	  		memcpy(str2, p2,strlen(p2));
		}
		else if(i==2){
			memset(str3,0,sizeof(str3));
	  		memcpy(str3, p2,strlen(p2));
		}
		else if(i==3){
			memset(str4,0,sizeof(str4));
	  		memcpy(str4, p2,strlen(p2));
		}
		else if(i==4){
			memset(str5,0,sizeof(str5));
	  		memcpy(str5, p2,strlen(p2));
		}
		else if(i==5){
			memset(str6,0,sizeof(str6));
	  		memcpy(str6, p2,strlen(p2));
		}
		else if(i==6){
			memset(str7,0,sizeof(str7));
	  		memcpy(str7, p2,strlen(p2));
		}
		else if(i==7){
			memset(str8,0,sizeof(str8));
	  		memcpy(str8, p2,strlen(p2));
		}
	  }
	//sscanf(p_receive_data, "%40[^,],%40[^,],%40[^,],%500[^,]%40[^,],%40[^,],%40[^,],%40[^,]",str1,str2,str3,str4,str5,str6,str7,str8);
        
                kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal str1 %s", str1);
                kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal str2 %s", str2);
                kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal str3 %s", str3);
                kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal str4 %s", str4);
		   kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal str5 %s", str5);
                kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal str6 %s", str6);
                kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal str7 %s", str7);
                kal_prompt_trace(  MOD_SOC,"Bird_soc_rev_command_deal str8 %s", str8);				

   	kal_prompt_trace(MOD_SOC,"Bird_soc_rev_command_deal COMMAND:%s",str2);
	if(strcmp(app_strtolower(str2), "s1") == 0)      /* login */
	 {
	 	count_judge_gps_send_timer = 0; // 发送标志清零	
	 	kal_prompt_trace(MOD_SOC,"Bird_soc_rev_command_deal deal login");
		//Bird_soc_set_loginflag();
	 	Yd_login_res(str1,str3);
	 }
	else if(strcmp(app_strtolower(str2), "s0") == 0)      /* heart */
	{
		count_judge_gps_send_timer = 0; //发送标志清零
		Yd_heart_res(str1);
		
	}
	else if(strcmp(app_strtolower(str2), "s3") == 0)      /* pos */
	{
		g_pos_return = 1;
		count_judge_gps_send_timer = 0; // 发送标志清零	
		g_sleeppos_return = 1;
		Rj_stop_timer(BIRD_TASK_POS_WAIT_TIMER);
		StopTimer(BIRD_READ_FILE);
		StartTimer(BIRD_READ_FILE, 1*1000, Yd_readposfile);
		Yd_sleeppos_rs();
	}
	else if(strcmp(app_strtolower(str2), "s4") == 0)      /* dis alarm */
	{
		g_alarmpoweroff_return = 1;
		g_alarmpoweroff_times = 0;
		count_judge_gps_send_timer = 0; // 发送标志清零
		Rj_stop_timer(BIRD_TASK_ALARM_POWEROFF_WAIT_TIMER);
		
	}
	else if(strcmp(app_strtolower(str2), "s7") == 0)      /* low power alarm */
	{
		g_alarmlow_return = 1;
		g_alarmlow_times = 0;
		count_judge_gps_send_timer = 0; // 发送标志清零
		Rj_stop_timer(BIRD_TASK_ALARM_LOW_WAIT_TIMER);
		Yd_alarmlow_rs();
		
	}
	else if(strcmp(app_strtolower(str2), "s8") == 0)      /* move alarm */
	{
		g_alarmmove_return = 1;
		g_alarmmove_times = 0;
		count_judge_gps_send_timer = 0; // 发送标志清零
		Rj_stop_timer(BIRD_TASK_ALARM_MOVE_WAIT_TIMER);
		
	}
	else if(strcmp(app_strtolower(str2), "s21") == 0)      /* run alarm */
	{
		g_alarmrun_return = 1;
		g_alarmrun_times = 0;
		count_judge_gps_send_timer = 0; // 发送标志清零
		Rj_stop_timer(BIRD_TASK_ALARM_RUN_WAIT_TIMER);
		
	}
	else if(strcmp(app_strtolower(str2), "s17") == 0)      /* speed alarm */
	{
		count_judge_gps_send_timer = 0; // 发送标志清零	
		
	}
	else if(strcmp(app_strtolower(str2), "s18") == 0)      /* equ state res */
	{
		count_judge_gps_send_timer = 0; // 发送标志清零	
		
	}
	else if(strcmp(app_strtolower(str2), "s14") == 0)      /* search params */
	{
		bird_search_param_res(str3,str4);  
		
	}
	else if(strcmp(app_strtolower(str2), "s2") == 0)      /* set params */
	{
		bird_set_param_res(str3,str4); 	
	}
	else if(strcmp(app_strtolower(str2), "s12") == 0)      /* set sf */
	{
		#if defined(BIRD_ECU_MMI_SUPPORT)
			bird_ecu_defend_res(1); //sf
		#else
			bird_set_sf_res(); //sf
		#endif
		count_judge_gps_send_timer = 0; 
	}
	else if(strcmp(app_strtolower(str2), "s13") == 0)      /* set cf */
	{
		#if defined(BIRD_ECU_MMI_SUPPORT)
		bird_ecu_defend_res(0); //cf
		#else
		bird_set_cf_res(); //cf
		#endif
		count_judge_gps_send_timer = 0; 
	}
	#if (!defined BIRD_ECU_MMI_SUPPORT)
	else if(strcmp(app_strtolower(str2), "s34") == 0)      /* set cf */
	{
		bird_set_defense_res(2); //永久性设防
		count_judge_gps_send_timer = 0; 
	}
	#endif
	else if(strcmp(app_strtolower(str2), "s11") == 0)      /* cq */
	{
    		 bird_soc_cq_res();
    		 yd_send_reboot_control_msg();/*重启*/
	}
	else if(strcmp(app_strtolower(str2), "s10") == 0)      /* dw */
	{
		bird_soc_dealdw(str3,str4);/*定位*/
	}
	else if(strcmp(app_strtolower(str2), "s30") == 0)      /* zc */
	{
		Yd_zc_res(str3);	
		count_judge_gps_send_timer = 0; // 发送标志清零
	}
	else if(strcmp(app_strtolower(str2), "s31") == 0)      /* getdw */
	{
		Yd_getdw_res(str3,str4);	
		count_judge_gps_send_timer = 0; // 发送标志清零
	}	
	else if(strcmp(app_strtolower(str2), "s99") == 0)      /* jy */
	{
		bird_soc_sendjy();	
	}
	else if(strcmp(app_strtolower(str2), "s15") == 0)      /* 锁车/解锁 */
	{
		#ifdef BIRD_ECU_MMI_SUPPORT
		Yd_ecu_lock_res(str3);
		#endif	
		count_judge_gps_send_timer = 0; // 发送标志清零
	}
	else if(strcmp(app_strtolower(str2), "s37") == 0)      
	{
		//平台对终端透传数据上传的应答
		count_judge_gps_send_timer = 0; // 发送标志清零
	}
	else if(strcmp(app_strtolower(str2), "s50") == 0)      
	{
		//静态侧翻
		count_judge_gps_send_timer = 0; // 发送标志清零
	}
	else if(strcmp(app_strtolower(str2), "s51") == 0)      
	{
		//动态侧翻
		count_judge_gps_send_timer = 0; // 发送标志清零
	}
#ifdef BIRD_ECU_MMI_SUPPORT
	else if(strcmp(app_strtolower(str2), "s53") == 0)
	{
		//平台下发透传数据
		count_judge_gps_send_timer = 0; // 发送标志清零
		yd_server_send_data_to_uart(str3,str4,str5);
	}
#endif
}
void Bird_soc_rev_deal()
{
	S32 zy_i,zy_j,zy_y,zy_index;
	U16 actlen=0;
	U8 ngetbg =0;
	U8 ngetend=0;
	U16 zy_id=0;
	S32 ngetbgid=0;
   	S32 ngetendid=0;
   	U16 rev_index=0;
	U16 rev_index1=0;
	S32 nrevindex=0;
	S32 nrevindexEnd=0;

	ngetbg = 0;
	ngetend = 0;
	ngetbgid = 0;
	ngetendid = 0;
	kal_prompt_trace(MOD_SOC,"Bird_soc_rev_deal rev index %d  %d",g_socket_rev.g_n_rev_index,g_socket_rev.g_n_rev_indexEnd);

	//for(zy_i=g_socket_rev.g_n_rev_index;zy_i<g_socket_rev.g_n_rev_indexEnd;zy_i++)
	//	{
	//		rev_index = (zy_i % RECEIVE_DATA_LENGTH);	
	//		kal_prompt_trace(MOD_SOC," %x ",g_socket_rev.rev_buf[rev_index]);
	//	}

   	for(zy_index=g_socket_rev.g_n_rev_index;zy_index<g_socket_rev.g_n_rev_indexEnd;zy_index++)
   	{
   		rev_index = (zy_index % RECEIVE_DATA_LENGTH);	
		if(g_socket_rev.rev_buf[rev_index] == 0x5B)
		{
			ngetbg = 1;
			ngetbgid=zy_index;
		}
		else if(g_socket_rev.rev_buf[rev_index] == 0x5D){
			if(ngetbg==1)
			{
				ngetend = 1;
				ngetendid=zy_index;
			}
		}
		
		//找到头尾
		if(ngetbg==1 && ngetend==1)
		{
			ngetbg = 0;
			ngetend = 0;	
			
			kal_prompt_trace(MOD_SOC,"rev index get hd %d  %d",g_socket_rev.g_n_rev_index,g_socket_rev.g_n_rev_indexEnd);
   			memset(receive_data_array, 0x00, RECEIVE_DATA_LENGTH);
	   		memset(receive_data_arraysingle, 0x00, RECEIVE_DATA_LENGTH);		
			zy_id=0;
			for(zy_y=ngetbgid+1;zy_y<ngetendid;zy_y++)
			{
				rev_index1 = (zy_y % RECEIVE_DATA_LENGTH);	
				receive_data_arraysingle[zy_id]= g_socket_rev.rev_buf[rev_index1];
				zy_id++;
			}
			actlen = zy_id;			
			//对命令进行处理
			Bird_soc_rev_command_deal(receive_data_arraysingle);				
			
		}
		
   	}
	if(ngetbg==0 && ngetend==0)
	{
		g_socket_rev.g_n_rev_index=g_socket_rev.g_n_rev_indexEnd;
	}
	else if(ngetbg==1 && ngetend==1)
	{
		g_socket_rev.g_n_rev_index=g_socket_rev.g_n_rev_indexEnd;
	}
	kal_prompt_trace(MOD_SOC,"rev end index %d  %d",g_socket_rev.g_n_rev_index,g_socket_rev.g_n_rev_indexEnd);
}

void Bird_soc_tbox_rev_command_deal(U8 *p_receive_data, U32 receive_length)
{
	U32 length=0;
	U8 command[2];
	U8 time_rec[6]={0};
	U8 time[6]={0};
	U8 vin[17];
	U8 encrypt[1];
	U8 datalength[2];
	U16 data_len=0;
	U8 rest_buf[RECEIVE_DATA_LENGTH];
	   
	kal_prompt_trace(  MOD_SOC,"Bird_soc_tbox_rev_command_deal %d", receive_length);
	
       if(receive_length<19)
	   	return;
		
	memset(command,0,sizeof(command));
	memcpy(command, p_receive_data,2);
	length=length+2;
	
	memset(vin,0,sizeof(vin));
	memcpy(vin, p_receive_data+length,17);
	length=length+17;
	
	memset(encrypt,0,sizeof(encrypt));	
	memcpy(encrypt, p_receive_data+length,1);
	length=length+1;

	memset(datalength,0,sizeof(datalength));		
	memcpy(datalength, p_receive_data+length,2);
	length=length+2;

	data_len=datalength[0]*256+datalength[1];
	
	if(strcmp(vin, rj_user_info.u_id) != 0)
	{
	 	kal_prompt_trace(  MOD_SOC,"Bird_soc_tbox_rev_command_deal vin not correct");
	 	return;
	}

	kal_prompt_trace(  MOD_SOC,"Bird_soc_tbox_rev_command_deal command %d %d", command[0],command[1]);
	
	if((command[0]==0x07)&&(command[1]==0x01))     
	{
	 	kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal heart");
		Rj_stop_timer(BIRD_TASK_HEART_WAIT_TIMER);
	 	count_judge_gps_send_timer = 0; //发送标志清零
	 	return;
	}

	kal_prompt_trace(  MOD_SOC,"Bird_soc_tbox_rev_command_deal %d %d", receive_length-length,data_len);

       if(receive_length-length==data_len+1)
       {
	 	memset(rest_buf,0,sizeof(rest_buf));
	 	memcpy(rest_buf, p_receive_data+length,data_len);
       }
	else
	{
	 	return;
	}

	if(encrypt[0]==0x03)
		bird_set_inv_aes128_encode(rest_buf,data_len/16+1);
	
	if((command[0]==0x01)&&(command[1]==0x01))   
	{
	 	count_judge_gps_send_timer = 0; // 发送标志清零	
	 	kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal login");
		Bird_soc_set_loginflag();
	 	Yd_tboxlogin_res(rest_buf,data_len);
	}
	else if((command[0]==0x04)&&(command[1]==0x01))     
	{
		kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal logout");
	 	count_judge_gps_send_timer = 0; //发送标志清零
	}
	else if((command[0]==0x08)&&(command[1]==0x01))   
	{
		kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal timing");	 
	 	count_judge_gps_send_timer = 0; //发送标志清零	
	 	Yd_calib_res(rest_buf,data_len);
	}
	else if((command[0]==0x02)&&(command[1]==0x01))   
	{
		applib_time_struct current_time = {0};
	 	memcpy(time_rec, rest_buf,6);
		txboxdecimal(time ,time_rec);
	 	current_time.nYear=(U16)time[0]+2000;
	 	current_time.nMonth=time[1];
	 	current_time.nDay=time[2];
	 	current_time.nHour=time[3];
	 	current_time.nMin=time[4];
	 	current_time.nSec=time[5];

	 	kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal realinfo %d %d %d %d %d %d",current_time.nYear,current_time.nMonth,
			current_time.nDay,current_time.nHour,current_time.nMin,current_time.nSec);
		if(applib_dt_is_valid(&current_time))
		Bird_soc_setflag_posfile(current_time);
		if(g_third_alarm_flag==0)
		{
		//StopTimer(BIRD_READ_FILE);
		//StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);
		}
	 	count_judge_gps_send_timer = 0; //发送标志清零
	}
	else if((command[0]==0x03)&&(command[1]==0x01))      
	{
		applib_time_struct current_time = {0};
	 	memcpy(time_rec, rest_buf,6);
		txboxdecimal(time ,time_rec);
	 	current_time.nYear=(U16)time[0]+2000;
	 	current_time.nMonth=time[1];
	 	current_time.nDay=time[2];
	 	current_time.nHour=time[3];
	 	current_time.nMin=time[4];
	 	current_time.nSec=time[5];

	 	kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal resendinfo %d %d %d %d %d %d",current_time.nYear,current_time.nMonth,
			current_time.nDay,current_time.nHour,current_time.nMin,current_time.nSec);
		if(applib_dt_is_valid(&current_time))
		Bird_soc_setflag_posfile(current_time);
		if(g_third_alarm_flag==0)
		{
		//StopTimer(BIRD_READ_FILE);
		//StartTimer(BIRD_READ_FILE, 1000, Yd_readposfile);
		}
	 	count_judge_gps_send_timer = 0; //发送标志清零
	}
	else if((command[0]==0x80)&&(command[1]==0xfe))     
	{
		kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal search");	 
		bird_tbox_search_param_res(rest_buf,data_len);
	 	count_judge_gps_send_timer = 0; //发送标志清零		
	}
	else if((command[0]==0x81)&&(command[1]==0xfe))     
	{
		kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal set");	 
		bird_tbox_set_param_res(rest_buf,data_len);	
	 	count_judge_gps_send_timer = 0; //发送标志清零		
	}
	else if((command[0]==0x82)&&(command[1]==0xfe))    
	{
		kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_command_deal control");	 
		bird_tbox_control_param_res(rest_buf,data_len);
	 	count_judge_gps_send_timer = 0; //发送标志清零		
	}
}

void Bird_soc_tbox_rev_deal()
{
	U32 zy_i,zy_j,zy_y,zy_index;
	U16 actlen=0;
	U8 ngetbg =0;
	U8 ngetend=0;
	U16 zy_id=0;
	U32 ngetbgid=0;
   	U32 ngetendid=0;
   	U16 rev_index=0;
	U16 rev_index1=0;
	U32 nrevindex=0;
	U32 nrevindexEnd=0;
	
	kal_prompt_trace(MOD_SOC,"Bird_soc_tbox_rev_deal rev index %d  %d",g_socket_rev.g_n_rev_index,g_socket_rev.g_n_rev_indexEnd);
   	for(zy_index=g_socket_rev.g_n_rev_index;zy_index<g_socket_rev.g_n_rev_indexEnd;zy_index++)
   	{
   		rev_index = (zy_index % RECEIVE_DATA_LENGTH);	

		if((g_socket_rev.rev_buf[rev_index] == 0x23)&&(zy_index<(g_socket_rev.g_n_rev_indexEnd-2)))
		{
			if(g_socket_rev.rev_buf[(rev_index+1)% RECEIVE_DATA_LENGTH] == 0x23)
			{
			ngetbg = 1;
			ngetend = 1;
			ngetbgid=zy_index+2;
			}
		}
		
		//找到头尾
		if(ngetbg==1 && ngetend==1)
		{
			ngetbg = 0;
			ngetend = 0;	
			
			kal_prompt_trace(MOD_SOC,"rev index get hd %d  %d",g_socket_rev.g_n_rev_index,g_socket_rev.g_n_rev_indexEnd);
   			memset(receive_data_array, 0x00, RECEIVE_DATA_LENGTH);
	   		memset(receive_data_arraysingle, 0x00, RECEIVE_DATA_LENGTH);		
			zy_id=0;
			for(zy_y=ngetbgid;zy_y<g_socket_rev.g_n_rev_indexEnd;zy_y++)
			{
				rev_index1 = (zy_y % RECEIVE_DATA_LENGTH);	
				receive_data_arraysingle[zy_id]= g_socket_rev.rev_buf[rev_index1];
				zy_id++;
			}
			actlen = zy_id;			
			//对命令进行处理
			Bird_soc_tbox_rev_command_deal(receive_data_arraysingle,g_socket_rev.g_n_rev_indexEnd-ngetbgid);				
			
		}
		
   	}
	if(ngetbg==0 && ngetend==0)
	{
		g_socket_rev.g_n_rev_index=g_socket_rev.g_n_rev_indexEnd;
	}
	else if(ngetbg==1 && ngetend==1)
	{
		g_socket_rev.g_n_rev_index=g_socket_rev.g_n_rev_indexEnd;
	}
	kal_prompt_trace(MOD_SOC,"rev end index %d  %d",g_socket_rev.g_n_rev_index,g_socket_rev.g_n_rev_indexEnd);
}

static U8 Lima_Soc_Write_Recv_buf(lima_soc_transaction_struct *p_socket)
{
	S32  handle;
    	U32  len;
    	S8   *pBegin = NULL; //lrf //写入内容开始
    	U32  wtLen;//写入内容长度
    	S32  offset;//文件读写指针偏移量
    	BOOL bRet;
    	S32  file_ret;
    	S8 filename[64]={0};
    
    
    	bRet = FALSE;   // false mean the feadback 
    
    	if (0 == p_socket->rcvd_counter)
    	{
        	pBegin = (S8*)p_socket->rcvd_buffer + p_socket->rcvd_head_len;
        	wtLen  = (U32)(p_socket->rcvd_once_len - p_socket->rcvd_head_len);
        	offset = 0; 
    	}
    	else
    	{
       	pBegin = (S8*)p_socket->rcvd_buffer;
        	wtLen  = (U32)p_socket->rcvd_once_len;
        	offset = (S32)(p_socket->rcvd_counter - p_socket->rcvd_head_len);
    	}
    
#ifdef LIMA_SOC_CONTINUE_DL//断点续传
    	if (p_socket->start_point > 0 )
    	{
        	offset += p_socket->start_point;
    	}
#endif
 	{
   		U16 rev_index=0;
		U32 zy_i = 0;
		for(zy_i=0;zy_i<wtLen;zy_i++)
		{
		   	rev_index = (g_socket_rev.g_n_rev_indexEnd % RECEIVE_DATA_LENGTH);	
			g_socket_rev.rev_buf[rev_index] = *(pBegin+zy_i);
			g_socket_rev.g_n_rev_indexEnd++;
		 }
               kal_prompt_trace(MOD_SOC,"Lima_Soc_Write_Recv_buf rev index %d  %d",g_socket_rev.g_n_rev_index,g_socket_rev.g_n_rev_indexEnd);
               if(g_gettime_flag==0)
               Bird_soc_rev_deal();
               else
               Bird_soc_tbox_rev_deal();		  
	}
    return bRet;
}






/**************************************************************************************************
Function name    :    Lima_Soc_Hex_Atoi
Functions        :        将16进制的数字字符串转化为数字(不处理0x头)
Parameters        :    1               
    input        :    hex 
    output        :    NONE
    return        :    NONE
< Author >            < Date >              < Notes >
GuoJin                2009-06-12        create:
**************************************************************************************************/
static U32 Lima_Soc_Hex_Atoi(S8 *hex)
{
    S8  *pCh = NULL; //lrf
    U32 num;
    
    num = 0;
    pCh = hex;
    while((48 <= *pCh && *pCh <= 57) || //'0'~'9'
          (65 <= *pCh && *pCh <= 70) || //'A'~'F'
          (97 <= *pCh && *pCh <= 102))  //'a'~'f'
    {
        num *= 16;
        
        if (48 <= *pCh && *pCh <= 57)
        {
            num += (*pCh - '0');
        }
        else if (65 <= *pCh && *pCh <= 70)
        {
            num += (10 + (*pCh - 'A'));
        }
        else
        {
            num += (10 + (*pCh - 'a'));
        }

        pCh++;
    }

    return num;
    
}



/******************************************Global variable start******************/
static kal_int8 socket_id;
static kal_uint8 downapp_id=0;
static kal_uint32 account_id=CBM_DEFAULT_ACCT_ID;
static sockaddr_struct g_bd_ip;
kal_int8* recv_buf;
kal_int8 g_is_need_continue = 0;
kal_int32 g_already_rev = 0;
kal_uint32 g_body_len=0;
static S8 g_revbuf[MAX_BIRD_RCV_BUFFER_SIZE];
static U8 g_url[80];
static U8 g_host[30];
static U8 g_port[10];
static kal_uint16 g_uart_sendlen=0;	
static kal_int32 g_getapsnnlen=0;
#define AGPSREQID 488

/******************************************Global variable end******************/

/*******************************HTTP_GetContentLength start*****************/
kal_uint32 HTTP_GetContentLength(kal_int8 *revbuf)
{
    kal_int8 *p1 = NULL, *p2 = NULL;
    kal_uint32 HTTP_Body = 0;//内容体长度

    p1 = strstr(revbuf,"Content-Length");
    if(p1 == NULL)
        return -1;
    else
    {
        p2 = p1+strlen("Content-Length")+ 2; 
        HTTP_Body = atoi(p2);
        return HTTP_Body;
    }

}


/*******************************HTTP_GetContentLength end*****************/

/*******************************HTTP_GetRecvLength start*****************/
kal_uint32  HTTP_GetRecvLength(kal_int8 *revbuf)
{
    kal_int8 *p1 = NULL;
    kal_uint32 HTTP_Body = 0;//内容体长度
    kal_uint32 HTTP_Head = 0;//HTTP 协议头长度


    HTTP_Body = HTTP_GetContentLength(revbuf);
    if(HTTP_Body == -1)
        return -1;

    p1=strstr(revbuf,"\r\n\r\n");
    if(p1==NULL)
        return -1;
    else
    {
        HTTP_Head = p1- revbuf +4;// 4是\r\n\r\n的长度
        return HTTP_Body+HTTP_Head;
    }

}
/*******************************HTTP_GetRecvLength end*****************/

static U16 HTTP_Get_Rsp_Status(S8 *chlwr)
{
    U16 status    = 0;
    S8  *pCh      = NULL;
    pCh = chlwr + strlen("HTTP/1.1") + 1;//或"HTTP/1.0 "取得状态码 
    while(*pCh == ' ')
    {
        pCh += 1;
    }

    status = atoi(pCh);
    kal_prompt_trace(MOD_SOC,"HTTP_Get_Rsp_Status =%d",status);
    return status;
    
}

static MMI_BOOL HTTP_is_Content_type_file(S8 *chlwr)
{
    if(strstr(chlwr,"Content-Type: application/octet-stream")!=NULL)
		return TRUE;
    else 
		return FALSE;
    
}

/*
功能:获取下载url中的文件名,最后一个/后的字符
参数:
返回值:
0-----------成功
-1----------失败
注:内部接收缓冲10k
*/

kal_int8 HTTP_GetFileName(char *url,char *filename)
{
    //提取url中最后一个/后的内容
    kal_int8 len;
    kal_int8 i;

    len = strlen(url);
    for(i=len-1;i>0;i--)
    {
        if(url[i] == '/')
            break;
    }
    if(i == 0)//下载地址错误
    {
        printf("url not contain '/'\n");
        return -1;
    }
    else
    {
    
        strcpy(filename,url+i+1);
        return 0;
    }
}


/*
功能:获取下载url中的路径,第一个/后的字符
参数:
返回值:
0-----------成功
-1----------失败
注:url ex "http://host:port/path"
*/
kal_int8 HTTP_GetPath(char *url,char *path)
{
    char *p;

    p = strstr(url,"http://");
    if(p == NULL)
    {
        p = strchr(url,'/');
        if(p == NULL)
            return -1;
        else
        {
            strcpy(path,p);
            return 0;
        }
    }
    else
    {
        p = strchr(url+strlen("http://"),'/');
        if(p == NULL)
            return -1;
        else
        {
            strcpy(path,p);
            return 0;
        }
    }

}

/*
功能:获取下载url中的ip和port,ip支持域名,端口默认为80
参数:
返回值:
1-----------域名式
2-----------ip port式
-1----------失败
注:url ex "http://host:port/path"
*/

kal_int8 HTTP_Get_IP_PORT(char *url,char *ip,char *port)
{
    char *p = NULL;
    int offset = 0;
    char DOMAIN_NAME[128];

    p = strstr(url,"http://");
    if(p == NULL)
    {
        offset = 0;
    }
    else
    {
        offset = strlen("http://");
    }

    p = strchr(url+offset,'/');
    if(p == NULL)
    {
        printf("url:%s format error\n",url);
        return -1;
        
    }
    else
    {

        memset(DOMAIN_NAME,0x0,sizeof(DOMAIN_NAME));
        memcpy(DOMAIN_NAME,url+offset,(p-url-offset));
        p = strchr(DOMAIN_NAME,':');
        if(p == NULL)
        {
            strcpy(ip,DOMAIN_NAME);
            strcpy(port,"80");
            //printf("ip %p,port %p\n",ip,port);
            
            #ifdef DEBUG_HTTP
            printf("ip=%s,port=%s\n",ip,port);//debug info
            #endif
            return 1;

        }
        else
        {    
            *p = '\0';

            strcpy(ip,DOMAIN_NAME);
            strcpy(port,p+1);
            
            #ifdef DEBUG_HTTP
            printf("ip=%s,port=%s\n",ip,port);//debug info
            #endif
            return 2;

        }


        return 0;
    }
    
}

/*******************************create and connect my socket start*****************/
void BD_socket_create(U8 * url)
{                 
    cbm_app_info_struct info; 
    kal_uint8 val_async;                            //设置socket为异步的val值
    kal_bool val_nonblocking=KAL_TRUE;                //设置socket为非阻塞的val值
    kal_uint8* recvaddr=NULL;
    kal_uint8 addr_len ;
    kal_int8 ret_domain=SOC_ERROR;
    kal_int8 ret_connect=SOC_ERROR;
    static S32 sAgpsnReq = AGPSREQID;
    /*apn*/
    srv_dtcnt_store_prof_qry_struct acct_id_query; 
    srv_dtcnt_result_enum result;
    int i,j;
    srv_dtcnt_prof_str_info_qry_struct prof_str_info_qry;
    U16 name[SRV_DTCNT_PROF_MAX_ACC_NAME_LEN+1];
    char charname[SRV_DTCNT_PROF_MAX_ACC_NAME_LEN+1]; 

    kal_prompt_trace(MOD_SOC," BD_socket_create");		
    recvaddr = OslMalloc(5);
    memset(recvaddr,0,sizeof(recvaddr));
    memset(&g_bd_ip,0,sizeof(g_bd_ip));
    memset(g_host,0,sizeof(g_host));
    memset(g_port,0,sizeof(g_port));
    memset(g_url,0,sizeof(g_url));
    memcpy(g_url,url,strlen(url));
    HTTP_Get_IP_PORT(g_url,g_host,g_port);
    kal_prompt_trace(MOD_SOC," HTTP_Get_IP_PORT");		

    recv_buf = NULL;
    if (NULL == g_lima_gp_soc_down_pool)
    {     
          kal_prompt_trace(MOD_SOC,"g_lima_gp_soc_down_pool NULL host,port=%s,%s",g_host,g_port);
          media_get_ext_buffer(MOD_MMI, (void **)&g_lima_gp_soc_down_pool, MAX_BIRD_SOC_MEMORY_SIZE);
          ASSERT(g_lima_gp_soc_down_pool != NULL);
          g_lima_gp_soc_down_adm_id = kal_adm_create((void*)g_lima_gp_soc_down_pool, MAX_BIRD_SOC_MEMORY_SIZE, NULL, KAL_FALSE);
	   recv_buf       = kal_adm_alloc(g_lima_gp_soc_down_adm_id, MAX_BIRD_RCV_BUFFER_SIZE);
    }
    else
    	{
    	        kal_prompt_trace(MOD_SOC,"g_lima_gp_soc_down_pool host,port=%s,%s",g_host,g_port);
    		recv_buf       = kal_adm_alloc(g_lima_gp_soc_down_adm_id, MAX_BIRD_RCV_BUFFER_SIZE);
    	}
	
    bird_get_nw_plmn();   
       kal_prompt_trace(MOD_SOC,"BD_socket_create host,port=%s,%s",g_host,g_port);
       memset(&acct_id_query,0,sizeof(acct_id_query)); 
 	acct_id_query.qry_info.filters = SRV_DTCNT_STORE_QRY_TYPE_ACC_TYPE;    
       acct_id_query.qry_info.acc_type_info = SRV_DTCNT_PROF_TYPE_FACTORY_CONF;	   
	
       result = srv_dtcnt_store_qry_ids(&acct_id_query);	
	if (result == SRV_DTCNT_RESULT_SUCCESS)
	{
		if(acct_id_query.num_ids>0)
		{
		             for (i=0;i<(acct_id_query.num_ids);i++)
	                     {
	                           prof_str_info_qry.dest = (S8*)name;
                                  prof_str_info_qry.dest_len = sizeof(name);
                                  srv_dtcnt_get_account_name(acct_id_query.ids[i],&prof_str_info_qry,SRV_DTCNT_ACCOUNT_PRIMARY);
                                  mmi_ucs2_to_asc((char *)charname,(char *)prof_str_info_qry.dest);
				     // get apn			
				     prof_str_info_qry.dest = (S8*)name;
                                  prof_str_info_qry.dest_len = sizeof(name);
                                  srv_dtcnt_get_apn(acct_id_query.ids[i],&prof_str_info_qry,SRV_DTCNT_ACCOUNT_PRIMARY);
                                  mmi_ucs2_to_asc((char *)charname,(char *)prof_str_info_qry.dest);
	                        }
		  }
		 else 
		 {
		         acct_id_query.qry_info.acc_type_info = SRV_DTCNT_PROF_TYPE_USER_CONF;	   
	                result = srv_dtcnt_store_qry_ids(&acct_id_query);
	
	                if (result == SRV_DTCNT_RESULT_SUCCESS)
	               {
		               if(acct_id_query.num_ids>0)
		               {
		                      for (i=0;i<(acct_id_query.num_ids);i++)
	                             {    
	                                    prof_str_info_qry.dest = (S8*)name;
                                           prof_str_info_qry.dest_len = sizeof(name);
                                           srv_dtcnt_get_account_name(acct_id_query.ids[i],&prof_str_info_qry,SRV_DTCNT_ACCOUNT_PRIMARY);
                                           mmi_ucs2_to_asc((char *)charname,(char *)prof_str_info_qry.dest);
				              // get apn			
				              prof_str_info_qry.dest = (S8*)name;
                                           prof_str_info_qry.dest_len = sizeof(name);
                                           srv_dtcnt_get_apn(acct_id_query.ids[i],&prof_str_info_qry,SRV_DTCNT_ACCOUNT_PRIMARY);
                                           mmi_ucs2_to_asc((char *)charname,(char *)prof_str_info_qry.dest);	 
	                              }
		                }
	                      else 
		               {
                                    add_new_acc_prof_id(L"BD Internet");
		                     //reboot 		 
		       	      {
						kal_prompt_trace(MOD_SOC,"BD_socket_create reboot");
                                         RJ_SYS_ReStart();
		                   	}	
	                      }			 
	                 }
                        else 
	                { 	 	
	                 }  						  
		}
      }		 	 			 	
      else 
      {	 	 	
      }   	
        //add by lqy

	if(acct_id_query.num_ids>0) {
		  aact_id =  acct_id_query.ids[0];
	        if (0 == downapp_id)
	        {
	            //cbm_register_app_id(&downapp_id);
	            //cbm_hold_bearer(downapp_id); 

				memset(&info,0,sizeof(info));                    //初始化info
	        info.app_str_id = 999;            //自己定义的字串
	        info.app_icon_id = 0;            //自己定义的icon
	        info.app_type = DTCNT_APPTYPE_EMAIL; //通过smart原则，匹配apn为cmnet的data account
	        		cbm_register_app_id_with_app_info(&info,&downapp_id);        //注册app id
	        }	  
	        account_id= cbm_encode_data_account_id(aact_id, CBM_SIM_ID_SIM1, downapp_id, 0);
	}  

	
    if(downapp_id == 0)
    {
        return ;
    }

    g_is_need_continue = 0;
     
    mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND, (PsIntFuncPtr)BD_socket_get_host_by_name);
    mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND,(PsIntFuncPtr)BD_socket_get_host_by_name,MMI_TRUE);
    //注册域名解析的notify处理函数

     mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND, (PsIntFuncPtr)BD_socket_notify);
     mmi_frm_set_protocol_event_handler(MSG_ID_APP_SOC_NOTIFY_IND,(PsIntFuncPtr)BD_socket_notify,MMI_TRUE);
     //注册异步的socket option选项相关的notify处理函数	

    socket_id = soc_create(SOC_PF_INET,SOC_SOCK_STREAM,0,MOD_MMI, account_id);//创建socket id
    if(socket_id < 0)
    {
        kal_prompt_trace(MOD_SOC," BD_socket_create soc_create  fail");
        soc_close(socket_id);    
        return ;
    }
	kal_prompt_trace(MOD_SOC,"BD_socket_create appid,accountid,socket=%d,%d,%d",downapp_id,account_id,socket_id); 
    if(soc_setsockopt(socket_id,SOC_NBIO, &val_nonblocking,sizeof(val_nonblocking))<0)//设置socket选项为非阻塞
    {
        kal_prompt_trace(MOD_SOC," soc_setsockopt  SOC_NBIO fail");
        soc_close(socket_id);    
        return ;
    }
     kal_prompt_trace(MOD_SOC," soc_setsockopt SOC_ASYNC enter");	
    val_async = SOC_WRITE | SOC_READ| SOC_CONNECT | SOC_CLOSE;//设置socket选项为异步
    if(soc_setsockopt(socket_id,SOC_ASYNC, & val_async,sizeof(val_async))<0)
    {
        kal_prompt_trace(MOD_SOC," soc_setsockopt SOC_ASYNC fail");
        soc_close(socket_id);    
	 return ;
    }
    kal_prompt_trace(MOD_SOC," soc_gethostbyname enter");	
    ret_domain=soc_gethostbyname(KAL_FALSE,MOD_MMI,sAgpsnReq,g_host,recvaddr,&addr_len,0, account_id);                 //对目标域名进行解析，获得目标server的ip。
    kal_prompt_trace(MOD_SOC, "[soc_gethostbyname] ret_domain = %d", ret_domain);
    if(ret_domain == SOC_SUCCESS)
    {
       kal_prompt_trace(MOD_SOC," soc_gethostbyname SOC_SUCCESS");		
        memcpy(g_bd_ip.addr,recvaddr,addr_len); //将域名解析得到的ip保存到结构体中
        g_bd_ip.addr_len = addr_len;            //地址长度
        g_bd_ip.port = 80;                    //端口
        g_bd_ip.sock_type = SOC_SOCK_STREAM;        //socket type为tcp
        ret_connect = soc_connect(socket_id,&g_bd_ip);    //与服务器建立连接
        if(ret_connect == SOC_SUCCESS)
        {
            BD_socket_send();                            //连接成功送http request
        }
        else if(ret_connect != SOC_WOULDBLOCK)            //连接失败
        {
           kal_prompt_trace(MOD_SOC,"connect fail");	
            soc_close(socket_id);    
            return ;
        }
    }
    else if(ret_domain != SOC_WOULDBLOCK)                //域名解析失败
    {
        kal_prompt_trace(MOD_SOC," soc_gethostbyname fail");		
        soc_close(socket_id);   
        return ;
    }
}
/***********************************create my socket end*****************/
/***********************************send http request start*****************/
void BD_socket_send()
{
    kal_int8 ret_send=0;
    kal_int8 CRLF[] = {0x0d,0x0a,0x00};
    kal_int8 send_buf[300]={0};
    kal_int8 filepath[60]={0};

   kal_prompt_trace(MOD_SOC," BD_socket_send ");		
    memset(send_buf,0,sizeof(send_buf));
    memset(filepath,0,sizeof(filepath));
    HTTP_GetPath(g_url,filepath);
	
	sprintf(send_buf,"GET %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"Connection: Close\r\n"
			"\r\n",filepath,g_host); 

    kal_prompt_trace(MOD_SOC,"BD_socket_send buf= %s",send_buf);
    ret_send=soc_send(socket_id, send_buf,strlen(send_buf)+1,0);
	kal_prompt_trace(MOD_SOC,"BD_socket_send SEND= %d",ret_send);
    if(ret_send > 0)
    {
        BD_socket_receive();
    }
    else if(ret_send != SOC_WOULDBLOCK)
    {
        soc_close(socket_id);    
        return ;
    }
}
/********************************send http request end*********************/
/********************************receive http response start*****************/
extern peph_op_struct agps_peph_op;
void BD_socket_receive()
{
    kal_int32 ret_recv=0;    
    kal_int8 * p = NULL;
    kal_int32 tmplen,soundret;
    S8 uart_sendbuf[2048];	
    U8 i=0;

    kal_prompt_trace(MOD_SOC," BD_socket_receive ");		
    ret_recv = soc_recv(socket_id,(kal_uint8*)recv_buf,MAX_LIMA_RCV_BUFFER_SIZE,0);
    kal_prompt_trace(MOD_SOC,"BD_socket_receive ret_recv %d",ret_recv);	
    if(ret_recv > 0)
    {       
	 if(g_is_need_continue==0)
	 {
	 	
		if(HTTP_Get_Rsp_Status(recv_buf)!=200)
		{
			return;
		}
		/*if(HTTP_is_Content_type_file(recv_buf)==FALSE)
		{
		       kal_prompt_trace(MOD_SOC,"HTTP_is_Content_type_file FALSE");
			return;
		}*/
		g_is_need_continue=1;
	 	g_body_len = HTTP_GetContentLength(recv_buf);
		kal_prompt_trace(MOD_SOC,"BD_socket_receive g_body_len %d",g_body_len);
	 	memset(g_revbuf,0,sizeof(g_revbuf));
        	p = strstr(recv_buf,"\r\n\r\n");
        	if(p == NULL)//jia ru duan dian baocun
        	{
            	 	kal_prompt_trace(MOD_SOC,"BD_socket_receive success NOT FOUND");
        	}
        	else
        	{
        		g_getapsnnlen = ret_recv-(p-recv_buf)-4;
             		memcpy(g_revbuf,p+4,g_getapsnnlen);	
			g_already_rev = g_getapsnnlen;
		       kal_prompt_trace(MOD_SOC,"BD_socket_receive g_already_rev=%d,g_getapsnnlen=%d",g_already_rev,g_getapsnnlen);
        	}
		
	 }
	 else{
	 	if(g_body_len-g_already_rev>=ret_recv)
	 	{
			memcpy(g_revbuf+g_getapsnnlen,recv_buf,ret_recv);
			g_already_rev = g_already_rev +ret_recv;
			g_getapsnnlen = g_getapsnnlen + ret_recv;
			kal_prompt_trace(MOD_SOC,"BD_socket_receive g_already_rev=%d,g_getapsnnlen=%d",g_already_rev,g_getapsnnlen);
	 	}
		else if(g_body_len>g_already_rev)
		{
			memcpy(g_revbuf+g_getapsnnlen,recv_buf,g_body_len-g_already_rev);
			g_already_rev = g_body_len;
			kal_prompt_trace(MOD_SOC,"BD_socket_receive g_already_rev=%d,g_getapsnnlen=%d",g_already_rev,g_getapsnnlen);
			
		}
	 }
        if( (g_already_rev == g_body_len) && (g_already_rev>0) ) 
        {
        	SetProtocolEventHandler(bird_socket_write_agps_data,MSG_ID_YD_TK001_MSG_WRITE_AGPS); 
	         kal_prompt_trace(MOD_SOC,"BD_socket_receive success end isneed,body,once,alreay, %d,%d,%d,%d",g_is_need_continue,g_body_len,ret_recv,g_already_rev);     	 
                // write to uart apgs data
/*modified by Huangwenjian for HD8020 20160617 begin*/                
#if 0
		  memset(uart_sendbuf,0,sizeof(uart_sendbuf));
		  memcpy(uart_sendbuf,g_revbuf,2048);		
	         //g_uart_sendlen = BD_UART_Write(g_revbuf,g_already_rev);
		  g_uart_sendlen = BD_UART_Write(uart_sendbuf,2048); 	 
		  kal_prompt_trace(MOD_SOC,"BD_UART_Write success  g_uart_sendlen %d",g_uart_sendlen);	 
		  if (g_uart_sendlen <g_already_rev)	 
		  {
                     kal_sleep_task(400);       //must add  
		       memset(uart_sendbuf,0,sizeof(uart_sendbuf));
			memcpy(uart_sendbuf,g_revbuf+g_uart_sendlen,g_already_rev-g_uart_sendlen);		   
		      // memcpy(g_revbuf,g_revbuf+g_uart_sendlen,g_already_rev-g_uart_sendlen);
		       //tmplen = BD_UART_Write(g_revbuf,g_already_rev-g_uart_sendlen);
		       tmplen = BD_UART_Write(uart_sendbuf,g_already_rev-g_uart_sendlen);
			kal_prompt_trace(MOD_SOC,"BD_UART_Write success writelen =%d, return tmplen %d",g_already_rev-g_uart_sendlen,tmplen);   
		       g_uart_sendlen =  g_uart_sendlen + tmplen;
		       kal_prompt_trace(MOD_SOC,"BD_UART_Write success end g_uart_sendlen %d",g_uart_sendlen);	   
		  }	  	
#else
		memcpy(agps_peph_op.agps_buf, g_revbuf, g_already_rev);
		agps_peph_op.agps_len = g_already_rev;
		bird_deal_send_msg_handler	(MSG_ID_YD_TK001_MSG_WRITE_AGPS, 0, 0);	
#endif
/*modified by Huangwenjian for HD8020 20160617 end*/
		   //保存时间
		applib_dt_get_rtc_time(&agps_peph_op.get_peph_last_time); 
		kal_prompt_trace(MOD_SUPC,"%d.%d.%d:%d.%d", agps_peph_op.get_peph_last_time.nYear, agps_peph_op.get_peph_last_time.nMonth, agps_peph_op.get_peph_last_time.nDay,
			  	agps_peph_op.get_peph_last_time.nHour, agps_peph_op.get_peph_last_time.nMin);
		  // write second 
		  /*
		  kal_sleep_task(400);       //must add  
		  kal_prompt_trace(MOD_SOC,"BD_socket_receive success2 end isneed,body,once,alreay %d,%d,%d,%d",g_is_need_continue,g_body_len,ret_recv,g_already_rev);     	 
                // write to uart apgs data
		  memset(uart_sendbuf,0,sizeof(uart_sendbuf));
		  memcpy(uart_sendbuf,g_revbuf,2048);		
	         //g_uart_sendlen = BD_UART_Write(g_revbuf,g_already_rev);
		  g_uart_sendlen = BD_UART_Write(uart_sendbuf,2048); 	 
		  kal_prompt_trace(MOD_SOC,"BD_UART_Write success2  g_uart_sendlen %d",g_uart_sendlen);	 
		  if (g_uart_sendlen <g_already_rev)	 
		  {
                     kal_sleep_task(400);       //must add  
		       memset(uart_sendbuf,0,sizeof(uart_sendbuf));
			memcpy(uart_sendbuf,g_revbuf+g_uart_sendlen,g_already_rev-g_uart_sendlen);		   
		      // memcpy(g_revbuf,g_revbuf+g_uart_sendlen,g_already_rev-g_uart_sendlen);
		       //tmplen = BD_UART_Write(g_revbuf,g_already_rev-g_uart_sendlen);
		       tmplen = BD_UART_Write(uart_sendbuf,g_already_rev-g_uart_sendlen);
			kal_prompt_trace(MOD_SOC,"BD_UART_Write success2 writelen =%d, return tmplen %d",g_already_rev-g_uart_sendlen,tmplen);   
		       g_uart_sendlen =  g_uart_sendlen + tmplen;
		       kal_prompt_trace(MOD_SOC,"BD_UART_Write success2 end g_uart_sendlen %d",g_uart_sendlen);
		  }	*/  	 
        } 
    }
    else if(ret_recv != SOC_WOULDBLOCK)
    {
        soc_close(socket_id);    
        return ;
    }
}
/**********************************receive http response end*********************/

/****************app socket notify indication handler start*************************/
static MMI_BOOL BD_socket_notify (void* msg_ptr)
{
    app_soc_notify_ind_struct* bd_soc_notify = (app_soc_notify_ind_struct*)msg_ptr;
    kal_int8 ret_connect=SOC_ERROR;
    kal_prompt_trace(MOD_USB, "[BD_socket_notify]Entry");
    if(bd_soc_notify->socket_id != socket_id)
    {
        kal_prompt_trace(MOD_USB, "[BD_socket_notify] socket_id != socket_id fail");
        return MMI_FALSE; 
    }
    kal_prompt_trace(MOD_USB, "[BD_socket_notify]deal");
    switch(bd_soc_notify->event_type)
    {
        case SOC_READ:
            BD_socket_receive();
            break;
        case SOC_WRITE:
            BD_socket_send();
            break;
        case SOC_CONNECT:
	     kal_prompt_trace(MOD_USB, "[BD_socket_notify] SOC_CONNECT enter");		
            if(bd_soc_notify->result == KAL_FALSE)
            {               
                ret_connect = soc_connect(socket_id,&g_bd_ip);    //与服务器建立连接
                if(ret_connect == SOC_SUCCESS)
                {
                    kal_prompt_trace(MOD_USB, "[BD_socket_notify] soc_connect SOC_SUCCESS");
                    BD_socket_send();                        //连接成功送http request
                }
                else if(ret_connect != SOC_WOULDBLOCK)            //连接失败
                {
                   kal_prompt_trace(MOD_USB, "[BD_socket_notify] soc_connect != SOC_WOULDBLOCK");
                    soc_close(socket_id);        
                }

               //SetProtocolEventHandler(NULL, MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND);		
		 //mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND,(PsIntFuncPtr)BD_socket_get_host_by_name);
            }
            else if(bd_soc_notify->result == KAL_TRUE)
            {
                kal_prompt_trace(MOD_USB, "[BD_socket_notify] bd_soc_notify->result == KAL_TRUE");
                BD_socket_send();
            }
            break;
        case SOC_CLOSE:
            soc_close(socket_id);
	     kal_prompt_trace(MOD_USB, "[BD_socket_notify] SOC_CLOSE");		
            break;
    }
    return MMI_TRUE;
}
/******************app socket notify indication handler end*************************/
/***************app socket gethostbyname indication handler start*******************/
static MMI_BOOL BD_socket_get_host_by_name(void *msg_ptr)
{
    app_soc_get_host_by_name_ind_struct* dns_ind;
    kal_int8 ret_connect;
    kal_uint8 domain_app_id;    
    if(!(msg_ptr))
    {
        return MMI_FALSE;
    }
    dns_ind = (app_soc_get_host_by_name_ind_struct*)msg_ptr;
    domain_app_id = cbm_get_app_id(dns_ind->account_id);
    if(domain_app_id != downapp_id)
    {
        return MMI_FALSE;
    }
    kal_prompt_trace(MOD_SOC, "[BD_socket_get_host_by_name] entry");
    if(dns_ind->result == KAL_TRUE)
    {
        if(dns_ind->request_id==(kal_int32)AGPSREQID)
    	 {
    	 mmi_frm_clear_protocol_event_handler(MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND, (PsIntFuncPtr)BD_socket_get_host_by_name);
        memcpy(g_bd_ip.addr,dns_ind->addr,dns_ind->addr_len);
        g_bd_ip.addr_len = dns_ind->addr_len;
        g_bd_ip.port = 80;
        g_bd_ip.sock_type = SOC_SOCK_STREAM;
	 kal_prompt_trace(MOD_SOC, "[BD_socket_get_host_by_name]Http send request to %d.%d.%d.%d and port: %d",
                    g_bd_ip.addr[0],
                    g_bd_ip.addr[1],
                    g_bd_ip.addr[2],
                    g_bd_ip.addr[3],
                    g_bd_ip.port);	
        ret_connect = soc_connect(socket_id,& g_bd_ip);
	 kal_prompt_trace(MOD_SOC, "[BD_socket_get_host_by_name] soc_connect ret_connect= %d", ret_connect);	
        if(ret_connect == SOC_SUCCESS)
        {
            kal_prompt_trace(MOD_USB, "[BD_socket_get_host_by_name]soc_connect SOC_SUCCESS ");
            BD_socket_send();
        } 
	 else if(ret_connect != SOC_WOULDBLOCK)
        {
            kal_prompt_trace(MOD_USB, "[BD_socket_get_host_by_name]soc_connect !SOC_WOULDBLOCK ");
            soc_close(socket_id);
        }
        }
	 else{
	 	return KAL_FALSE;
	 }
    }
    else if(dns_ind->result == KAL_FALSE)
    {
        kal_prompt_trace(MOD_USB, "[BD_socket_get_host_by_name]soc_connect KAL_FALSE ");
        soc_close(socket_id);
    }
    return MMI_TRUE;
}
/***************app socket gethostbyname indication handler end*******************/


/***************************************************************************
  Function:      Bird_Log 2
  Description:    write log to file
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
static void Bird_Log2(U8 n_log_type)
{
if(0)
{
    Bird_log_type p_soc;
    applib_time_struct dt;
    S8                    temp[8];
    unsigned short                 file[256];
    S32                 hFile;
    kal_prompt_trace(MOD_SOC,"Bird_Log2 entry");   
    p_soc.Log_data_len = 0;
    memset(p_soc.Log_buffer, 0, 512);
    applib_dt_get_date_time(&dt);
    sprintf((char*)p_soc.Log_buffer, "[%04d-%02d-%02d %02d:%02d:%02d]", dt.nYear, dt.nMonth,dt.nDay,dt.nHour, dt.nMin, dt.nSec);
    p_soc.Log_data_len = strlen((const char *)p_soc.Log_buffer);
    kal_prompt_trace(MOD_SOC,"Bird_Log data %s",p_soc.Log_buffer);   
    if(n_log_type==1)
    {
    	/*CONN*/
    	strcat((char*)p_soc.Log_buffer, " CONN:");
    	p_soc.Log_data_len += strlen(" CONN:");
            
    	memset(temp, 0, sizeof(temp));
    	Lima_Soc_Itoa(temp, Lima_get_soc_conn_flag());
    	strcat((char*)p_soc.Log_buffer, (char*)temp);
    	p_soc.Log_data_len += strlen(temp);


	/*g_n_reconn*/
    	strcat((char*)p_soc.Log_buffer, " RECONN:");
    	p_soc.Log_data_len += strlen(" RECONN:");

    	memset(temp, 0, sizeof(temp));
    	Lima_Soc_Itoa(temp, g_n_reconn);
    	strcat((char*)p_soc.Log_buffer, (char*)temp);
    	p_soc.Log_data_len += strlen(temp);
	
 	/*g_lima_is_getip*/
    	strcat((char*)p_soc.Log_buffer, " GETIP:");
    	p_soc.Log_data_len += strlen(" GETIP:");

    	memset(temp, 0, sizeof(temp));
    	Lima_Soc_Itoa(temp, g_lima_is_getip);
    	strcat((char*)p_soc.Log_buffer, (char*)temp);
    	p_soc.Log_data_len += strlen(temp);
    	}
	else if(n_log_type==2)
	{
		 	/*g_socket_send.g_n_send_state*/
    		strcat((char*)p_soc.Log_buffer, " SENDSTATE:");
    		p_soc.Log_data_len += strlen(" SENDSTATE:");

    		memset(temp, 0, sizeof(temp));
    		Lima_Soc_Itoa(temp, g_socket_send.g_n_send_state);
    		strcat((char*)p_soc.Log_buffer, (char*)temp);
    		p_soc.Log_data_len += strlen(temp);

		 	/*g_socket_send.g_n_rev_state*/
    		strcat((char*)p_soc.Log_buffer, " REVSTATE:");
    		p_soc.Log_data_len += strlen(" REVSTATE:");

    		memset(temp, 0, sizeof(temp));
    		Lima_Soc_Itoa(temp, g_socket_send.g_n_rev_state);
    		strcat((char*)p_soc.Log_buffer, (char*)temp);
    		p_soc.Log_data_len += strlen(temp);	

		 	/*g_n_rev_reconn*/
    		strcat((char*)p_soc.Log_buffer, " REVCONN:");
    		p_soc.Log_data_len += strlen(" REVCONN:");

    		memset(temp, 0, sizeof(temp));
    		Lima_Soc_Itoa(temp, g_n_rev_reconn);
    		strcat((char*)p_soc.Log_buffer, (char*)temp);
    		p_soc.Log_data_len += strlen(temp);	

		 	/*g_socket_send.g_n_send_index*/
    		strcat((char*)p_soc.Log_buffer, " NINDEX:");
    		p_soc.Log_data_len += strlen(" NINDEX:");

    		memset(temp, 0, sizeof(temp));
    		Lima_Soc_Itoa(temp, g_socket_send.g_n_send_index);
    		strcat((char*)p_soc.Log_buffer, (char*)temp);
    		p_soc.Log_data_len += strlen(temp);	

		 	/*g_socket_send.g_n_send_indexAdd*/
    		strcat((char*)p_soc.Log_buffer, " MAXINDEX:");
    		p_soc.Log_data_len += strlen(" MAXINDEX:");

    		memset(temp, 0, sizeof(temp));
    		Lima_Soc_Itoa(temp, g_socket_send.g_n_send_indexAdd);
    		strcat((char*)p_soc.Log_buffer, (char*)temp);
    		p_soc.Log_data_len += strlen(temp);				
	}

		
	
    strcat((char*)p_soc.Log_buffer, " \r\n");
    p_soc.Log_data_len += strlen(" \r\n");

    kal_prompt_trace(MOD_SOC,"Bird_Log data %s",p_soc.Log_buffer);   
    kal_wsprintf((kal_wchar*) file,BIRD_LOG_FILE_PATH,(S16)MMI_CARD_DRV);
    //对比url和文件名称
    hFile = FS_Open((U16 *)file, FS_CREATE | FS_READ_WRITE);
    if (hFile >= FS_NO_ERROR)
    {    
       
        UINT len;
        //UINT fsize;
        //FS_GetFileSize(hFile,fsize);
        FS_Seek( hFile, 0, FS_FILE_END);
        FS_Write(hFile, p_soc.Log_buffer, p_soc.Log_data_len, &len);
        FS_Close(hFile);
    }
}
}
