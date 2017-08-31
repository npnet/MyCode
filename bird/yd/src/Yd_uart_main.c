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

#ifdef BIRD_ECU_MMI_SUPPORT
#include "Bird_app.h"
#include "Yd_main.h"
#include "Yd_uart_main.h"

extern kal_uint8 ECU_SUMbuf[300];
extern kal_uint8 ECU2GPS_C0_CMD3[30]; //buffer length:15
extern kal_uint8 ECU2GPS_C0_CMD6[60]; //buffer length:9
extern kal_uint8 ECU2GPS_C0_CMD10[60]; //buffer length:24
extern kal_uint8 ECU2GPS_C0_CMD11[60]; //buffer length:24
extern kal_uint8 ECU2GPS_C0_CMD5[60]; //buffer length:24

extern kal_uint8 ECU_C0_CMD2_length();
extern void GPS2ECU_inquire_about_battery_message();
extern kal_uint16 ECU_UART_Write( kal_uint8 *buffer, kal_uint16 length);
extern void GPS2ECU_UART_wirte_C9_CMD1(BIRD_UART_C9_INFO *c9_info);
extern void GPS2ECU_UART_wirte_C9_CMD4(BIRD_UART_C9_04_INFO *c4_info);
extern void bird_soc_set_cf_res(U8 rtn);
extern void bird_soc_set_sf_res(U8 rtn);
extern kal_bool is_ac_charger_in(void);
extern void bird_soc_T15_res(U8 rtn);
extern void bird_soc_set_param_res(U8 *paramcmd,U8 rtn,U32 sevice_order_id);
extern void bird_soc_T37_send();
extern kal_uint8 ECU_C0_CMD2_flag();
extern BIRD_UART_A0_INFO A0_info;

 #ifndef BIRD_EXTERNEL_POWER_UNSUPPORT      
extern void rj_volt_poweroff(void);
 #endif
extern kal_uint16 ECU_BATTERY_percent();
extern kal_uint32 ECU_speed();
extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;
extern void bird_soc_sendlc(U8 type);
extern kal_uint8 ECU_defend_status();

U8 get_uart_connect_status();
void yd_uart_battery_data();

//服务器下发寻车时，根据波导uart 1.1处理函数
U8 g_findcar_count=0;
U8 g_findcar_flag=0xff;
U32 g_findcar_id=0;
void Yd_findcar_to_uart()
{
	BIRD_UART_C9_INFO c9_info;

	kal_prompt_trace(MOD_SOC," Yd_findcar_to_uart ");
	   
	memset(&c9_info,0,sizeof(BIRD_UART_C9_INFO));  
	c9_info.findcar_flag=1;
	c9_info.findcar=g_findcar_flag;
	GPS2ECU_UART_wirte_C9_CMD1(&c9_info);
}
void Yd_findcar_to_server(U8 flag)
{
    if(flag==TO_SERVER_NORES)
         bird_soc_set_param_res("FINDCAR=1",0,g_findcar_id);
    else if(flag==TO_SERVER_NOK)
         bird_soc_set_param_res("FINDCAR=1",0,g_findcar_id);
    else if(flag==TO_SERVER_OK)
         bird_soc_set_param_res("FINDCAR=1",1,g_findcar_id);
}
void Yd_ecu_findcar_deal()
{

	kal_prompt_trace(MOD_SOC," Yd_ecu_findcar_deal %d ",g_findcar_count);

	if(g_findcar_count<NORES_RESENDTIMES)
	{
	    Yd_findcar_to_uart();
           g_findcar_count++;
	    StartTimer(BIRD_ECU_FINDCAR_RESP, NORES_TIME*1000, Yd_ecu_findcar_deal);
	}
	else
	{
	    kal_prompt_trace(MOD_SOC," Yd_ecu_findcar_deal no response ");
	    Yd_findcar_to_server(TO_SERVER_NORES);
	    g_findcar_count=0;
	    g_findcar_flag=0xff;
	    StopTimer(BIRD_ECU_FINDCAR_RESP);
	}
}
void Yd_ecu_findcar_res(U32 id)
{
	kal_prompt_trace(MOD_SOC,"Yd_ecu_findcar_res %d",id);

	g_findcar_count=0;
	g_findcar_flag=1;
	g_findcar_id=id;
	StopTimer(BIRD_ECU_FINDCAR_RESP);
	Yd_ecu_findcar_deal();
}
//服务器下发限速时，根据波导uart 1.1处理函数
U8 g_limit_speed_value=0;
U8 g_limit_speed_flag=0xff;
U8 g_limit_speed_count=0;
U32 g_limit_speed_id=0;
void Yd_limit_speed_to_uart()
{
	BIRD_UART_C9_INFO c9_info;
	   
	memset(&c9_info,0,sizeof(BIRD_UART_C9_INFO));   
	c9_info.limitspeed_flag=1;
	c9_info.limitspeed=(g_limit_speed_value==0)?0:1;
	c9_info.limitspeed_value=g_limit_speed_value*UART_RATIO;
	GPS2ECU_UART_wirte_C9_CMD1(&c9_info);
}

void Yd_limit_speed_to_server(U8 flag)
{
     S8 paramsvalue[50];
     S8 fjchar[16];
	   
     memset(paramsvalue,0,sizeof(paramsvalue));
     memcpy(paramsvalue, "LIMITSPEED", strlen("LIMITSPEED"));
     memset(fjchar,0,sizeof(fjchar));
     sprintf(fjchar,"%d",g_limit_speed_value);
     bird_to_upper(paramsvalue);
     memcpy(paramsvalue+strlen("LIMITSPEED"), "=", strlen("="));
     memcpy(paramsvalue+strlen("LIMITSPEED")+strlen("="), (S8 *)fjchar, strlen((S8 *)fjchar));

    if(flag==TO_SERVER_NORES)
         bird_soc_set_param_res(paramsvalue,0,g_limit_speed_id);
    else if(flag==TO_SERVER_NOK)
         bird_soc_set_param_res(paramsvalue,0,g_limit_speed_id);
    else if(flag==TO_SERVER_OK)
         bird_soc_set_param_res(paramsvalue,1,g_limit_speed_id);
}
void Yd_ecu_limit_speed_deal()
{

	kal_prompt_trace(MOD_SOC," Yd_ecu_limit_speed_deal %d ",g_limit_speed_count);

	if(g_limit_speed_count<NORES_RESENDTIMES)
	{
	    Yd_limit_speed_to_uart();
           g_limit_speed_count++;
	    StartTimer(BIRD_ECU_LIMITSPEED_RESP, NORES_TIME*1000, Yd_ecu_limit_speed_deal);
	}
	else
	{
	    kal_prompt_trace(MOD_SOC," Yd_ecu_limit_speed_deal no response ");
	    Yd_limit_speed_to_server(TO_SERVER_NORES);
	    g_limit_speed_count=0;
	    g_limit_speed_flag=0xff;
	    StopTimer(BIRD_ECU_LIMITSPEED_RESP);
	}
}
void Yd_ecu_limit_speed_res(U8 value,U32 id)
{
	kal_prompt_trace(MOD_SOC,"Yd_ecu_limit_speed_res %d %d",value,id);

	g_limit_speed_value=value;
	g_limit_speed_id=id;
	g_limit_speed_flag=1;
	g_limit_speed_count=0;
	StopTimer(BIRD_ECU_LIMITSPEED_RESP);
	Yd_ecu_limit_speed_deal();
}

//服务器下发锁车/解锁时，根据波导uart 1.1处理函数
U8 g_lock_count=0;
U8 g_lock_flag=0xff;
void yd_ecu_lock_to_uart()
{
	BIRD_UART_C9_INFO c9_info;

	kal_prompt_trace(MOD_SOC," yd_ecu_lock_to_uart %d ",g_lock_flag);
	
	memset(&c9_info,0,sizeof(BIRD_UART_C9_INFO));   
	c9_info.lockcar_flag=1;
	c9_info.lockcar=g_lock_flag;
	GPS2ECU_UART_wirte_C9_CMD1(&c9_info);
}
void yd_ecu_lock_to_server(U8 flag)
{
    if(flag==TO_SERVER_NORES)
         bird_soc_T15_res(TO_SERVER_NORES);
    else if(flag==TO_SERVER_NOK)
         bird_soc_T15_res(TO_SERVER_NOK);
    else if(flag==TO_SERVER_OK)
         bird_soc_T15_res(TO_SERVER_OK);
}
void Yd_ecu_lock_deal()
{
	kal_prompt_trace(MOD_SOC," Yd_ecu_lock_deal %d ",g_lock_count);

	if(g_lock_count<NORES_RESENDTIMES)
	{
           yd_ecu_lock_to_uart();
           g_lock_count++;
	    StartTimer(BIRD_ECU_LOCK_RESP, NORES_TIME*1000, Yd_ecu_lock_deal);
	}
	else
	{
	    kal_prompt_trace(MOD_SOC," Yd_ecu_lock_deal no response ");
	    yd_ecu_lock_to_server(TO_SERVER_NORES);
	    g_lock_count=0;
	    g_lock_flag=0xff;
	    StopTimer(BIRD_ECU_LOCK_RESP);
	}
}
void Yd_ecu_lock_res(U8 *flag)
{
	U8 nflag = 0;	
	nflag = atoi(flag);

	kal_prompt_trace(MOD_SOC," Yd_ecu_lock_res %d ",nflag);
	
	g_lock_flag=nflag;
	g_lock_count=0;
	StopTimer(BIRD_ECU_LOCK_RESP);
       Yd_ecu_lock_deal();
}

//服务器下发开鞍座时，根据波导uart 1.1处理函数
U8 g_openaz_count=0;
U8 g_openaz_flag=0xff;
U32 g_openaz_id=0;
void Yd_openaz_to_uart()
{
	BIRD_UART_C9_INFO c9_info;

	kal_prompt_trace(MOD_SOC," Yd_openaz_to_uart ");
	   
	memset(&c9_info,0,sizeof(BIRD_UART_C9_INFO));   
	c9_info.anzuo_flag=1;
	c9_info.anzuo=g_openaz_flag;
	GPS2ECU_UART_wirte_C9_CMD1(&c9_info);
}
void Yd_openaz_to_server(U8 flag)
{
    if(flag==TO_SERVER_NORES)
         bird_soc_set_param_res("OPENTRUNK=1",0,g_openaz_id);
    else if(flag==TO_SERVER_NOK)
         bird_soc_set_param_res("OPENTRUNK=1",0,g_openaz_id);
    else if(flag==TO_SERVER_OK)
         bird_soc_set_param_res("OPENTRUNK=1",1,g_openaz_id);
}
void Yd_ecu_openaz_deal()
{

	kal_prompt_trace(MOD_SOC," Yd_ecu_openaz_deal %d ",g_openaz_count);

	if(g_openaz_count<NORES_RESENDTIMES)
	{
	    Yd_openaz_to_uart();
           g_openaz_count++;
	    StartTimer(BIRD_ECU_OPENAZ_RESP, NORES_TIME*1000, Yd_ecu_openaz_deal);
	}
	else
	{
	    kal_prompt_trace(MOD_SOC," Yd_ecu_openaz_deal no response ");
	    Yd_openaz_to_server(TO_SERVER_NORES);
	    g_openaz_count=0;
	    g_openaz_flag=0xff;
	    StopTimer(BIRD_ECU_OPENAZ_RESP);
	}
}
void Yd_ecu_openaz_res(U32 id)
{
	kal_prompt_trace(MOD_SOC,"Yd_ecu_openaz_res %d",id);

	g_openaz_count=0;
	g_openaz_flag=1;
	g_openaz_id=id;
	StopTimer(BIRD_ECU_OPENAZ_RESP);
	Yd_ecu_openaz_deal();
}


//服务器下发自检时，根据波导uart 1.1处理函数
U8 g_carcheck_count=0;
U8 g_carcheck_flag=0xff;
U32 g_carcheck_id=0;
void Yd_carcheck_to_uart()
{
	BIRD_UART_C9_INFO c9_info;

	kal_prompt_trace(MOD_SOC," Yd_carcheck_to_uart ");
	   
	memset(&c9_info,0,sizeof(BIRD_UART_C9_INFO));   
	c9_info.selfcheck_flag=1;
	c9_info.selfcheck=g_carcheck_flag;
	GPS2ECU_UART_wirte_C9_CMD1(&c9_info);
}
void Yd_carcheck_to_server(U8 flag)
{
    if(flag==TO_SERVER_NORES)
         bird_soc_set_param_res("CARCHECK=1",0,g_carcheck_id);
    else if(flag==TO_SERVER_NOK)
         bird_soc_set_param_res("CARCHECK=1",0,g_carcheck_id);
    else if(flag==TO_SERVER_OK)
         bird_soc_set_param_res("CARCHECK=1",1,g_carcheck_id);
}
void Yd_ecu_carcheck_deal()
{

	kal_prompt_trace(MOD_SOC," Yd_ecu_carcheck_deal %d ",g_carcheck_count);

	if(g_carcheck_count<NORES_RESENDTIMES)
	{
	    Yd_carcheck_to_uart();
           g_carcheck_count++;
	    StartTimer(BIRD_ECU_CARCHECK_RESP, NORES_TIME*1000, Yd_ecu_carcheck_deal);
	}
	else
	{
	    kal_prompt_trace(MOD_SOC," Yd_ecu_carcheck_deal no response ");
	    Yd_carcheck_to_server(TO_SERVER_NORES);
	    g_carcheck_count=0;
	    g_carcheck_flag=0xff;
	    StopTimer(BIRD_ECU_CARCHECK_RESP);
	}
}
void Yd_ecu_carcheck_res(U32 id)
{
	kal_prompt_trace(MOD_SOC,"Yd_ecu_carcheck_res %d",id);

	g_carcheck_count=0;
	g_carcheck_flag=1;
	g_carcheck_id=id;
	StopTimer(BIRD_ECU_CARCHECK_RESP);
	Yd_ecu_carcheck_deal();
}

//服务器下发自启动时，根据波导uart 1.1处理函数
U8 g_carstart_count=0;
U8 g_carstart_flag=0xff;
U32 g_carstart_id=0;
U8 g_car_status=0;
void yd_uart_car_start()
{
     kal_prompt_trace(MOD_SOC,"yd_uart_car_start %d",g_car_status);

     if(g_car_status==0)
     {
         g_car_status=1;
         bird_soc_sendpos();
     }
}
void yd_uart_car_close()
{
     kal_prompt_trace(MOD_SOC,"yd_uart_car_close %d",g_car_status);

     if(g_car_status==1)
     {
         g_car_status=0;
         bird_soc_sendpos();
     }
}
U8 get_car_status()
{
     return g_car_status;
}
void Yd_carstart_to_uart()
{
	BIRD_UART_C9_INFO c9_info;

	kal_prompt_trace(MOD_SOC," Yd_carstart_to_uart ");
	   
	memset(&c9_info,0,sizeof(BIRD_UART_C9_INFO));   
	c9_info.startup_flag=1;
	c9_info.startup=g_carstart_flag;
	GPS2ECU_UART_wirte_C9_CMD1(&c9_info);
}
void Yd_carstart_to_server(U8 flag)
{
	if(g_carstart_flag==1)
	{
    	    if(flag==TO_SERVER_NORES)
               bird_soc_set_param_res("CARSTART=1",0,g_carstart_id);
    	    else if(flag==TO_SERVER_NOK)
    	        bird_soc_set_param_res("CARSTART=1",0,g_carstart_id);
    	    else if(flag==TO_SERVER_OK)
    	        bird_soc_set_param_res("CARSTART=1",1,g_carstart_id);
	}
	else
	{
    	    if(flag==TO_SERVER_NORES)
               bird_soc_set_param_res("CARSTART=0",0,g_carstart_id);
    	    else if(flag==TO_SERVER_NOK)
    	        bird_soc_set_param_res("CARSTART=0",0,g_carstart_id);
    	    else if(flag==TO_SERVER_OK)
    	        bird_soc_set_param_res("CARSTART=0",1,g_carstart_id);
	}
}
void Yd_ecu_carstart_deal()
{

	kal_prompt_trace(MOD_SOC," Yd_ecu_carstart_deal %d ",g_carstart_count);

	if(g_carstart_count<NORES_RESENDTIMES)
	{
	    Yd_carstart_to_uart();
           g_carstart_count++;
	    StartTimer(BIRD_ECU_CARSTART_RESP, NORES_TIME*1000, Yd_ecu_carstart_deal);
	}
	else
	{
	    kal_prompt_trace(MOD_SOC," Yd_ecu_carstart_deal no response ");
	    Yd_carstart_to_server(TO_SERVER_NORES);
	    g_carstart_count=0;
	    g_carstart_flag=0xff;
	    StopTimer(BIRD_ECU_CARSTART_RESP);
	}
}
void Yd_ecu_carstart_res(U32 id,U8 flag)
{
	kal_prompt_trace(MOD_SOC,"Yd_ecu_carstart_res %d %d",id,flag);

	g_carstart_count=0;
	g_carstart_flag=flag;
	g_carstart_id=id;
	StopTimer(BIRD_ECU_CARSTART_RESP);
	Yd_ecu_carstart_deal();
}

U8 g_getbat_count=0;
U8 g_getbat_flag=0xff;
U32 g_getbat_id=0;
void Yd_getbat_to_uart()
{
	BIRD_UART_C9_INFO c9_info;

	kal_prompt_trace(MOD_SOC," Yd_getbat_to_uart ");
	   
	GPS2ECU_inquire_about_battery_message();
}
void Yd_getbat_to_server(U8 flag)
{
    	    if(flag==TO_SERVER_NORES)
               bird_soc_set_param_res("GETBAT=1",0,g_getbat_id);
    	    else if(flag==TO_SERVER_NOK)
    	        bird_soc_set_param_res("GETBAT=1",0,g_getbat_id);
    	    else if(flag==TO_SERVER_OK)
    	        bird_soc_set_param_res("GETBAT=1",1,g_getbat_id);
}
void Yd_ecu_battery_deal()
{

	kal_prompt_trace(MOD_SOC," Yd_ecu_battery_deal %d ",g_getbat_count);

	if(g_getbat_count<NORES_RESENDTIMES)
	{
		Yd_getbat_to_uart();
		g_getbat_count++;
		StartTimer(BIRD_ECU_BATTERY_RESP, NORES_TIME*1000, Yd_ecu_battery_deal);
	}
	else
	{
		kal_prompt_trace(MOD_SOC," Yd_ecu_battery_deal no response ");
		Yd_getbat_to_server(TO_SERVER_NORES);
		g_getbat_count=0;
		g_getbat_flag=0xff;
		StopTimer(BIRD_ECU_BATTERY_RESP);
	}
}
void Yd_ecu_getbat_res(U32 id)
{
	kal_prompt_trace(MOD_SOC,"Yd_ecu_getbat_res %d",id);

	g_getbat_count=0;
	g_getbat_flag=1;
	g_getbat_id=id;
	StopTimer(BIRD_ECU_BATTERY_RESP);
	Yd_ecu_battery_deal();
}

void yd_uart_battery_response()
{
	kal_prompt_trace(MOD_SOC," yd_uart_battery_response ");

	Yd_getbat_to_server(TO_SERVER_OK);
	yd_uart_battery_data();
}


extern U8 g_defend_count;
extern U8 g_defend_flag;
extern void yd_ecu_defend_deal();

void yd_uart_a0_response()
{
     if(g_defend_flag!=0xff)
     {
         kal_prompt_trace(MOD_SOC," yd_ecu_res_defend %d %d %d",g_defend_flag,A0_info.defend,A0_info.defend_flag);
         if((g_defend_flag==A0_info.defend)&&(A0_info.defend_flag==1))
         {
              StopTimer(BIRD_ECU_DEFEND_RESP);
       	kal_prompt_trace(MOD_SOC," yd_ecu_res_defend success %d",g_defend_flag);
       	bird_set_soc_log_info(ECU_status_log,"ecu_def %x %x",g_defend_flag,yd_tk001_get_defense());
       
       	if(yd_tk001_get_defense()!=g_defend_flag){
       		//yd_tk001_set_defense((MMI_BOOL)g_defend_flag);
			if(g_defend_flag==0)
			    yd_tk001_set_server_defense(0);
       		yd_send_save_nv_msg(); 
       	}
       	yd_ecu_defend_to_server(TO_SERVER_OK);
       	g_defend_count=0;
       	g_defend_flag=0xff;
         }
         else if((g_defend_flag!=A0_info.defend)&&(A0_info.defend_flag==1))
         {
              StopTimer(BIRD_ECU_DEFEND_RESP);
              if(g_defend_count<NORES_RESENDTIMES)
              {    
       	        yd_ecu_defend_to_uart();
       	        g_defend_count++;
       	        StartTimer(BIRD_ECU_DEFEND_RESP, NORES_TIME*1000, yd_ecu_defend_deal);
              }
              else
              {
       		 kal_prompt_trace(MOD_SOC," yd_ecu_defend fail");
       		 yd_ecu_defend_to_server(TO_SERVER_NOK);
       		 g_defend_count=0;
       		 g_defend_flag=0xff;
              }
         }
     }
	 
     if(g_lock_flag!=0xff)
     {
         kal_prompt_trace(MOD_SOC," yd_ecu_res_lock %d %d %d",g_lock_flag,A0_info.lockcar,A0_info.lockcar_flag);
         if((g_lock_flag==A0_info.lockcar)&&(A0_info.lockcar_flag==1))
         {
              StopTimer(BIRD_ECU_LOCK_RESP);
              kal_prompt_trace(MOD_SOC," yd_ecu_res_lock success %d",g_lock_flag);
       
              if(bird_get_lock_state()!=g_lock_flag)
              {
       	        //bird_set_lock_state(g_lock_flag);
       	        //DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SAVENV, 0);       
       	        //msg_send_ext_queue(rj_ilm);
       	        //bird_soc_sendpos();
              }
              yd_ecu_lock_to_server(TO_SERVER_OK);
              g_lock_count=0;
              g_lock_flag=0xff;
         }
         else if((g_lock_flag!=A0_info.lockcar)&&(A0_info.lockcar_flag==1))
         {
              StopTimer(BIRD_ECU_LOCK_RESP);
              if(g_lock_count<NORES_RESENDTIMES)
              {
       	        yd_ecu_lock_to_uart();
       	        StartTimer(BIRD_ECU_LOCK_RESP, NORES_TIME*1000, Yd_ecu_lock_deal);
       	        g_lock_count++;
              }
              else
              {
       	        kal_prompt_trace(MOD_SOC,"yd_ecu_lock fail");
       	        yd_ecu_lock_to_server(TO_SERVER_NOK);
       	        g_lock_count=0;
       	        g_lock_flag=0xff;
              }
         }
     }
	 
     if(g_limit_speed_flag!=0xff)
     {
         U8 limit_speed_tmp=0;
         kal_prompt_trace(MOD_SOC," yd_ecu_res_limit_speed %d %d %d",g_limit_speed_value,A0_info.limitspeed,A0_info.limitspeed_flag);
	  limit_speed_tmp=(g_limit_speed_value>0)?1:0;
         if((limit_speed_tmp==A0_info.limitspeed)&&(A0_info.limitspeed_flag==1))
         {
              kal_prompt_trace(MOD_SOC," yd_ecu_res_limit_speed");
    	       bird_set_soc_log_info(ECU_status_log,"limon");
			   
      	       StopTimer(BIRD_ECU_LIMITSPEED_RESP);  
    	       bird_set_limit_speed_value(g_limit_speed_value);
    	       yd_send_save_nv_msg();
    	       Yd_limit_speed_to_server(TO_SERVER_OK);
    	       g_limit_speed_count=0;
    	       g_limit_speed_flag=0xff;
         }
         else if((limit_speed_tmp!=A0_info.limitspeed)&&(A0_info.limitspeed_flag==1))
         {
              StopTimer(BIRD_ECU_LIMITSPEED_RESP);
              if(g_limit_speed_count<NORES_RESENDTIMES)
              {    
       	        Yd_limit_speed_to_uart();
       	        StartTimer(BIRD_ECU_LIMITSPEED_RESP, NORES_TIME*1000, Yd_ecu_limit_speed_deal);
       	        g_limit_speed_count++;
              }
              else
              {
       		 kal_prompt_trace(MOD_SOC," yd_ecu_limit_speed fail");
       		 Yd_limit_speed_to_server(TO_SERVER_NOK);
       		 g_limit_speed_count=0;
       		 g_limit_speed_flag=0xff;
              }
         }
     }

     if(g_findcar_flag!=0xff)
     {
         kal_prompt_trace(MOD_SOC," yd_ecu_res_findcar %d %d %d",g_findcar_flag,A0_info.findcar,A0_info.findcar_flag);

         if((g_findcar_flag==A0_info.findcar)&&(A0_info.findcar_flag==1))
         {
    	       kal_prompt_trace(MOD_SOC," yd_ecu_res_findcar");
    	       bird_set_soc_log_info(ECU_status_log,"findcar");
			   
    	       Yd_findcar_to_server(TO_SERVER_OK);
    	       g_findcar_count=0;
    	       g_findcar_flag=0xff;
    	       StopTimer(BIRD_ECU_FINDCAR_RESP);
         }
         else if((g_findcar_flag!=A0_info.findcar)&&(A0_info.findcar_flag==1))
         {
              StopTimer(BIRD_ECU_FINDCAR_RESP);
              if(g_findcar_count<NORES_RESENDTIMES)
              {    
       	        Yd_findcar_to_uart();
       	        StartTimer(BIRD_ECU_FINDCAR_RESP, NORES_TIME*1000, Yd_ecu_findcar_deal);
       	        g_findcar_count++;
              }
              else
              {
       		 kal_prompt_trace(MOD_SOC," yd_ecu_findcar fail");
       		 Yd_findcar_to_server(TO_SERVER_NOK);
       		 g_findcar_count=0;
       		 g_findcar_flag=0xff;
              }
         }

     }

     if(g_openaz_flag!=0xff)
     {
         kal_prompt_trace(MOD_SOC," yd_ecu_res_openaz %d %d %d",g_openaz_flag,A0_info.anzuo,A0_info.anzuo_flag);

         if((g_openaz_flag==A0_info.anzuo)&&(A0_info.anzuo_flag==1))
         {
         
    	       kal_prompt_trace(MOD_SOC," yd_ecu_res_openaz");
    	       bird_set_soc_log_info(ECU_status_log,"openaz");

    	       StopTimer(BIRD_ECU_OPENAZ_RESP);
              if(yd_tk001_get_az_flag()!=g_openaz_flag)
              {
       	        //yd_tk001_set_az_flag(g_openaz_flag);
       	        //DRV_BuildPrimitive(rj_ilm, MOD_AUX, MOD_MMI, MSG_ID_YD_TK001_MSG_SAVENV, 0);       
       	        //msg_send_ext_queue(rj_ilm);
       	        //bird_soc_sendpos();
              }			   
			   
    	       Yd_openaz_to_server(TO_SERVER_OK);
    	       g_openaz_count=0;
    	       g_openaz_flag=0xff;
         }
         else if((g_openaz_flag!=A0_info.anzuo)&&(A0_info.anzuo_flag==1))
         {
              StopTimer(BIRD_ECU_OPENAZ_RESP);
              if(g_openaz_count<NORES_RESENDTIMES)
              {    
       	        Yd_openaz_to_uart();
       	        StartTimer(BIRD_ECU_OPENAZ_RESP, NORES_TIME*1000, Yd_ecu_openaz_deal);
       	        g_openaz_count++;
              }
              else
              {
       		 kal_prompt_trace(MOD_SOC," yd_ecu_openaz fail");
       		 Yd_openaz_to_server(TO_SERVER_NOK);
       		 g_openaz_count=0;
       		 g_openaz_flag=0xff;
              }
         }

     }

     if(g_carcheck_flag!=0xff)
     {
         kal_prompt_trace(MOD_SOC," yd_ecu_res_carcheck %d %d %d",g_carcheck_flag,A0_info.selfcheck,A0_info.selfcheck_flag);

         if((g_carcheck_flag==A0_info.selfcheck)&&(A0_info.selfcheck_flag==1))
         {
    	       kal_prompt_trace(MOD_SOC," yd_ecu_res_carcheck");
    	       bird_set_soc_log_info(ECU_status_log,"carcheck");

    	       StopTimer(BIRD_ECU_CARCHECK_RESP);			   
    	       Yd_carcheck_to_server(TO_SERVER_OK);
    	       g_carcheck_count=0;
    	       g_carcheck_flag=0xff;
         }
         else if((g_carcheck_flag!=A0_info.selfcheck)&&(A0_info.selfcheck_flag==1))
         {
              StopTimer(BIRD_ECU_CARCHECK_RESP);
              if(g_carcheck_count<NORES_RESENDTIMES)
              {    
       	        Yd_carcheck_to_uart();
       	        StartTimer(BIRD_ECU_CARCHECK_RESP, NORES_TIME*1000, Yd_ecu_carcheck_deal);
       	        g_carcheck_count++;
              }
              else
              {
       		 kal_prompt_trace(MOD_SOC," yd_ecu_carcheck fail");
       		 Yd_carcheck_to_server(TO_SERVER_NOK);
       		 g_carcheck_count=0;
       		 g_carcheck_flag=0xff;
              }
         }

     }

     if(g_carstart_flag!=0xff)
     {
         kal_prompt_trace(MOD_SOC," yd_ecu_res_carstart %d %d %d",g_carstart_flag,A0_info.startup,A0_info.startup_flag);

         if((g_carstart_flag==A0_info.startup)&&(A0_info.startup_flag==1))
         {
    	       kal_prompt_trace(MOD_SOC," yd_ecu_res_carstart");
    	       bird_set_soc_log_info(ECU_status_log,"carstart");

    	       StopTimer(BIRD_ECU_CARSTART_RESP);			   
    	       Yd_carstart_to_server(TO_SERVER_OK);
    	       g_carstart_count=0;
    	       if(g_car_status!=g_carstart_flag)
    	       {
       	        //g_car_status=g_carstart_flag;
       	        //bird_soc_sendpos(); 
    	       }
    	       g_carstart_flag=0xff;
         }
         else if((g_carstart_flag!=A0_info.startup)&&(A0_info.startup_flag==1))
         {
              StopTimer(BIRD_ECU_CARSTART_RESP);
              if(g_carstart_count<NORES_RESENDTIMES)
              {    
       	        Yd_carstart_to_uart();
       	        StartTimer(BIRD_ECU_CARSTART_RESP, NORES_TIME*1000, Yd_ecu_carstart_deal);
       	        g_carstart_count++;
              }
              else
              {
       		 kal_prompt_trace(MOD_SOC," yd_ecu_carstart fail");
       		 Yd_carstart_to_server(TO_SERVER_NOK);
       		 g_carstart_count=0;
       		 g_carstart_flag=0xff;
              }
         }

     }

}

void yd_uart_lock()
{
	kal_prompt_trace(MOD_SOC," yd_uart_lock %d",bird_get_lock_state());

	if(bird_get_lock_state()==0)
	{
	        bird_set_lock_state(1);
	        bird_soc_sendpos();
	        yd_send_save_nv_msg();
	}
}
void yd_uart_unlock()
{
	kal_prompt_trace(MOD_SOC," yd_uart_unlock %d",bird_get_lock_state());
	
	    if(bird_get_lock_state()==1)
	    {
	        bird_set_lock_state(0);
	        bird_soc_sendpos();
	        yd_send_save_nv_msg();
	    }
}

//uart限速消息待确定
void yd_uart_limit_speed_on()
{
	kal_prompt_trace(MOD_SOC," yd_uart_limit_speed_on %d %d",bird_get_limit_speed_value());

	bird_set_limit_speed_value(bird_get_limit_speed_value());
	yd_send_save_nv_msg();
}

void yd_uart_limit_speed_off()
{
	kal_prompt_trace(MOD_SOC," yd_uart_limit_speed_off %d %d",bird_get_limit_speed_value());

	bird_set_limit_speed_value(0);
	yd_send_save_nv_msg();
}

U8 uart_charging_status=0;
void yd_uart_charging_on()
{
     kal_prompt_trace(MOD_SOC,"yd_uart_charging_on %d",uart_charging_status);
     if(uart_charging_status==0)
     {
         uart_charging_status=1;
         bird_soc_sendpos();
     }
}
void yd_uart_uncharge()
{
     kal_prompt_trace(MOD_SOC,"yd_uart_uncharge",uart_charging_status);
     if(uart_charging_status==1)
     {
         uart_charging_status=0;
         bird_soc_sendpos();
     }
}

U8 get_uart_charging_status()
{
     if(get_uart_connect_status()==1)
         return uart_charging_status;
     else 
         return 0;	 	
}

void yd_uart_search_c9_01()
{
       BIRD_UART_C9_INFO c9_info;
	   
       kal_prompt_trace(MOD_SOC,"yd_uart_search_c9_01");
	   
       memset(&c9_info,0,sizeof(BIRD_UART_C9_INFO));   

       c9_info.defend_flag=1;
       c9_info.findcar_flag=0;
       c9_info.lockcar_flag=1;
       c9_info.limitspeed_flag=1;
       c9_info.anzuo_flag=1;
       c9_info.selfcheck_flag=0;
       c9_info.startup=0;

       c9_info.defend=yd_tk001_get_defense();
       c9_info.findcar_flag=0;
       c9_info.limitspeed=(bird_get_limit_speed_value()==0)?0:1;
       c9_info.limitspeed_value=bird_get_limit_speed_value()*UART_RATIO;
       c9_info.lockcar=bird_get_lock_state();
       c9_info.anzuo=yd_tk001_get_az_flag();
       c9_info.selfcheck=0;
       c9_info.startup=0;

       GPS2ECU_UART_wirte_C9_CMD1(&c9_info);

}
void yd_uart_search_c9_04()
{
       BIRD_UART_C9_04_INFO c9_04_info;
	   	   
       memset(&c9_04_info,0,sizeof(BIRD_UART_C9_04_INFO));   

	if(rj_led_status_info.b_GPS_IS_CLOSE == KAL_TRUE)   
	{
           //c9_04_info.gps_status=BIRD_UART_GPS_CLOSE;
	}
	else if(rj_led_status_info.b_GPS_IS_SERACHING==KAL_TRUE)   
           c9_04_info.gps_status=BIRD_UART_GPS_SEARCH;
	else
           c9_04_info.gps_status=BIRD_UART_GPS_LOCA_SUC;

	if(rj_led_status_info.b_GSM_IS_SERACHING==KAL_TRUE)   
           c9_04_info.gsm_status=BIRD_UART_GSM_SEARCH;
	else
           c9_04_info.gsm_status=BIRD_UART_GSM_OK;	

	if(rj_led_status_info.b_SIM_IS_SERACHING==KAL_TRUE)   
           c9_04_info.sim_status=BIRD_UART_SIM_NOK;
 	else  
           c9_04_info.sim_status=BIRD_UART_SIM_OK;

	c9_04_info.vibrate_status=Yd_get_equstatus();
	#ifdef BIRD_BT_SUPPORT
	c9_04_info.btconnect_status=bird_get_bt_connect_status();
	c9_04_info.btKey_induction_status=bird_get_bt_induction_status();	
	#endif
	
	kal_prompt_trace(MOD_SOC,"yd_uart_search_c9_04 %d %d %d %d",c9_04_info.gps_status,c9_04_info.gsm_status,c9_04_info.sim_status,c9_04_info.vibrate_status);

       GPS2ECU_UART_wirte_C9_CMD4(&c9_04_info);

}

void yd_uart_run_status(U8 run_state)
{
       BIRD_UART_C9_04_INFO c9_04_info;
	   	   
       memset(&c9_04_info,0,sizeof(BIRD_UART_C9_04_INFO));   

	if(rj_led_status_info.b_GPS_IS_CLOSE == KAL_TRUE)   
	{
           //c9_04_info.gps_status=BIRD_UART_GPS_CLOSE;
	}
	else if(rj_led_status_info.b_GPS_IS_SERACHING==KAL_TRUE)   
           c9_04_info.gps_status=BIRD_UART_GPS_SEARCH;
	else
           c9_04_info.gps_status=BIRD_UART_GPS_LOCA_SUC;

	if(rj_led_status_info.b_GSM_IS_SERACHING==KAL_TRUE)   
           c9_04_info.gsm_status=BIRD_UART_GSM_SEARCH;
	else
           c9_04_info.gsm_status=BIRD_UART_GSM_OK;	

	if(rj_led_status_info.b_SIM_IS_SERACHING==KAL_TRUE)   
           c9_04_info.sim_status=BIRD_UART_SIM_NOK;
 	else  
           c9_04_info.sim_status=BIRD_UART_SIM_OK;

	c9_04_info.vibrate_status=run_state;
	#ifdef BIRD_BT_SUPPORT
	c9_04_info.btconnect_status=bird_get_bt_connect_status();
	c9_04_info.btKey_induction_status=bird_get_bt_induction_status();	
	#endif
	
	kal_prompt_trace(MOD_SOC,"yd_uart_run_status %d %d %d %d",c9_04_info.gps_status,c9_04_info.gsm_status,c9_04_info.sim_status,c9_04_info.vibrate_status);

       GPS2ECU_UART_wirte_C9_CMD4(&c9_04_info);

}

U8* yd_get_uart_model(BIRD_URAT_DATA_ENUM uart_data_type)
{
       kal_prompt_trace(MOD_SOC,"yd_get_uart_model %d",uart_data_type);

	 switch(uart_data_type)
        {
        	case BIRD_URAT_DATA_realtime: 
			return (U8*)YD_TK001_T37_realtime;
			break;
		case BIRD_URAT_DATA_fault:
			return (U8*)YD_TK001_T37_fault;
			break;
		case BIRD_URAT_DATA_battery:
			return (U8*)YD_TK001_T37_battery;
			break;
		case BIRD_URAT_DATA_alarm:
			return (U8*)YD_TK001_T37_alarm;
			break;
		case BIRD_URAT_DATA_param:
			return (U8*)YD_TK001_T37_param;
			break;
		case BIRD_URAT_DATA_selfdefine: 
			return (U8*)YD_TK001_T37_selfdefine;
			break;
		case BIRD_URAT_DATA_hardware:
			return (U8*)YD_TK001_T37_hardware;
			break;
		default:
			 (U8*)YD_TK001_T37_realtime;
			break;
       }	
}

U8* yd_get_T37_vendor()
{
     kal_prompt_trace(MOD_SOC,"yd_get_T37_vendor"); 

     return (U8*)YD_TK001_T37_vendor;
}

U8 g_realtime_length=0;
U8 g_fault_length=0;
U8 g_battery_length=0;
U8 g_alarm_length=0;
U8 g_selfdefine_length=0;
U8 g_hardware_length=0;
U8* yd_get_uart_buf(BIRD_URAT_DATA_ENUM uart_data_type)
{
       U8 i=0,j=0;
       kal_prompt_trace(MOD_SOC,"yd_get_uart_buf %d",uart_data_type);

	 switch(uart_data_type)
        {
        	case BIRD_URAT_DATA_realtime: 
			g_realtime_length=ECU_C0_CMD2_length();
			for(i=0;i<g_realtime_length;i++)
			{
				if(ECU_SUMbuf[i]==0x5B)
				{
				    for(j=g_realtime_length-1;j>i;j--)
				        ECU_SUMbuf[j+1]=ECU_SUMbuf[j];
				    ECU_SUMbuf[i+1]=0x02;
				    ECU_SUMbuf[i]=0x7E;
				    g_realtime_length++;
				    i++;	
				}
				else if(ECU_SUMbuf[i]==0x5D)
				{
				    for(j=g_realtime_length-1;j>i;j--)
				        ECU_SUMbuf[j+1]=ECU_SUMbuf[j];
				    ECU_SUMbuf[i+1]=0x02;
				    ECU_SUMbuf[i]=0x7C;
				    g_realtime_length++;
				    i++;			
				}
				else if((ECU_SUMbuf[i]==0x7E)||(ECU_SUMbuf[i]==0x7C))
				{
				    for(j=g_realtime_length-1;j>i;j--)
				        ECU_SUMbuf[j+1]=ECU_SUMbuf[j];
				    ECU_SUMbuf[i+1]=0x01;
				    g_realtime_length++;
				    i++;			
				}
			}
			return ECU_SUMbuf;
			break;
		case BIRD_URAT_DATA_fault:
			g_fault_length=15;
			for(i=0;i<g_fault_length;i++)
			{
				if(ECU2GPS_C0_CMD3[i]==0x5B)
				{
				    for(j=g_fault_length-1;j>i;j--)
				        ECU2GPS_C0_CMD3[j+1]=ECU2GPS_C0_CMD3[j];
				    ECU2GPS_C0_CMD3[i+1]=0x02;
				    ECU2GPS_C0_CMD3[i]=0x7E;
				    g_fault_length++;
				    i++;			
				}
				else if(ECU2GPS_C0_CMD3[i]==0x5D)
				{
				    for(j=g_fault_length-1;j>i;j--)
				        ECU2GPS_C0_CMD3[j+1]=ECU2GPS_C0_CMD3[j];
				    ECU2GPS_C0_CMD3[i+1]=0x02;
				    ECU2GPS_C0_CMD3[i]=0x7C;
				    g_fault_length++;
				    i++;			
				}
				else if((ECU2GPS_C0_CMD3[i]==0x7E)||(ECU2GPS_C0_CMD3[i]==0x7C))
				{
				    for(j=g_fault_length-1;j>i;j--)
				        ECU2GPS_C0_CMD3[j+1]=ECU2GPS_C0_CMD3[j];
				    ECU2GPS_C0_CMD3[i+1]=0x01;
				    g_fault_length++;
				    i++;			
				}
			}

			return ECU2GPS_C0_CMD3;
			break;
		case BIRD_URAT_DATA_battery:
			g_battery_length=24;
			for(i=0;i<g_battery_length;i++)
			{
				if(ECU2GPS_C0_CMD11[i]==0x5B)
				{
 				    for(j=g_battery_length-1;j>i;j--)
				        ECU2GPS_C0_CMD11[j+1]=ECU2GPS_C0_CMD11[j];
				    ECU2GPS_C0_CMD11[i+1]=0x02;
				    ECU2GPS_C0_CMD11[i]=0x7E;
				    g_battery_length++;
				    i++;					
				}
				else if(ECU2GPS_C0_CMD11[i]==0x5D)
				{
 				    for(j=g_battery_length-1;j>i;j--)
				        ECU2GPS_C0_CMD11[j+1]=ECU2GPS_C0_CMD11[j];
				    ECU2GPS_C0_CMD11[i+1]=0x02;
				    ECU2GPS_C0_CMD11[i]=0x7C;
				    g_battery_length++;
				    i++;			
				}
				else if((ECU2GPS_C0_CMD11[i]==0x7E)||(ECU2GPS_C0_CMD11[i]==0x7C))
				{
				    for(j=g_battery_length-1;j>i;j--)
				        ECU2GPS_C0_CMD11[j+1]=ECU2GPS_C0_CMD11[j];
				    ECU2GPS_C0_CMD11[i+1]=0x01;
				    g_battery_length++;
				    i++;			
				}
			}
			return (U8*)ECU2GPS_C0_CMD11;
			break;
		case BIRD_URAT_DATA_alarm:
			g_alarm_length=9;
			for(i=0;i<g_alarm_length;i++)
			{
				if(ECU2GPS_C0_CMD6[i]==0x5B)
				{
				    for(j=g_alarm_length-1;j>i;j--)
				        ECU2GPS_C0_CMD6[j+1]=ECU2GPS_C0_CMD6[j];
				    ECU2GPS_C0_CMD6[i+1]=0x02;
				    ECU2GPS_C0_CMD6[i]=0x7E;
				    g_alarm_length++;
				    i++;
				}
				else if(ECU2GPS_C0_CMD6[i]==0x5D)
				{
 				    for(j=g_alarm_length-1;j>i;j--)
				        ECU2GPS_C0_CMD6[j+1]=ECU2GPS_C0_CMD6[j];
				    ECU2GPS_C0_CMD6[i+1]=0x02;
				    ECU2GPS_C0_CMD6[i]=0x7C;
				    g_alarm_length++;
				    i++;	
				}
				else if((ECU2GPS_C0_CMD6[i]==0x7E)||(ECU2GPS_C0_CMD6[i]==0x7C))
				{
 				    for(j=g_alarm_length-1;j>i;j--)
				        ECU2GPS_C0_CMD6[j+1]=ECU2GPS_C0_CMD6[j];
				    ECU2GPS_C0_CMD6[i+1]=0x01;
				    g_alarm_length++;
				    i++;
				}
			}
			return ECU2GPS_C0_CMD6;
			break;
		//case BIRD_URAT_DATA_param:
			//return (U8*)ECU_SUMbuf;
			//break;
		case BIRD_URAT_DATA_selfdefine: 
			g_selfdefine_length=24;
			for(i=0;i<g_selfdefine_length;i++)
			{
				if(ECU2GPS_C0_CMD5[i]==0x5B)
				{
				    for(j=g_selfdefine_length-1;j>i;j--)
				        ECU2GPS_C0_CMD5[j+1]=ECU2GPS_C0_CMD5[j];
				    ECU2GPS_C0_CMD5[i+1]=0x02;
				    ECU2GPS_C0_CMD5[i]=0x7E;
				    g_selfdefine_length++;
				    i++;	
				}
				else if(ECU2GPS_C0_CMD5[i]==0x5D)
				{
 				    for(j=g_selfdefine_length-1;j>i;j--)
				        ECU2GPS_C0_CMD5[j+1]=ECU2GPS_C0_CMD5[j];
				    ECU2GPS_C0_CMD5[i+1]=0x02;
				    ECU2GPS_C0_CMD5[i]=0x7C;
				    g_selfdefine_length++;
				    i++;
				}
				else if((ECU2GPS_C0_CMD5[i]==0x7E)||(ECU2GPS_C0_CMD5[i]==0x7C))
				{
 				    for(j=g_selfdefine_length-1;j>i;j--)
				        ECU2GPS_C0_CMD5[j+1]=ECU2GPS_C0_CMD5[j];
				    ECU2GPS_C0_CMD5[i+1]=0x01;
				    g_selfdefine_length++;
				    i++;
				}
			}
			return (U8*)ECU2GPS_C0_CMD5;
			break;
		case BIRD_URAT_DATA_hardware:
			g_hardware_length=24;
			for(i=0;i<g_hardware_length;i++)
			{
				if(ECU2GPS_C0_CMD10[i]==0x5B)
				{
				    for(j=g_hardware_length-1;j>i;j--)
				        ECU2GPS_C0_CMD10[j+1]=ECU2GPS_C0_CMD10[j];
				    ECU2GPS_C0_CMD10[i+1]=0x02;
				    ECU2GPS_C0_CMD10[i]=0x7E;
				    g_hardware_length++;
				    i++;
				}
				else if(ECU2GPS_C0_CMD10[i]==0x5D)
				{
				    for(j=g_hardware_length-1;j>i;j--)
				        ECU2GPS_C0_CMD10[j+1]=ECU2GPS_C0_CMD10[j];
				    ECU2GPS_C0_CMD10[i+1]=0x02;
				    ECU2GPS_C0_CMD10[i]=0x7C;
				    g_hardware_length++;
				    i++;			
				}
				else if((ECU2GPS_C0_CMD10[i]==0x7E)||(ECU2GPS_C0_CMD10[i]==0x7C))
				{
				    for(j=g_hardware_length-1;j>i;j--)
				        ECU2GPS_C0_CMD10[j+1]=ECU2GPS_C0_CMD10[j];
				    ECU2GPS_C0_CMD10[i+1]=0x01;
				    g_hardware_length++;
				    i++;
				}
			}
			return (U8*)ECU2GPS_C0_CMD10;
			break;
		default:
			 return (U8*)ECU_SUMbuf;
			break;
       }	
}

U8 yd_get_uart_buf_length(BIRD_URAT_DATA_ENUM uart_data_type)
{
       kal_prompt_trace(MOD_SOC,"yd_get_uart_buf %d",uart_data_type);

	 switch(uart_data_type)
        {
        	case BIRD_URAT_DATA_realtime: 
			return g_realtime_length;
			break;
		case BIRD_URAT_DATA_fault:
			return g_fault_length;
			break;
		case BIRD_URAT_DATA_battery:
			return g_battery_length;
			break;
		case BIRD_URAT_DATA_alarm:
			return g_alarm_length;
			break;
		//case BIRD_URAT_DATA_param:
			//return ECU_C0_CMD2_length();
			//break;
		case BIRD_URAT_DATA_selfdefine: 
			return g_selfdefine_length;
			break;
		case BIRD_URAT_DATA_hardware:
			return g_hardware_length;
			break;
		default:
			ECU_C0_CMD2_length();
			break;
       }	
}

void yd_uart_real_time_data()
{
       kal_prompt_trace(MOD_SOC,"yd_uart_real_time_data");

       bird_soc_uart_T37_send(BIRD_URAT_DATA_realtime);
}

void yd_uart_fault_data()
{
       kal_prompt_trace(MOD_SOC,"yd_uart_fault_data");

       bird_soc_uart_T37_send(BIRD_URAT_DATA_fault);
}

void yd_uart_alarm_data()
{
       kal_prompt_trace(MOD_SOC,"yd_uart_alarm_data");

       bird_soc_uart_T37_send(BIRD_URAT_DATA_alarm);
}

void yd_uart_hardware_data()
{
       kal_prompt_trace(MOD_SOC,"yd_uart_hardware_data");

       bird_soc_uart_T37_send(BIRD_URAT_DATA_hardware);
}

void yd_uart_battery_data()
{
       kal_prompt_trace(MOD_SOC,"yd_uart_battery_data");

       bird_soc_uart_T37_send(BIRD_URAT_DATA_battery);
}

void yd_uart_selfdefine_data()
{
       kal_prompt_trace(MOD_SOC,"yd_uart_selfdefine_data");

       bird_soc_uart_T37_send(BIRD_URAT_DATA_selfdefine);
}

void yd_uart_data_to_server()
{
       kal_prompt_trace(MOD_SOC,"yd_uart_data_to_server %d",ECU_C0_CMD2_flag());

       if(ECU_C0_CMD2_flag())
           yd_uart_real_time_data();
       StartTimer(BIRD_ECU_CONNECT_TIMER, 15*1000, yd_uart_data_to_server);
}

void yd_uart_battery_data_to_server()
{
       U8 ntimer=bird_get_uart_bat_interval();

       kal_prompt_trace(MOD_SOC,"yd_uart_battery_data_to_server %d",ntimer);

       yd_uart_battery_data();
	   
       StartTimer(BIRD_ECU_BATTERY_TIMER, ntimer*60*1000, yd_uart_battery_data_to_server);
}

U8 uart_connect_status=1;
void yd_uart_connect()
{
       U8 ntimer=bird_get_uart_bat_interval();

	uart_connect_status=1;  
	   
       kal_prompt_trace(MOD_SOC,"yd_uart_connect %d",ntimer);
       if(!yd_tk001_get_reboot_flag())
           bird_soc_sendpos();
       StartTimer(BIRD_ECU_CONNECT_TIMER, 15*1000, yd_uart_data_to_server);
       StartTimer(BIRD_ECU_BATTERY_TIMER, ntimer*60*1000, yd_uart_battery_data_to_server);

}
void yd_uart_disconnect()
{
       kal_prompt_trace(MOD_SOC,"yd_uart_disconnect");

	uart_connect_status=0;  
       if(!yd_tk001_get_reboot_flag())
           bird_soc_sendpos();
	StopTimer(BIRD_ECU_CONNECT_TIMER);
	StopTimer(BIRD_ECU_BATTERY_TIMER);
}
U8 get_uart_connect_status()
{
     kal_prompt_trace(MOD_SOC,"get_uart_connect_status %d",uart_connect_status);

     return uart_connect_status;
}
void set_uart_connect_status(U8 flag)
{
     kal_prompt_trace(MOD_SOC,"set_uart_connect_status %d",flag);

     uart_connect_status=flag;
}

U8 get_uart_battery_percent()
{
     U8 value =0;

     kal_prompt_trace(MOD_SOC,"get_uart_battery_percent %d",ECU_BATTERY_percent());
	 
     value=(U8)ECU_BATTERY_percent()/UART_RATIO;
	 
     if(value>100)	 
	 	value=100;
     if(value<0)	 
	 	value=0;

     return value;
}

U8 get_uart_battery_percent_ext()
{

     kal_prompt_trace(MOD_SOC,"get_uart_battery_percent %d %d",get_uart_connect_status(),get_uart_battery_percent());
	 
     if(get_uart_connect_status()==1)
	 	return get_uart_battery_percent();
     else
	 	return 0;
}

//uart 消息注册函数
void Yd_set_uart_msg_handler()
{
	yd_set_uart_vibration_alarm_msg();
	yd_set_uart_power_off_msg();
	yd_set_uart_reboot_msg();
	
	yd_set_uart_run_event_msg();
	yd_set_uart_static_event_msg();
	
	yd_set_uart_defend_on_msg();
	yd_set_uart_defend_off_msg();
	
	yd_set_ECU_command_acc_on_msg();
	yd_set_ECU_command_acc_off_msg();
	yd_set_ECU_key_on_msg();
	yd_set_ECU_key_off_msg();

	SetProtocolEventHandler(yd_uart_lock,MSG_ID_YD_TK001_MSG_DISCONNECT_MOTOR_STATUS);
	SetProtocolEventHandler(yd_uart_unlock,MSG_ID_YD_TK001_MSG_NORMAL_MOTOR_STATUS);
	
	SetProtocolEventHandler(yd_uart_charging_on,MSG_ID_YD_TK001_MSG_ECU_CHARGING_ON);
	SetProtocolEventHandler(yd_uart_uncharge,MSG_ID_YD_TK001_MSG_ECU_UNCHARGING);
	//SetProtocolEventHandler(yd_uart_limit_speed_on,MSG_ID_YD_TK001_MSG_SPEED_LIMIT_ON);
	//SetProtocolEventHandler(yd_uart_limit_speed_off,MSG_ID_YD_TK001_MSG_SPEED_LIMIT_OFF);
	
	SetProtocolEventHandler(yd_uart_a0_response,MSG_ID_YD_TK001_MSG_A0_CMD_ALARM);
	
	SetProtocolEventHandler(yd_uart_search_c9_01,MSG_ID_YD_TK001_MSG_C9_CMD1_INQUIRE);
	//SetProtocolEventHandler(yd_c9_init,MSG_ID_YD_TK001_MSG_C9_CMD2_INQUIRE);
	//SetProtocolEventHandler(yd_c9_init,MSG_ID_YD_TK001_MSG_C9_CMD3_INQUIRE);
	SetProtocolEventHandler(yd_uart_search_c9_04,MSG_ID_YD_TK001_MSG_C9_CMD4_INQUIRE);

	//uart 连接和断开消息
	SetProtocolEventHandler(yd_uart_disconnect,MSG_ID_AUX_ECU_DISCONNECT_ALARM);
	SetProtocolEventHandler(yd_uart_connect,MSG_ID_AUX_ECU_CONNECT_ALARM);

       //透传数据
	SetProtocolEventHandler(yd_uart_fault_data,MSG_ID_FAULT_CMD_ALARM);   
	SetProtocolEventHandler(yd_uart_alarm_data,MSG_ID_ALARM_CMD);   
	SetProtocolEventHandler(yd_uart_hardware_data,MSG_ID_HARDWARE_CONFIG_CMD_ALARM);   
	SetProtocolEventHandler(yd_uart_battery_response,MSG_ID_BATTERY_CMD_ALARM);   
	SetProtocolEventHandler(yd_uart_selfdefine_data,MSG_ID_SELFDEFINE_DATA_CMD);   

       //整车启动消息
	SetProtocolEventHandler(yd_uart_car_start,MSG_ID_YD_TK001_MSG_ECU_START);   
	SetProtocolEventHandler(yd_uart_car_close,MSG_ID_YD_TK001_MSG_ECU_CLOSE);   

	//SetProtocolEventHandler(,MSG_ID_YD_TK001_MSG_PLAYTONE_ALARM);   
	//SetProtocolEventHandler(,MSG_ID_YD_TK001_MSG_BT_ACCREDIT);   
	//SetProtocolEventHandler(,MSG_ID_BT_SENSOR_OPEN);   
	//SetProtocolEventHandler(,MSG_ID_BT_SENSOR_CLOSE);   
}

#endif
