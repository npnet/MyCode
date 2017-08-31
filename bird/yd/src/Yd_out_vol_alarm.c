#include "Yd_out_vol_alarm.h"
#include "Bird_task.h"
#include "Bird_app.h"
#include "Yd_main.h"

U8 g_adc_connect_post = 1;
U8 g_poweroff_count = 0;/*断电计数*/

/*=============================   使用的外部变量与函数====================================*/
extern YD_TK001_COMMAND_Info yd_tk001_info;
extern ALARM_msg_param alarm_msg[TK001_ALARM_ENUM_max];

extern U8 yd_upload_judge_function(ALARM_msg_param *alarm_kind);
extern void yd_set_compare_time_function(ALARM_msg_param *alarm_kind);
extern U8 bird_get_equmode();
extern void bird_soc_sendalarm(TK001_ALARM_ENUM alarm_category);
extern void bird_soc_sendpos();
#ifdef DW02_PROJECT
extern double bird_get_adc_channel_voltage(void);
#endif
extern kal_bool is_ac_charger_in(void);
extern U32 bird_get_poft_value();
extern U8 bird_get_normal_sleep_state();
extern void yd_tk001_wakeup_function();
extern void Yd_stop_sleepTimer();
extern void yd_send_gps_open_msg();
#ifdef RJ_GPS_APP
extern void bird_set_gpspositon(S8 updatekind);
#endif


void yd_init_power_off_alarm_param(kal_uint8 interval_time)
{
	alarm_msg[TK001_ALARM_ENUM_power_off].upload_onoff = 1;
	alarm_msg[TK001_ALARM_ENUM_power_off].call_onoff = 0;

	alarm_msg[TK001_ALARM_ENUM_power_off].alarm_times = 0;
	alarm_msg[TK001_ALARM_ENUM_power_off].interval_time = interval_time;
	memset(&(alarm_msg[TK001_ALARM_ENUM_power_off].compare_time),0,sizeof(alarm_msg[TK001_ALARM_ENUM_power_off].compare_time));
}

void bird_set_poft_value(U32 value)
{
	kal_prompt_trace(MOD_SOC,"bird_set_poft_value %d  \n",value);    
	yd_tk001_info.POFT = value;
}

U32 bird_get_poft_value()
{
	return yd_tk001_info.POFT;
}

void bird_set_external_power_alarm_flag(U8 flag)
{
	if(yd_tk001_info.external_power_alarm != flag)
	{
		yd_tk001_info.external_power_alarm = flag;
		alarm_msg[TK001_ALARM_ENUM_power_off].alarm_times = 0;
	}
}

U8 bird_get_power_dis_to_platform_flag()
{
	return yd_tk001_info.external_power_alarm ;
}

void yd_power_off_alarm_handler()
{
	if(bird_get_equmode()!=1)
	{
		alarm_msg[TK001_ALARM_ENUM_power_off].upload_onoff = bird_get_power_dis_to_platform_flag();
		alarm_msg[TK001_ALARM_ENUM_power_off].upload_onoff = yd_upload_judge_function(&(alarm_msg[TK001_ALARM_ENUM_power_off]));
	}
	yd_set_compare_time_function(&(alarm_msg[TK001_ALARM_ENUM_power_off]));
	kal_prompt_trace(MOD_SOC,"yd_power_off_alarm_handler upload = %d",alarm_msg[TK001_ALARM_ENUM_power_off].upload_onoff);     

	if(alarm_msg[TK001_ALARM_ENUM_power_off].upload_onoff)
	{
		bird_soc_sendalarm(TK001_ALARM_ENUM_power_off);	
	}
	else
	{
		bird_soc_sendpos();
	}
}

void yd_power_off_delay()
{
	double vol = bird_get_adc_channel_voltage();	
	if(!is_ac_charger_in() && vol<9.5)/*未接通USB且外接电压小于9.5V*/
	{
		g_poweroff_count++;
		if(g_poweroff_count>bird_get_poft_value())
		{
			g_poweroff_count = 0;
			if(bird_get_normal_sleep_state())
			{
				yd_tk001_wakeup_function(); //插电唤醒
			}
	    		Yd_stop_sleepTimer();
			yd_power_off_alarm_handler();
			g_adc_connect_post=1;
		}
		else
		{
	 		Rj_stop_timer(BIRD_TASK_POWER_OFF_DELAY);
	 		Rj_start_timer(BIRD_TASK_POWER_OFF_DELAY, 1000, yd_power_off_delay,NULL);	
			g_adc_connect_post=0;
		}
	} 
	else
	{
		Rj_stop_timer(BIRD_TASK_POWER_OFF_DELAY);
		g_poweroff_count = 0;
		g_adc_connect_post=0;
	}
}

void yd_tk001_post_alarm_power_on_off(void *data)
{
	kal_prompt_trace(MOD_SOC,"yd_tk001_post_alarm_power_on_off  msg_handle_function");  
	if( 0 == ((BIRD_id_struct*)data)->para_setting_id)/*外部电源断开*/
	{
		g_poweroff_count = 0;
		Rj_stop_timer(BIRD_TASK_POWER_OFF_DELAY);
		Rj_start_timer(BIRD_TASK_POWER_OFF_DELAY, 10*1000, yd_power_off_delay,NULL);		
		g_adc_connect_post=0;
	}
	else
	{
		Rj_stop_timer(BIRD_TASK_POWER_OFF_DELAY);
		g_poweroff_count = 0;
		if((g_adc_connect_post==1)||(bird_get_normal_sleep_state()))
		{
			Yd_stop_sleepTimer();		
			yd_tk001_wakeup_function(); /*插电唤醒*/
			bird_soc_sendpos();
		}
	}	
}

void yd_set_out_vol_alarm_msg()
{
	SetProtocolEventHandler(yd_tk001_post_alarm_power_on_off,MSG_ID_RJ_OUTSIDE_VOLT_DISCONNECT);
}

