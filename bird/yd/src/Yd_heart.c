#include "Yd_heart.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"


/*===============================使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern U8 rj_gps_stage;/* 主逻辑运行状态标识 */
extern U8 g_n_is_gps_close;/*GPS关闭状态标识*/

extern U8 bird_get_normal_sleep_state(void);
extern void bird_set_normal_sleep_state(U8 flag);
extern void Yd_DinitSocket();
extern BOOL Lima_get_soc_conn_flag();
extern void Bird_soc_conn();
extern void bird_set_gpspositon(S8 updatekind);
extern U8 yd_tk001_get_reboot_flag(void);
extern void bird_soc_sendpos();
extern U32 bird_get_wakeupt_value();
extern void bird_soc_sendheart();
extern void bird_soc_send_tboxheart();
extern U8 Yd_get_lc_static_count();
extern U8 Yd_get_lc_station_count();
extern void yd_send_gps_close_msg();


void bird_set_heart_interval_value(U32 interval)
{
	yd_tk001_info.heart_interval = interval;
}

U32 bird_get_heart_interval_value()
{
	return yd_tk001_info.heart_interval;
}

void Yd_heart_res(U8 *datetime)
{
	//set time
	//yd_set_utility_time(datetime);
	kal_prompt_trace(MOD_SOC," Yd_heart_res entry");
	if(bird_get_normal_sleep_state()==1)
	{
		kal_prompt_trace(MOD_SOC," Yd_heart_res run %d",rj_gps_stage);
		if(rj_gps_stage == BIRD_STATUS_SLEEPSENDHEART)
		{
			rj_gps_stage = BIRD_STATUS_SLEEP;
			Rj_stop_timer(Bird_task_sleep_dinisocket_Timer);
			Rj_start_timer(Bird_task_sleep_dinisocket_Timer, RJ_GPS_APP_1M*2, Yd_DinitSocket,NULL); /*2分钟之后,反初始化socket*/
		}
	}
}

void Yd_heart()
{
	kal_uint32 ntimer = 30000;
	U8 n1=0;
	U8 n2=0;

	ntimer = 1000*bird_get_heart_interval_value();
	 switch(rj_gps_stage)
        {
        	case BIRD_STATUS_RUN: 
		case BIRD_STATUS_HEART:
		case BIRD_STATUS_HEARTBEFORE:
		case BIRD_STATUS_SLEEPSENDHEART:
		case BIRD_STATUS_SLEEPBEFORE:
		{
			bird_soc_sendheart();
			break;
		}
		case BIRD_STATUS_SLEEP: 
		default:
			break;
       }
	//if close gps
	n1=Yd_get_lc_static_count();/*持续静止次数(每20秒查询一次)*/
	n2=Yd_get_lc_station_count();/*里程统计时,非休眠状态，卫星持续不可用次数*/
	kal_prompt_trace(MOD_SOC," Yd_heart n1 = %d,n2 = %d ",n1,n2);
	if(n1>90 && n2>90)
	{
		if(g_n_is_gps_close==0)
		{
			g_n_is_gps_close=1;/*gps关闭标志位置1*/
			rj_led_status_info.b_GPS_IS_CLOSE = KAL_TRUE;
			kal_prompt_trace(MOD_SOC," Yd_heart close_gps");
			yd_send_gps_close_msg();		
		}
	}
	Rj_stop_timer(Bird_task_heart_Timer); 
	Rj_start_timer(Bird_task_heart_Timer, ntimer, Yd_heart,NULL);
}

void Yd_tbox_heart()
{
	kal_uint32 ntimer = 30000;

	kal_prompt_trace(MOD_SOC," Yd_tbox_heart ");

	ntimer = 1000*bird_get_heart_ival();
	bird_soc_send_tboxheart();
	Rj_stop_timer(Bird_task_heart_Timer); 
	Rj_start_timer(Bird_task_heart_Timer, ntimer, Yd_tbox_heart,NULL);
}


