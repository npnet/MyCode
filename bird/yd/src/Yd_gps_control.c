#include "Yd_gps_control.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"

U8 g_n_is_gps_close = 0;
/*=============================   使用的外部变量与函数====================================*/
extern RJ_POWER_GSM_GPRS_status_Info rj_led_status_info;

extern U8 RJ_GPS_GetGpsModule(void);
extern void RJ_GPS_Stop(void);
extern void Bird_LED_Identify_Wink(void);
extern void RJ_GPS_Gsmled_LightWink(void);
extern void RJ_GPS_Enter(void);
extern U32 bird_get_main_interval_value();
extern void Lima_set_soc_init_flag(BOOL flag);
extern void Lima_Soc_Dinit(void);
extern U8 Yd_get_equstatus();
extern U8 yd_tk001_get_defense();


void yd_close_gps()
{
	kal_prompt_trace(MOD_SOC,"yd_close_gps msg %d",RJ_GPS_GetGpsModule());
	if(RJ_GPS_GetGpsModule()==1)
	{
		RJ_GPS_Stop();//CLOSE
	}
}

void yd_open_gps()
{
	kal_prompt_trace(MOD_SOC,"yd_open_gps msg %d",RJ_GPS_GetGpsModule());	  
	if(RJ_GPS_GetGpsModule()==0)
	{
		rj_led_status_info.b_GPS_IS_CLOSE = KAL_FALSE;
		rj_led_status_info.b_GPS_IS_SERACHING = KAL_TRUE;
		#ifdef __BIRD_LED_IDENTIFY_WINK__
			Bird_LED_Identify_Wink();
		#else
			RJ_GPS_Gsmled_LightWink();
		#endif
	
		RJ_GPS_Enter();//冷启动GPS  lrf modify		
	}
}


void yd_start_open_gps(void)
{
	kal_prompt_trace(MOD_SOC,"yd_start_open_gps %d",Yd_get_equstatus());
	{
		yd_send_gps_open_msg();	
	}
}

void yd_set_gps_open_msg()
{
	SetProtocolEventHandler(yd_open_gps,MSG_ID_YD_TK001_MSG_OPENGPS); 
}

void yd_set_gps_close_msg()
{
	SetProtocolEventHandler(yd_close_gps,MSG_ID_YD_TK001_MSG_CLOSEGPS); 
}

void yd_send_gps_open_msg()
{
	ilm_struct *bird_ilm = NULL;
	
	DRV_BuildPrimitive(bird_ilm, MOD_MMI, MOD_MMI, MSG_ID_YD_TK001_MSG_OPENGPS, NULL);       
	msg_send_ext_queue(bird_ilm);     
}

void yd_send_gps_close_msg()
{
	ilm_struct *bird_ilm = NULL;
	
	DRV_BuildPrimitive(bird_ilm, MOD_MMI, MOD_MMI, MSG_ID_YD_TK001_MSG_CLOSEGPS, NULL);       
	msg_send_ext_queue(bird_ilm);     
}

