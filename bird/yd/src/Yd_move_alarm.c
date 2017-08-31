#include "Yd_move_alarm.h"
#include "Bird_app.h"
#include "Yd_main.h"

 /*位移处理*/
U8 g_move_count  = 0;       
U8 g_move_timer=0;

/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern ALARM_msg_param alarm_msg[TK001_ALARM_ENUM_max];
extern kal_bool volatile b_gps_msg_post;
extern RJ_Gps_Position yd_set_defense_position;
extern RJ_Gps_Position gps_position;

extern U8 yd_upload_judge_function(ALARM_msg_param *alarm_kind);
extern void yd_set_compare_time_function(ALARM_msg_param *alarm_kind);
extern U8 bird_get_equmode();
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);
extern U8 yd_tk001_get_defense();
extern void bird_soc_sendpos();
extern U32 bird_get_alarm_delay_value();
extern U8 RJ_GPS_is_working(void);
extern U8 Yd_get_equstatus();
extern S32 RJ_GPS_GetDistance(double lat1, double lng1, double lat2, double lng2);


void yd_init_move_alarm_param(kal_uint8 interval_time)
{
	alarm_msg[TK001_ALARM_ENUM_moved].upload_onoff = 1;
	alarm_msg[TK001_ALARM_ENUM_moved].call_onoff = 0;

	alarm_msg[TK001_ALARM_ENUM_moved].alarm_times = 0;
	alarm_msg[TK001_ALARM_ENUM_moved].interval_time = interval_time;
	memset(&(alarm_msg[TK001_ALARM_ENUM_moved].compare_time),0,sizeof(alarm_msg[TK001_ALARM_ENUM_moved].compare_time));
}

void bird_set_move_position_alarm_value(U32 distance)
{
	if(yd_tk001_info.move_position_distance != distance)
	{
		yd_tk001_info.move_position_distance = distance;
		alarm_msg[TK001_ALARM_ENUM_moved].alarm_times = 0;
	}
}

U32 bird_get_move_position_alarm_value()
{
	return yd_tk001_info.move_position_distance;
}

void bird_set_move_position_alarm_flag(U8 flag)
{
	yd_tk001_info.move_position_alarm = flag;
}

U8 bird_get_enable_move_to_platform_flag()
{
	return yd_tk001_info.move_position_alarm;
}

void yd_move_alarm_handler()
{
	alarm_msg[TK001_ALARM_ENUM_moved].upload_onoff = bird_get_enable_move_to_platform_flag();
	alarm_msg[TK001_ALARM_ENUM_moved].upload_onoff = yd_upload_judge_function(&alarm_msg[TK001_ALARM_ENUM_moved]);
	if(bird_get_equmode()!=1)
	{
		yd_set_compare_time_function(&alarm_msg[TK001_ALARM_ENUM_moved]);
	}
	
	kal_prompt_trace(MOD_SOC,"yd_move_alarm_handler  upload_onoff = %d",alarm_msg[TK001_ALARM_ENUM_moved].upload_onoff);

	if(alarm_msg[TK001_ALARM_ENUM_moved].upload_onoff)
	{
		bird_soc_sendalarm(TK001_ALARM_ENUM_moved);
	}
}

void yd_move_alarm_msg_handler()
{
	ilm_struct       *rj_ilm = NULL; 
	kal_prompt_trace(MOD_SOC,"yd_move_alarm_msg_handler  defense = %d",yd_tk001_get_defense());
	if(!yd_tk001_get_defense())
	{
		return;
	}
	yd_move_alarm_handler();

	bird_soc_sendpos(); 
}

void yd_set_move_alarm_msg()
{
	SetProtocolEventHandler(yd_move_alarm_msg_handler,MSG_ID_MMI_YD_TK001_MSG_ALARM_MOVED);
}


void Bird_move_ext()
{
	kal_prompt_trace(MOD_SOC,"Bird_move_ext");
	StopTimer(BIRD_SET_MOVE_DELAY_TIMER);
	g_move_timer=0;

}

void RJ_GPS_distance_handle(mdi_gps_nmea_gga_struct rmc_data)	
//void RJ_GPS_distance_handle(mdi_gps_nmea_rmc_struct rmc_data)
{
	if(b_gps_msg_post)
	{
		if(RJ_GPS_is_working())
		{       
			if (gps_position.ishavedate==1 && yd_set_defense_position.ishavedate==0)
			{
				yd_set_defense_position.la = gps_position.la;
				yd_set_defense_position.lg = gps_position.lg;
				yd_set_defense_position.ishavedate = 1;	
				b_gps_msg_post = MMI_FALSE;
			}
		}
	}
	if(Yd_get_equstatus()==1 && yd_set_defense_position.ishavedate==1 && gps_position.ishavedate == 1)
	{
		U32 dis =0;

		dis = RJ_GPS_GetDistance( yd_set_defense_position.la, yd_set_defense_position.lg, gps_position.la, gps_position.lg) ;        
		kal_prompt_trace(MOD_SOC,"RJ_GPS_distance_handle dis=%d move_position_alarm_value=%d Yd_get_equstatus=%d",dis,bird_get_move_position_alarm_value(),Yd_get_equstatus()); 
		if(dis >bird_get_move_position_alarm_value())
		{
			if((g_move_timer == 0)&&Yd_get_equstatus())
			{
				ilm_struct *post_ilm;         
				g_move_timer=1;
				DRV_BuildPrimitive(post_ilm, MOD_MMI, MOD_MMI, MSG_ID_MMI_YD_TK001_MSG_ALARM_MOVED, NULL);       
				msg_send_ext_queue(post_ilm);
				b_gps_msg_post = MMI_TRUE;/*移动流程中删除原始信息，置1后允许刷新原始位置*/        
				StopTimer(BIRD_SET_MOVE_DELAY_TIMER);
				StartTimer(BIRD_SET_MOVE_DELAY_TIMER, 60*1000, Bird_move_ext);			
			}
		}
	}
}

//暂未使用
void Bird_move_delay()
{
	StopTimer(BIRD_SET_MOVE_DELAY_TIMER);
	if(yd_tk001_get_defense())
	{
		g_move_count++;
		if(g_move_count>bird_get_alarm_delay_value())
		{			
			ilm_struct *post_ilm;               
           		DRV_BuildPrimitive(post_ilm, MOD_MMI, MOD_MMI, MSG_ID_MMI_YD_TK001_MSG_ALARM_MOVED, NULL);       
		       msg_send_ext_queue(post_ilm);
			g_move_count = 0;   
	           	b_gps_msg_post = MMI_TRUE;			
		}	
		else
		{
			StartTimer(BIRD_SET_MOVE_DELAY_TIMER, 1000, Bird_move_delay);
		}
	}
	else
	{
		g_move_count=0;
	}
}

